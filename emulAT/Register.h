#pragma once

#include <bitset>

using Register = std::bitset<8>;

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