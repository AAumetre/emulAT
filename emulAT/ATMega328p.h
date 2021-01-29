#pragma once
#include <vector>

#include "Memory.h"
#include "Memory.cpp" // Needed because the implementation of the template class is missing a linking
#include "Register.h"

class ATMega328p {
public:
	ATMega328p();

public:
	static const size_t _flash_lines	= 32 * 1024 * 8 / 16; // lines
	static const size_t _eeprom_lines	= 1 * 1024 * 8 / 8; // lines
	static const size_t _ram_lines		= 2 * 1024 * 8 / 8; // lines

	static const size_t _register_size	= 32; // bytes

public:
	Flash<uint16_t> _flash;		// Blocks of 16 bits
	Eeprom<uint8_t> _eeprom;	// Blocks of 8 bits
	Ram<uint8_t>	_ram;		// Blocks of 8 bits

private:
	StatusRegister SREG;
	/*
	X = R27 R26
	Y = R29 R28
	Z = R31 R30
	*/

	// Stack in RAM, from high addresses to lower
	Register SPH; // Stack Pointer High
	Register SPL; // Stack Pointer Low


	/*SRAM Data Memory
	32 Registers			0x0000 - 0x001F
	64 I/O Registers		0x0020 - 0x005F
	160 Ext I/O Registers	0x0060 - 0x00FF
	Internal SRAM			0x0100 - 0x08FF*/

	Register EEARH; // EEPROM Address Register High
	Register EEARL; // EEPROM Address Register Low
	Register EEDR; // EEPROM Data Register
	Register EECR; // EEPROM Control Register

	Register GPIOR2; // General Purpose I/O Register 2
	Register GPIOR1; // General Purpose I/O Register 1
	Register GPIOR0; // General Purpose I/O Register 0

};