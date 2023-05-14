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

#ifndef PCF8574KEYPAD_H
#define PCF8574KEYPAD_H

#include <Keypad.h>
#include <PCF8574.h>
#include <Wire.h>

class KeypadPCF8574 : public Keypad
{
	public:
		KeypadPCF8574(PCF8574 *ioChipToSet, char *userKeymap, byte *row, byte *col, byte numRows, byte numCols);
		void pin_mode(byte pinNum, byte mode);
		void pin_write(byte pinNum, boolean level);
		int pin_read(byte pinNum);
		void scanKeys();

	private:
		PCF8574 *ioChip;
		uint8_t OutputBuffer;
		uint8_t InputBuffer;
};

#endif

*/
