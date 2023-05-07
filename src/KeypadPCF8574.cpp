#include "KeypadPCF8574.h"

KeypadPCF8574::KeypadPCF8574(*PCF8574 ioChipToSet, char *userKeymap, byte *row, byte *col, byte numRows, byte numCols) : KeyPad(userKeymap, row, col, numRows, numCols)
{
	ioChip = ioChipToSet;
}

void KeypadPCF8574::pin_mode(byte pinNum, byte mode)
{
	ioChip->pinMode(pinNum, mode);
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
	// Re-intialize the row pins. Allows sharing these pins with other hardware.
	for (byte r=0; r<sizeKpd.rows; r++) {
		pin_mode(rowPins[r],INPUT_PULLUP);
	}

	// bitMap stores ALL the keys that are being pressed.
	for (byte c=0; c<sizeKpd.columns; c++) {
		pin_mode(columnPins[c],OUTPUT);
		pin_write(columnPins[c], LOW);	// Begin column pulse output.
		for (byte r=0; r<sizeKpd.rows; r++) {
			bitWrite(bitMap[r], c, !pin_read(rowPins[r]));  // keypress is active low so invert to high.
		}
		// Set pin to high impedance input. Effectively ends column pulse.
		pin_write(columnPins[c],HIGH);
		pin_mode(columnPins[c],INPUT);
	}
}

