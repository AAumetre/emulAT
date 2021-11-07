#pragma once
#include <vector>

#include "Memory.h"
#include "Memory.cpp" // Needed because the implementation of the template class is missing a linking
#include "Register.h"
#include "BitUtility.h"

class ATMega328p {
public:
	ATMega328p();

private:
	static const size_t _flash_lines	= 32 * 1024 * 8 / 16; // lines
	static const size_t _eeprom_lines	= 1 * 1024 * 8 / 8; // lines
	static const size_t _ram_lines		= 2 * 1024 * 8 / 8; // lines

	static const size_t _register_size	= 32; // bytes

public:
	void reset(void);
	void decode(void);
	void fetch(void);
	void execute(void);

public:
	bool _isVerbose;
public:
	Flash<uint16_t> _flash;		// Blocks of 16 bits - contains the program
	Eeprom<uint8_t> _eeprom;	// Blocks of 8 bits - contains additional data
	Ram<uint16_t>	_ram;		// Blocks of 8 bits

	Register _registers[_register_size];

public: // Should be private
	Register SREG;
	/*
	X = R27 R26
	Y = R29 R28
	Z = R31 R30
	*/

	// Stack in RAM, from high addresses to lower, the SP points to the top (lowest address)
	StackPointer SP;

	// Program Counter points to the next instruction to be decoded, sotred in SRAM
	// Let's say it's the 14 MSBs of the SRAM, for now
	// So 8 bits MSB and 6 MSB of MSB-1 
	ProgramCounter PC;

	// Instruction Register temporarily stores the decoded instruction
	Instruction IR;
	// Decoded instruction structure
	DecodedInstruction _instruction;


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

private:
	void handleUnknownInstruction();
	void stop();

private:
	void ADD(void);
	void ADC(void);
	void ADIW(void); // d {24,26,28,30}, 0<=K<=63
	void BREAK(void);
	void BREQ(void);
	void BRNE(void);
	void CALL(void);
	void CPI(void);
	void LDI(void);
	void MOV(void);
	void NOP(void);
	void POP(void);
	void PUSH(void);
	void RET(void);
	void RJMP(void);
	void unknownInstruction(void);

};