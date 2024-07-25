#include "KeypadPCF8575.h"

KeypadPCF8575::KeypadPCF8575(PCF8575 *ioChipToSet, char *userKeymap, byte *row, byte *col, byte numRows, byte numCols) : Keypad(userKeymap, row, col, numRows, numCols)
{
    ioChip = ioChipToSet;
    // Build the input check mask, that is, the bit mask sent to pcf8575 to set all rows high, and columns low
    InputCheckMask = 0x0000;
    for (uint8_t RowIndex = 0; RowIndex < sizeKpd.rows; RowIndex++)
        bitWrite(InputCheckMask, rowPins[RowIndex], HIGH);
    memset(previousBitmap, 0, MAPSIZE * sizeof(int));
    // Serial.printf("InputCheckMask = 0x%4x\n", InputCheckMask);
    // for (uint8_t RowIndex = 0; RowIndex < sizeKpd.rows; RowIndex++)
    //     Serial.printf("Rowpin %d: %d\n", RowIndex, rowPins[RowIndex]);
    // for (uint8_t ColIndex = 0; ColIndex < sizeKpd.rows; ColIndex++)
    //     Serial.printf("Rowpin %d: %d\n", ColIndex, columnPins[ColIndex]);
}

void KeypadPCF8575::pin_mode(byte pinNum, byte mode)
{
}
void KeypadPCF8575::pin_write(byte pinNum, boolean level)
{
    ioChip->write(pinNum, level);
}
int KeypadPCF8575::pin_read(byte pinNum)
{
    return ioChip->read(pinNum);
}
void KeypadPCF8575::scanKeys()
{    
    // From RobTillaart I2CKeyPad8x8 :
    // improve the odds that IO will not interrupted.
    yield();
    ioChip->write16(InputCheckMask);
    // Read all the rows.
    // If any are grounded, then we know a switch is active.
    InputBuffer = ioChip->read16();    
    // If nothing active, reset the bitMap to all off and return.
    if ((InputBuffer & InputCheckMask) == InputCheckMask)
    {
        // no keypressed, reset the entire keypad bitmap
        for (uint8_t RowIndex = 0; RowIndex < sizeKpd.rows; RowIndex++)
            bitMap[RowIndex] = 0;
        // Turn off output.
        OutputBuffer = 0xFFFF;
        ioChip->write16(OutputBuffer);
        return;
    }
    // if we detected a keypress, use it as a mask. It seems to help with floating lines strange behavior
    int16_t readmask = (~InputBuffer) & InputCheckMask;
    int16_t colread = 0;
    // At least one button is active. So read one column at a time.
    // Test each column independently and write results to bitMap.
    for (uint8_t ColumnIndex = 0; ColumnIndex < sizeKpd.columns; ColumnIndex++)
    {
        OutputBuffer = 0xFFFF;
        bitWrite(OutputBuffer, columnPins[ColumnIndex], LOW);
        ioChip->write16(OutputBuffer);
        InputBuffer = ioChip->read16();
        // Invert result and apply the readmask
        colread = (~InputBuffer) & readmask;
        // Check columns in the current row
        for (uint8_t RowIndex = 0; RowIndex < sizeKpd.rows; RowIndex++)
        {
            // update keypad bitmap accordingly
            bitWrite(bitMap[RowIndex], ColumnIndex, bitRead(colread, rowPins[RowIndex]));
        }
    }
    // Disable chip inputs
    ioChip->write16(0xFFFF);
//    ioChip->write16(InputCheckMask);
}

void KeypadPCF8575::addChangesEventListener(void (*listener)(uint8_t[], uint8_t[], uint8_t, uint8_t))
{
    KeypadPCF8575EventListener = listener;
}

void KeypadPCF8575::enableInterrupt()
{
    ioChip->write16(InputCheckMask);
}

bool KeypadPCF8575::getKeys() {
	bool keyActivity = false;
    uint16_t delta, currentRow;
    uint8_t keyCode;    
    uint8_t pressedCount = 0;
    uint8_t releasedCount = 0;
    memset(Pressed, -1, MAX_LIST_SIZE);
    memset(Released, -1, MAX_LIST_SIZE);
	// Limit how often the keypad is scanned. This makes the loop() run 10 times as fast.
	if ( (millis()-startTime)>debounceTime ) {
		scanKeys();
        for (uint8_t row = 0; row < sizeKpd.rows; row++) {
            currentRow = bitMap[row];
            if (previousBitmap[row] != currentRow) {
                delta = previousBitmap[row] ^ currentRow;
                for (uint8_t col = 0; col < sizeKpd.columns; col++)
                    if (bitRead(delta, col)) {
                        int keyCode = row * sizeKpd.columns + col;
                        if (bitRead(currentRow, col))
                            Pressed[pressedCount++] = keyCode;
                        else
                            Released[releasedCount++] = keyCode;
                    }
                previousBitmap[row] = currentRow;
            }
        }
		startTime = millis();
	}

    if ((pressedCount > 0) || (releasedCount > 0)) {
    	if (KeypadPCF8575EventListener!=NULL)
		{
			KeypadPCF8575EventListener(Pressed, Released, pressedCount, releasedCount);
		}
        keyActivity = true;
    }
	return keyActivity;
}