#include <iostream>

#include "ATMega328p.h"

void ATMega328p::handleUnknownInstruction() {
	std::cout << "Unknown instruction read: " << IR << std::endl;
	_instruction.callback = std::bind(&ATMega328p::unknownInstruction, this);
}

void ATMega328p::decode(void) {

	_instruction.name = "unknown";
	// Cut-down the instruction in 4 bits chunks
	uint8_t splitInstruction[4];
	for (int i = 0; i < 4; ++i) {
		splitInstruction[3-i] = (IR.to_ulong() & (0b1111 << i * 4)) >> i * 4;
	}
	// Big switch
	switch (splitInstruction[0]) {
	case 0b0000:
		switch ((splitInstruction[1] & 0b1100) >> 2) { // Next two bits
		case 0b11: // ADD - Add without carry
			_instruction.op = 0b000011;
			_instruction.d = (((splitInstruction[1] & 0b1) << 4) | splitInstruction[2]);
			_instruction.r = (((splitInstruction[1] & 0b10) << 3) | splitInstruction[3]);
			_instruction.callback = std::bind(&ATMega328p::ADD, this);
			_instruction.name = "ADD";
			break;
			break;
		}
		break;
	case 0b0001:
		switch ((splitInstruction[1] & 0b1100) >> 2) { // Next two bits
		case 0b11: // ADC - Add with carry
			_instruction.op = 0b000111;
			_instruction.d = (((splitInstruction[1] & 0b1) << 4) | splitInstruction[2]);
			_instruction.r = (((splitInstruction[1] & 0b10) << 3) | splitInstruction[3]);
			_instruction.callback = std::bind(&ATMega328p::ADC, this);
			_instruction.name = "ADC";
			break;
			break;
		}
		break;
	case 0b0010:
		switch ((splitInstruction[1] & 0b1100) >> 2) {
		case 0b11:
			break;
			break;
		}
		break;
	case 0b1001:
		switch ((splitInstruction[1] & 0b1100) >> 2) { // Next two bits
		case 0b01:

			switch (splitInstruction[1] & 0b0011) { // Next two bits
			case 0b10: // ADIW - Add Immediate to Word
				_instruction.op = 0b10010110;
				_instruction.d = ((splitInstruction[2] & 0b0011) << 4);
				_instruction.K = ((splitInstruction[2] & 0b1100) << 4) | splitInstruction[3];
				_instruction.callback = std::bind(&ATMega328p::ADIW, this);
				_instruction.name = "ADIW";
				break;
				break;
			}
			break;
		}

		
	}

	if (_instruction.name == "unknown") {
		handleUnknownInstruction();
	}

	if (_isVerbose) {
		std::cout << "Instruction " << _instruction.name << ", from r" << (int)_instruction.r <<
			" to r" << (int)_instruction.d << "." << std::endl;
	}
} // end decode()