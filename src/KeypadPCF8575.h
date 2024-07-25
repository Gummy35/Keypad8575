/*
||
|| @file Keypad.h
|| @version 3.1
|| @author Mark Stanley, Alexander Brevig
|| @contact mstanley@technologist.com, alexanderbrevig@gmail.com
||
|| @description
|| | This library provides a simple interface for using matrix
|| | keypads. It supports multiple keypresses while maintaining
|| | backwards compatibility with the old single key library.
|| | It also supports user selectable pins and definable keymaps.
|| | This derived class specifically uses the PCF8574 to control
|| | the keypad over I2C.
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/

#ifndef PCF8575KEYPAD_H
#define PCF8575KEYPAD_H

#include <Keypad.h>
#include <PCF8575.h>
#include <Wire.h>

#define MAX_LIST_SIZE 16

class KeypadPCF8575 : public Keypad
{
	public:
		KeypadPCF8575(PCF8575 *ioChipToSet, char *userKeymap, byte *row, byte *col, byte numRows, byte numCols);
		void pin_mode(byte pinNum, byte mode);
		void pin_write(byte pinNum, boolean level);
		int pin_read(byte pinNum);
		virtual void scanKeys();
		virtual bool getKeys();
		void addChangesEventListener(void (*listener)(uint8_t[], uint8_t[], uint8_t, uint8_t));
		void enableInterrupt();
		PCF8575 *ioChip;
		uint8_t Pressed[MAX_LIST_SIZE];
		uint8_t Released[MAX_LIST_SIZE];
		uint previousBitmap[MAPSIZE];
	private:
		uint16_t OutputBuffer;
		uint16_t InputBuffer;
		uint16_t InputCheckMask;
		void (*KeypadPCF8575EventListener)(uint8_t[], uint8_t[], uint8_t, uint8_t);

};

#endif
