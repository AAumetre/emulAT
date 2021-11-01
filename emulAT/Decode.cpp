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
		case 0b11:
			_instruction.name = "ADD";
			_instruction.description = "Add without Carry";
			_instruction.op = 0b000011;
			_instruction.d = (((splitInstruction[1] & 0b1) << 4) | splitInstruction[2]);
			_instruction.r = (((splitInstruction[1] & 0b10) << 3) | splitInstruction[3]);
			_instruction.K = 0;
			_instruction.callback = std::bind(&ATMega328p::ADD, this);
			break;
			break;
		}
		break;
	case 0b0001:
		switch ((splitInstruction[1] & 0b1100) >> 2) { // Next two bits
		case 0b11:
			_instruction.name = "ADC";
			_instruction.description = "Add with Carry";
			_instruction.op = 0b000111;
			_instruction.d = (((splitInstruction[1] & 0b1) << 4) | splitInstruction[2]);
			_instruction.r = (((splitInstruction[1] & 0b10) << 3) | splitInstruction[3]);
			_instruction.K = 0;
			_instruction.callback = std::bind(&ATMega328p::ADC, this);
			break;
			break;
		}
		break;
	case 0b0010:
		switch ((splitInstruction[1] & 0b1100) >> 2) {
		case 0b11:
			_instruction.name = "MOV";
			_instruction.description = "Copy Register";
			_instruction.op = 0b00101100;
			_instruction.d = 0b10000*(splitInstruction[1]&0b0001) + splitInstruction[2];
			_instruction.r = 0b10000 * (splitInstruction[1] & 0b0010) + splitInstruction[3];
			_instruction.K = 0;
			_instruction.callback = std::bind(&ATMega328p::MOV, this);
			break;
			break;
		}
		break;
	case 0b1001:
		switch ((splitInstruction[1] & 0b1100) >> 2) { // Next two bits
		case 0b01:

			switch (splitInstruction[1] & 0b0011) { // Next two bits
			case 0b10:
				_instruction.name = "ADIW";
				_instruction.description = "Add Immediate to Word";
				_instruction.op = 0b10010110;
				_instruction.d = 24 + 2*(splitInstruction[2] & 0b0011);
				_instruction.r = 0;
				_instruction.K = ((splitInstruction[2] & 0b1100) << 4) | splitInstruction[3];
				_instruction.callback = std::bind(&ATMega328p::ADIW, this);
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
		std::cout << "Instruction " << _instruction.name << ", d:" << (int)_instruction.d <<
			", r:" << (int)_instruction.r << ", K: " << (int)_instruction.K << ". (" << _instruction.description << ")" << std::endl;
	}
} // end decode()