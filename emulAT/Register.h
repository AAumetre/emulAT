#pragma once
#include <bitset>

using Register = std::bitset<8>;
using ProgramCounter = std::bitset<14>;
using Instruction = std::bitset<16>;
using StackPointer = std::bitset<16>;

class StatusRegister : public Register {
public:
	enum {
		Carry = 0,
		Zero,
		Negative,
		TwoCpl,
		Sign,
		HalfCarry,
		BitCpy,
		GlobalInt
	};
	enum {
		C = 0,
		Z,
		N,
		V,
		S,
		H,
		T,
		I
	};
};

struct DecodedInstruction {
	uint8_t op;
	uint8_t dest;
	uint8_t src;
};