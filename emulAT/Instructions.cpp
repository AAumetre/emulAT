#include "ATMega328p.h"

void ATMega328p::ADD(void) {
	// Store the result of the operation
	Register R = _registers[_instruction.dest] + _registers[_instruction.src];
	// Set Half Carry flag (h)
	bool h = (checkBitAt(_instruction.dest, 3) && checkBitAt(_instruction.src, 3)) ||
		(checkBitAt(_instruction.src, 3) && ~checkBitAt(R, 3)) ||
		(~checkBitAt(R, 3) && checkBitAt(_instruction.dest, 3));
	setBitValue(SREG, StatusRegister::HalfCarry, h);
	// Set Two Complement flag (v)
	bool v = (checkBitAt(_instruction.dest, 7) && checkBitAt(_instruction.src, 7) && ~checkBitAt(R, 7)) ||
		(~checkBitAt(_instruction.dest, 7) && ~checkBitAt(_instruction.src, 7) && checkBitAt(R, 7));
	setBitValue(SREG, StatusRegister::TwoCpl, v);
	// Set Negative flasg (n)
	bool n = checkBitAt(R, 7);
	setBitValue(SREG, StatusRegister::Negative, n);
	// Set Sign flag (s)
	bool s = n ^ v;
	setBitValue(SREG, StatusRegister::Sign, s);
	// Set Zero flag (z)
	bool z = (R == 0);
	setBitValue(SREG, StatusRegister::Zero, z);
	// Set Carry flag (c)
	bool c = (checkBitAt(_instruction.dest, 7) && checkBitAt(_instruction.src, 7)) ||
		(checkBitAt(_instruction.src, 7) && ~checkBitAt(R, 7)) ||
		(~checkBitAt(R, 7) && checkBitAt(_instruction.dest, 7));
	setBitValue(SREG, StatusRegister::Carry, c);
	// Write the result
	_registers[_instruction.dest] = R;
	// Increment the Program Counter
	PC++;
}