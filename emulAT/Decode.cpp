#include <iostream>

#include "ATMega328p.h"

void ATMega328p::handleUnknownInstruction() {
	std::cout << "Unknown instruction read: " << IR << std::endl;
	_instruction.callback = std::bind(&ATMega328p::unknownInstruction, this);
}

void ATMega328p::decode(void) {
	// Set default values for the members
	_instruction.name = "unknown";
	_instruction.description = "no description provided";
	_instruction.d = 0;
	_instruction.r = 0;
	_instruction.K = 0;
	_instruction.k = 0;
	_instruction.callback = nullptr;
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
			_instruction.callback = std::bind(&ATMega328p::ADD, this);
			break;
			break;
		case 0b00:
			if (splitInstruction[0] == 0 && splitInstruction[1] == 0 && splitInstruction[2] == 0 && splitInstruction[3] == 0) {
				_instruction.name = "NOP";
				_instruction.description = "No Operation";
				_instruction.op = 0b0000'0000'0000'0000;
				_instruction.callback = std::bind(&ATMega328p::NOP, this);
				break;
			}
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
			_instruction.d = 0b10000 * (splitInstruction[1] & 0b0001) + splitInstruction[2];
			_instruction.r = 0b10000 * (splitInstruction[1] & 0b0010) + splitInstruction[3];
			_instruction.callback = std::bind(&ATMega328p::MOV, this);
			break;
			break;
		}
		break;
	case 0b0011:
		_instruction.name = "CPI";
		_instruction.description = "Compare with Immediate";
		_instruction.op = 0b0011;
		_instruction.d = 16 + splitInstruction[2];
		_instruction.K = (splitInstruction[1] << 4) + splitInstruction[3];
		_instruction.callback = std::bind(&ATMega328p::CPI, this);
		break;
	case 0b1001:
		switch ((splitInstruction[1] & 0b1100) >> 2) { // Next two bits
		case 0b01:
			switch (splitInstruction[1] & 0b0011) { // Next two bits
			case 0b10:
				_instruction.name = "ADIW";
				_instruction.description = "Add Immediate to Word";
				_instruction.op = 0b10010110;
				_instruction.d = 24 + 2 * (splitInstruction[2] & 0b0011);
				_instruction.K = ((splitInstruction[2] & 0b1100) << 4) | splitInstruction[3];
				_instruction.callback = std::bind(&ATMega328p::ADIW, this);
				break;
			case 0b01:
				if (splitInstruction[2] == 0b1001 && splitInstruction[3] == 0b1000) {
					_instruction.name = "BREAK";
					_instruction.description = "Break";
					_instruction.op = 0b1001'0101'1001'1000;
					_instruction.callback = std::bind(&ATMega328p::BREAK, this);
					break;
				}
				break;
			}
			break;
		}
		break;
	case 0b1100:
		_instruction.name = "RJMP";
		_instruction.description = "Relative Jump";
		_instruction.op = 0b1100;
		_instruction.k = (splitInstruction[1] << (3 * 4)) + (splitInstruction[2] << (2 * 4)) + (splitInstruction[3] << (1 * 4));
		_instruction.k /= 16;
		_instruction.callback = std::bind(&ATMega328p::RJMP, this);
		break;
	case 0b1111:
		switch ((splitInstruction[1] & 0b1100) >> 2) { // Next two bits
		case 0b00:
			if ((splitInstruction[3] & 0b0111) == 0b0001) {
				_instruction.name = "BREQ";
				_instruction.description = "Branch if Equal";
				_instruction.op = 0b111100001;
				_instruction.k = ((splitInstruction[1] & 0b0011) << 14) + (splitInstruction[2] << 10) + ((splitInstruction[3] & 0b1000) << 6);
				_instruction.k /= 512;
				_instruction.callback = std::bind(&ATMega328p::BREQ, this);
				break;
			}
		case 0b01:
			if ((splitInstruction[3] & 0b0111) == 0b0001) {
				_instruction.name = "BRNE";
				_instruction.description = "Branch if Not Equal";
				_instruction.op = 0b111101001;
				_instruction.k = ((splitInstruction[1] & 0b0011) << 14) + (splitInstruction[2] << 10) + ((splitInstruction[3] & 0b1000) << 6);
				_instruction.k /= 512;
				_instruction.callback = std::bind(&ATMega328p::BRNE, this);
				break;
			}
		case 0b10:
		case 0b11:
		break;
		}
	}

	if (_instruction.name == "unknown") {
		handleUnknownInstruction();
	}

	if (_isVerbose) {
		std::cout << "Instruction " << _instruction.name << ", d:" << (int)_instruction.d <<
			", r:" << (int)_instruction.r << ", K: " << (int)_instruction.K << ", k: " << (int)_instruction.k << ". (" << _instruction.description << ")" << std::endl;
	}
} // end decode()