#include <iostream>

#include "ATMega328p.h"

ATMega328p::ATMega328p() : 
	_flash(_flash_lines), _eeprom(_eeprom_lines), _ram(_ram_lines){
}
