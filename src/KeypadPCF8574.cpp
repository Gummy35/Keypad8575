#include "KeypadPCF8574.h"

KeypadPCF8574::KeypadPCF8574(*PCF8574 ioChipToSet, char *userKeymap, byte *row, byte *col, byte numRows, byte numCols) : KeyPad(userKeymap, row, col, numRows, numCols)
{
	ioChip = ioChipToSet;
}

void KeypadPCF8574::pin_mode(byte pinNum, byte mode)
{
	//ioChip->pinMode(pinNum, mode);
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
	//Turn on all columns.
	OutputBuffer = 0;
	for (uint8_t ColumnIndex = 0; ColumnIndex < sizeKpd.columns; ColumnIndex++)
	{
		//pin_mode(columnPins[ColumnIndex],OUTPUT);
		bitWrite(OutputBuffer, columnPins[ColumnIndex], 1);
	}
	ioChip->write8(OutputBuffer);
	//Read all the rows.
	InputBuffer = ioChip->read8();
	bool AllOff = true;
	for (uint8_t RowIndex = 0; RowIndex < sizeKpd.rows; RowIndex++)
	{
		if ( bitRead(InputBuffer, rowPins[RowIndex]) )
		{
			AllOff = false;
			break;
		}
	}
	//If nothing active, reset the bitMap to all off and return.
	if (AllOff)
	{
		for (uint8_t ColumnIndex=0; ColumnIndex<sizeKpd.columns; ColumnIndex++)
		{
			for (uint8_t RowIndex=0; RowIndex<sizeKpd.rows; RowIndex++)
			{
				bitWrite(bitMap[RowIndex], ColumnIndex, 0);
			}
		}
		//Turn off output.
		OutputBuffer = 0;
		ioChip->write8(OutputBuffer);
		//for (uint8_t ColumnIndex=0; ColumnIndex<sizeKpd.columns; ColumnIndex++)
		//{
		//	pin_mode(columnPins[ColumnIndex],INPUT);
		//}
		return;
	}
	//At least one button is active. So turn off all columns.
	OutputBuffer = 0;
	ioChip->write8(OutputBuffer);
	//for (uint8_t ColumnIndex=0; ColumnIndex<sizeKpd.columns; ColumnIndex++)
	//{
	//	pin_mode(columnPins[ColumnIndex],INPUT);
	//}
	//Test each column independently and write results to bitMap.
	for (uint8_t ColumnIndex=0; ColumnIndex<sizeKpd.columns; ColumnIndex++)
	{
		//pin_mode(columnPins[ColumnIndex],OUTPUT);
		pin_write(columnPins[ColumnIndex], HIGH);	// Begin column pulse output.
		InputBuffer = ioChip->read8();
		for (uint8_t RowIndex=0; RowIndex<sizeKpd.rows; RowIndex++)
		{
			bitWrite(bitMap[RowIndex], ColumnIndex, bitread(InputBuffer[rowPins[RowIndex]]));  // keypress is active low so invert to high.
		}
		// Set pin to high impedance input. Effectively ends column pulse.
		pin_write(columnPins[ColumnIndex],LOW);
		//pin_mode(columnPins[ColumnIndex],INPUT);
	}
}

