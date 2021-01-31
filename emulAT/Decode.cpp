#include <iostream>

#include "ATMega328p.h"

void ATMega328p::decode(void) {
	// Cut-down the instruction in 4 bits chunks
	uint8_t splitInstruction[4];
	for (int i = 3; i > 0; --i) {
		splitInstruction[i] = (IR.to_ulong() & (0b1111 << i*4)) >> i*4;
	}
	// Big switch
	switch (splitInstruction[3]) {
	case 0b0000:
		switch ((splitInstruction[2] & 0b1100) >> 2) {
		case 0b11: // ADD - Add without carry
			_instruction.op = 0b000111;
			_instruction.dest = (((splitInstruction[2] & 0b1) << 5) | splitInstruction[1]);
			_instruction.src =	(((splitInstruction[2] & 0b10) << 4) | splitInstruction[0] );
			if (_isVerbose) {
				std::cout << "Instruction ADD, from r" << (int)_instruction.src << 
					" to r" << (int)_instruction.dest << "." << std::endl;
			}
		default:
		break;
		}
	break;
	default :
		std::cout << "Unknown instruction received: " << IR << std::endl;
	}
}