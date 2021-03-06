#pragma once
#include <bitset>
#include <string>
#include <functional>

using Register = uint8_t;
using RegisterN = int8_t;
using WideRegister = uint16_t;
using ProgramCounter = uint16_t;
using Instruction = std::bitset<16>;
using StackPointer = uint16_t;

class StatusRegister {
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
	static const uint8_t C = 0b00000000;
	static const uint8_t Z = 0b00000010;
	static const uint8_t N = 0b00000100;
	static const uint8_t V = 0b00001000;
	static const uint8_t S = 0b00010000;
	static const uint8_t H = 0b00100000;
	static const uint8_t T = 0b01000000;
	static const uint8_t I = 0b10000000;
};


struct DecodedInstruction {
	uint8_t op;
	uint8_t d; // Destination
	uint8_t r; // Source
	uint8_t K; // Const data
	int16_t k; // Const address, can be negative
	std::string name;
	std::string description;
	std::function<void(void)> callback;
};