#include "ATMega328p.h"

void ATMega328p::ADD(void) {
	// Store the result of the operation
	Register R = _registers[_instruction.d] + _registers[_instruction.r];
	// Set Half Carry flag (h)
	bool h = (checkBitAt(_registers[_instruction.d], 3) && checkBitAt(_registers[_instruction.r], 3)) ||
		(checkBitAt(_registers[_instruction.r], 3) && ~checkBitAt(R, 3)) ||
		(~checkBitAt(R, 3) && checkBitAt(_registers[_instruction.d], 3));
	setBitValue(SREG, StatusRegister::HalfCarry, h);
	// Set Two Complement flag (v)
	bool v = (checkBitAt(_registers[_instruction.d], 7) && checkBitAt(_registers[_instruction.r], 7) && ~checkBitAt(R, 7)) ||
		(~checkBitAt(_registers[_instruction.d], 7) && ~checkBitAt(_registers[_instruction.r], 7) && checkBitAt(R, 7));
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
	bool c = (checkBitAt(_registers[_instruction.d], 7) && checkBitAt(_registers[_instruction.r], 7)) ||
		(checkBitAt(_registers[_instruction.r], 7) && ~checkBitAt(R, 7)) ||
		(~checkBitAt(R, 7) && checkBitAt(_registers[_instruction.d], 7));
	setBitValue(SREG, StatusRegister::Carry, c);
	// Write the result
	_registers[_instruction.d] = R;
	// Increment the Program Counter
	PC++;
}

void ATMega328p::ADC(void) {
	// Store the result of the operation
	Register R = _registers[_instruction.d] + _registers[_instruction.r] + checkBitAt(SREG, StatusRegister::Carry);
	// Set Half Carry flag (h)
	bool h = (checkBitAt(_registers[_instruction.d], 3) && checkBitAt(_registers[_instruction.r], 3)) ||
		(checkBitAt(_registers[_instruction.r], 3) && ~checkBitAt(R, 3)) ||
		(~checkBitAt(R, 3) && checkBitAt(_registers[_instruction.d], 3));
	setBitValue(SREG, StatusRegister::HalfCarry, h);
	// Set Two Complement flag (v)
	bool v = (checkBitAt(_registers[_instruction.d], 7) && checkBitAt(_registers[_instruction.r], 7) && ~checkBitAt(R, 7)) ||
		(~checkBitAt(_registers[_instruction.d], 7) && ~checkBitAt(_registers[_instruction.r], 7) && checkBitAt(R, 7));
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
	bool c = (checkBitAt(_registers[_instruction.d], 7) && checkBitAt(_registers[_instruction.r], 7)) ||
		(checkBitAt(_registers[_instruction.r], 7) && ~checkBitAt(R, 7)) ||
		(~checkBitAt(R, 7) && checkBitAt(_registers[_instruction.d], 7));
	setBitValue(SREG, StatusRegister::Carry, c);
	// Write the result
	_registers[_instruction.d] = R;
	// Increment the Program Counter
	PC++;
}

void ATMega328p::ADIW(void) {
	// Store the result of the operation
	WideRegister wideR = (_registers[_instruction.d + 1] << 8) + _registers[_instruction.d] + _instruction.K;
	Register Rl = wideR & 0x00FF;
	Register Rh = (wideR & 0xFF00) >> 8;
	// Set Two Complement flag (v)
	bool v = ~checkBitAt(Rh, 7) && checkBitAt(wideR, 15);
	setBitValue(SREG, StatusRegister::TwoCpl, v);
	// Set Negative flasg (n)
	bool n = checkBitAt(wideR, 15);
	setBitValue(SREG, StatusRegister::Negative, n);
	// Set Sign flag (s)
	bool s = n ^ v;
	setBitValue(SREG, StatusRegister::Sign, s);
	// Set Zero flag (z)
	bool z = (wideR == 0);
	setBitValue(SREG, StatusRegister::Zero, z);
	// Set Carry flag (c)	
	bool c = ~checkBitAt(wideR, 15) && checkBitAt(Rh, 7);
	setBitValue(SREG, StatusRegister::Carry, c);
	// Write the result
	_registers[_instruction.d] = Rl;
	_registers[_instruction.d + 1] = Rh;
	// Increment the Program Counter
	PC++;
}

void ATMega328p::BREAK(void) {
	std::cout << "=== BREAK INSTRUCTION, USED AS STOP CONDITION ===" << std::endl;
	stop();
}

void ATMega328p::BREQ(void) {
	// Check the Zero status flag
	if (checkBitAt(SREG, StatusRegister::Zero)) {
		PC = PC + _instruction.k + 1;
	}
	else {
		PC++;
	}
}

void ATMega328p::BRNE(void) {
	// Check the Zero status flag
	if (!checkBitAt(SREG, StatusRegister::Zero)) {
		PC = PC + _instruction.k + 1;
	}
	else {
		PC++;
	}
}

void ATMega328p::CPI(void) {
	// Store the result of the operation
	Register Rd = _registers[_instruction.d];
	Register K = _instruction.K;
	RegisterN R = Rd - K;
	// Set Half Carry flag (h)
	bool h = (~checkBitAt(Rd, 3) && checkBitAt(K, 3)) ||
		(checkBitAt(K, 3) && checkBitAt(R, 3)) ||
		(checkBitAt(R, 3) && ~checkBitAt(Rd, 3));
	setBitValue(SREG, StatusRegister::HalfCarry, h);
	// Set Two Complement flag (v)
	bool v = (checkBitAt(Rd, 7) && ~checkBitAt(K, 7) && ~checkBitAt(R, 7)) ||
		(~checkBitAt(Rd, 7) && checkBitAt(K, 7) && checkBitAt(R, 7));
	setBitValue(SREG, StatusRegister::TwoCpl, v);
	// Set Negative flag (n)
	bool n = checkBitAt(R, 7);
	setBitValue(SREG, StatusRegister::Negative, n);
	// Set Sign flag (s)
	bool s = n ^ v;
	setBitValue(SREG, StatusRegister::Sign, s);
	// Set Zero flag (z)
	bool z = (R == 0);
	setBitValue(SREG, StatusRegister::Zero, z);
	// Set Carry flag (c)	
	bool c = (~checkBitAt(Rd, 7) && checkBitAt(K, 7)) ||
		(checkBitAt(K, 7) && checkBitAt(R, 7)) ||
		(checkBitAt(R, 7) && ~checkBitAt(Rd, 7));
	setBitValue(SREG, StatusRegister::Carry, c);
	// Increment the Program Counter
	PC++;
}

void ATMega328p::MOV(void) {
	// Write the result
	_registers[_instruction.d] = _registers[_instruction.r];
	// Increment the Program Counter
	PC++;
}

void ATMega328p::NOP(void) {
	// Increment the Program Counter
	PC++;
}

void ATMega328p::RJMP(void) {
	// Increment the Program Counter
	PC = PC + _instruction.k + 1;
}