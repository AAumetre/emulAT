#include <iostream>
#include <iomanip>

#include "ATMega328p.h"

ATMega328p::ATMega328p() : 
	_flash(_flash_lines), _eeprom(_eeprom_lines), _ram(_ram_lines), _isVerbose(false){
	reset();
}

void ATMega328p::reset(void) {
	// Reset the Stack Pointer
	SP = 0;

	// Reset the Program Counter
	_ram.writeLineAt(_ram.END,		0);
	_ram.writeLineAt(_ram.END-1,	0);
	PC = 0;
}

void ATMega328p::fetch(void) {
	// Read the flash at the address pointed to by the PC register
	IR = _flash.readLineAt(PC);
	std::cout << "Fetching instruction at address: 0x" << std::hex << std::setfill('0') << std::setw(8) << (int)PC  << ": " << IR << std::endl;
}

void ATMega328p::execute(void) {
	if (_instruction.callback ) {
		_instruction.callback();
	}
	else {
		std::cout << "!!!= UNKNOWN CALLBACK, IGNORING =!!!" << std::endl;
	}
}

void ATMega328p::unknownInstruction(void) {
	std::cout << "!!!= UNKNOWN INSTRUCTION RECEIVED, ABORTING =!!!" << std::endl;
	exit(1);
}

void ATMega328p::stop(void) {
	std::cout << "=== BREAK INSTRUCTION, USED AS STOP CONDITION ===" << std::endl;
	exit(1);
}
