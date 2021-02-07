#pragma once
#include "Register.h"

void setBit(Register& reg, const uint8_t mask);
void setBitAt(Register& reg, uint8_t pos);

void clearBit(Register& reg, const uint8_t mask);
void clearBitAt(Register& reg, uint8_t pos);

bool checkBit(Register& reg, const uint8_t mask);
bool checkBitAt(Register& reg, uint8_t pos);

void setBitValue(Register& reg, uint8_t pos, bool val);
