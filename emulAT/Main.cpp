#include <iostream>
#include <iomanip>

#include "BitUtility.h"
#include "ATMega328p.h"

void displayRegisters(ATMega328p& uC, size_t n = 8) {
	// Display usual registers
	for (size_t i = 0; i < n; ++i) {
		std::cout << "Reg" << i << " = 0x" << std::hex << std::setfill('0') << std::setw(2) << (int)uC._registers[i] << "\t";
		if ((i+1) % 4 == 0) {
			std::cout << std::endl;
		}
	}
	// Display Status Register
	std::cout << "I  T  H  S  V  N  Z  C" << std::endl;
	for (int i = 0; i < 8; ++i) {
		std::cout << checkBitAt(uC.SREG, i) << "  ";
	}
	std::cout << std::endl;
}

int main(int argc, char** argv) {

	ATMega328p uC;
	uC._isVerbose = true;
	// Write a dummy program
	uC._registers[0] = 0x0F; // Manually write 0x0F in reg0
	uC._registers[1] = 0xF0; // Manually write 0xF0 in reg1
	uC._flash.writeLineAt(0, 0b0000110000000001); // ADD reg0 = reg0 + reg1
	uC._flash.writeLineAt(1, 0); // Nothing

	std::cout << std::endl;
	std::cout << "------------------------------------------------------------------------------" << std::endl;
	std::cout << "    Launching the execution of a program, starting @ 0x0000 0000 0000 0000" << std::endl;
	std::cout << "------------------------------------------------------------------------------" << std::endl;
	int nClocks = 2;
	for (int i = 0; i < nClocks; ++i) {
		displayRegisters(uC);
		uC.fetch();
		uC.decode();
		uC.execute();
		std::cout << "-------------------------------------------" << std::endl;
	}
	


	return 0;
}