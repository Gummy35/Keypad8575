#include "KeypadPCF8574.h"

KeypadPCF8574::KeypadPCF8574(*PCF8574 ioChipToSet, char *userKeymap, byte *row, byte *col, byte numRows, byte numCols) : KeyPad(userKeymap, row, col, numRows, numCols)
{
	ioChip = ioChipToSet;
}

void KeypadPCF8574::pin_mode(byte pinNum, byte mode)
{

}
void KeypadPCF8574::pin_write(byte pinNum, boolean level)
{
	ioChip->write(pinNum, level);
}
int  KeypadPCF8574::pin_read(byte pinNum)
{
	return ioChip->read(pinNum);
}
void KeypadPCF8574::scanKeys()
{
	//Turn on all columns and rows.
	//The PCF8574 is better at grounding than raising a pin.
	OutputBuffer = 0xFF;
	for (uint8_t ColumnIndex = 0; ColumnIndex < sizeKpd.columns; ColumnIndex++)
	{
		bitWrite(OutputBuffer, columnPins[ColumnIndex], 0);
	}
	ioChip->write8(OutputBuffer);
	//Read all the rows.
	//If any are grounded, then we know a switch is active.
	InputBuffer = ioChip->read8();
	bool AllInactive = true;
	for (uint8_t RowIndex = 0; RowIndex < sizeKpd.rows; RowIndex++)
	{
		if ( bitRead(InputBuffer, rowPins[RowIndex]) == false )
		{
			AllInactive = false;
			break;
		}
	}
	//If nothing active, reset the bitMap to all off and return.
	if (AllInactive)
	{
		for (uint8_t ColumnIndex=0; ColumnIndex<sizeKpd.columns; ColumnIndex++)
		{
			for (uint8_t RowIndex=0; RowIndex<sizeKpd.rows; RowIndex++)
			{
				bitWrite(bitMap[RowIndex], ColumnIndex, 0);
			}
		}
		//Turn off output.
		OutputBuffer = 0xFF;
		ioChip->write8(OutputBuffer);
		return;
	}
	//At least one button is active. So read one column at a time.
	OutputBuffer = 0xFF;
	ioChip->write8(OutputBuffer);
	//Test each column independently and write results to bitMap.
	for (uint8_t ColumnIndex=0; ColumnIndex<sizeKpd.columns; ColumnIndex++)
	{
		// Begin column pulse output.
		pin_write(columnPins[ColumnIndex], LOW);
		InputBuffer = ioChip->read8();
		for (uint8_t RowIndex=0; RowIndex<sizeKpd.rows; RowIndex++)
		{
			//If the pin is low, then the switch is active.
			bitWrite(bitMap[RowIndex], ColumnIndex, bitread(InputBuffer[rowPins[RowIndex]]) == false);
		}
		// Set pin to output high. Effectively ends column pulse.
		pin_write(columnPins[ColumnIndex],HIGH);
	}
}

