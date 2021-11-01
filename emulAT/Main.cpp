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
	for (int i = 7; i >= 0; --i) {
		std::cout << checkBitAt(uC.SREG, i) << "  ";
	}
	std::cout << std::endl;
}

int main(int argc, char** argv) {
	// Instanciate the ATMega328p
	ATMega328p uC;
	uC._isVerbose = true;

	// Write a dummy program
	uC._registers[0] = 0x0D; // Manually write 0x01 in reg0
	uC._registers[1] = 0x01; // Manually write 0x01 in reg1
	uC._registers[2] = 0xF1; // Manually write 0x10 in reg2
	uC._registers[3] = 0xF1; // Manually write 0x10 in reg3
	uC._registers[26] = 0xFA; // Manually write 0d250 in reg26
	int n = 0;
	uC._flash.writeLineAt(n++, 0b0000'1100'0000'0001); // ADD reg0 = reg0 + reg1
	uC._flash.writeLineAt(n++, 0b0001'1100'0010'0000); // ADC reg2 = reg2 + reg0
	uC._flash.writeLineAt(n++, 0b0001'1100'0010'0001); // ADC reg2 = reg2 + reg1
	uC._flash.writeLineAt(n++, 0b0001'1100'0010'0011); // ADC reg2 = reg2 + reg3
	uC._flash.writeLineAt(n++, 0b0001'1100'0010'0011); // ADC reg2 = reg2 + reg3
	uC._flash.writeLineAt(n++, 0b1001'0110'0001'1010); // ADIW reg1 = reg1 + 10
	uC._flash.writeLineAt(n++, 0b0010'1100'0000'0010); // MOV reg0 = reg2
	uC._flash.writeLineAt(n++, 0b0010'1100'0001'0000); // MOV reg1 = reg0
	uC._flash.writeLineAt(n++, 0); // Nothing, will make the emulator stop


	std::cout << std::endl;
	std::cout << "------------------------------------------------------------------------------" << std::endl;
	std::cout << "    Launching the execution of a program, starting @ 0x0000 0000 0000 0000" << std::endl;
	std::cout << "------------------------------------------------------------------------------" << std::endl;
	int nClocks = n;
	for (int i = 0; i < nClocks; ++i) {
		displayRegisters(uC, 32);
		uC.fetch();
		uC.decode();
		uC.execute();
		std::cout << "-------------------------------------------" << std::endl;
	}
	



	return 0;
}