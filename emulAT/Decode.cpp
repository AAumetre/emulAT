#include <iostream>

#include "ATMega328p.h"

void ATMega328p::handleUnknownInstruction() {
	std::cout << "Unknown instruction read: " << IR << std::endl;
	_instruction.callback = std::bind(&ATMega328p::unknownInstruction, this);
}

void ATMega328p::decode(void) {
	// Cut-down the instruction in 4 bits chunks
	uint8_t splitInstruction[4];
	for (int i = 3; i >= 0; --i) {
		splitInstruction[i] = (IR.to_ulong() & (0b1111 << i*4)) >> i*4;
	}
	// Big switch
	switch (splitInstruction[3]) {
	case 0b0000:
		switch ((splitInstruction[2] & 0b1100) >> 2) {
		case 0b11: // ADD - Add without carry
			_instruction.op = 0b000011;
			_instruction.dest = (((splitInstruction[2] & 0b1) << 4) | splitInstruction[1]);
			_instruction.src =	(((splitInstruction[2] & 0b10) << 3) | splitInstruction[0] );
			_instruction.callback = std::bind(&ATMega328p::ADD, this);
			_instruction.name = "ADD";
			break;
		default:
			handleUnknownInstruction();
		break;
		}
	break;
	case 0b0001:
		switch ((splitInstruction[2] & 0b1100) >> 2) {
		case 0b11: // ADC - Add with carry
			_instruction.op = 0b000111;
			_instruction.name = "ADC";
			break;
		default:
			handleUnknownInstruction();
		break;
		}
	break;
	case 0b0010:
		switch ((splitInstruction[2] & 0b1100) >> 2) {
		case 0b11:
		break;
		default:
			handleUnknownInstruction();
		break;
		}
	break;
	default :
		handleUnknownInstruction();
	}
	if (_isVerbose) {
		std::cout << "Instruction " << _instruction.name << ", from r" << (int)_instruction.src <<
			" to r" << (int)_instruction.dest << "." << std::endl;
	}
}