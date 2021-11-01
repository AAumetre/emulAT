#pragma once
#include "Register.h"

template <typename T>
void setBit(T& reg, const uint8_t mask) {
	reg |= (reg & mask);
}
template <typename T>
void setBitAt(T& reg, uint8_t pos) {
	reg |= (1 << pos);
}

template <typename T>
void clearBit(T& reg, const uint8_t mask) {
	reg &= ~mask;
}
template <typename T>
void clearBitAt(T& reg, uint8_t pos) {
	reg &= ~(1 << pos);
}

template <typename T>
bool checkBit(T& reg, const uint8_t mask) {
	return (reg & mask) > 0;
}
template <typename T>
bool checkBitAt(T& reg, uint8_t pos) {
	return (reg >> pos) & 1;
}

template <typename T>
void setBitValue(T& reg, uint8_t pos, bool val) {
	if (val) {
		setBitAt(reg, pos);
	}
	else {
		clearBitAt(reg, pos);
	}
}
