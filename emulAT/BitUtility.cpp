#include "BitUtility.h"

void setBit(Register& reg, const uint8_t mask){
	reg |= (reg & mask);
}
void setBitAt(Register& reg, uint8_t pos){
	reg |= (reg & (1 << pos));
}

void clearBit(Register& reg, const uint8_t mask){
	reg &= ~mask;
}
void clearBitAt(Register& reg, uint8_t pos){
	reg &= ~(1 << pos);
}

bool checkBit(Register& reg, const uint8_t mask){
	return (reg & mask) > 0;
}
bool checkBitAt(Register& reg, uint8_t pos){
	return (reg >> pos) & 1;
}

void setBitValue(Register& reg, uint8_t pos, bool val) {
	if (val) {
		setBitAt(reg, pos);
	}
	else {
		clearBitAt(reg, pos);
	}
}