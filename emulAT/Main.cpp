#include <iostream>
#include <iomanip>

#include "BitUtility.h"
#include "ATMega328p.h"

void displayRegisters(ATMega328p& uC, size_t cycles, size_t n = 8) {
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
	// Display the program counter value
	std::cout << "=== PC: 0x" << std::hex << std::setfill('0') << std::setw(4) << (int)uC.PC << " === ";
	std::cout << "=== Cycles: " << cycles << " === " << std::endl;
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
	uC._registers[17] = 0x01; // Manually write 0x01 in reg17
	uC._registers[26] = 0xFA; // Manually write 0d250 in reg26

	int n = 0;
	uC._flash.writeLineAt(n++, 0b0000'1100'0000'0001);	// 0: ADD reg0 = reg0 + reg1
	uC._flash.writeLineAt(n++, 0b0001'1100'0010'0000);	// 1: ADC reg2 = reg2 + reg0
	uC._flash.writeLineAt(n++, 0b0001'1100'0010'0001);	// 2: ADC reg2 = reg2 + reg1
	uC._flash.writeLineAt(n++, 0b0001'1100'0010'0011);	// 3: ADC reg2 = reg2 + reg3
	uC._flash.writeLineAt(n++, 0b0001'1100'0010'0011);	// 4: ADC reg2 = reg2 + reg3
	uC._flash.writeLineAt(n++, 0b1001'0110'0001'1010);	// 5: ADIW reg1 = reg1 + 10
	uC._flash.writeLineAt(n++, 0b0010'1100'0000'0010);	// 6: MOV reg0 = reg2
	uC._flash.writeLineAt(n++, 0b0010'1100'0001'0000);	// 7: MOV reg1 = reg0
	uC._flash.writeLineAt(n++, 0b1100'0000'0000'0011);	// 8: RJMP PC = PC + 3 + 1 = 12
	uC._flash.writeLineAt(n++, 0);						// 9: NOP
	uC._flash.writeLineAt(n++, 0b1100'0000'0000'0110);	// 10: RJMP PC = PC + 6 + 1 = 17
	uC._flash.writeLineAt(n++, 0);					    // 11: NOP
	uC._flash.writeLineAt(n++, 0b0000'1100'0000'0001);	// 12: ADD reg0 = reg0 + reg1
	uC._flash.writeLineAt(n++, 0b1100'1111'1111'1100);	// 13: RJMP PC = PC - 4 + 1 = 10
	uC._flash.writeLineAt(n++, 0);						// 14: NOP
	uC._flash.writeLineAt(n++, 0);						// 15: NOP
	uC._flash.writeLineAt(n++, 0b1001'0101'1001'1000);	// 16: BREAK - current implementation stops the emulator
	uC._flash.writeLineAt(n++, 0b1111'0100'0000'1001);	// 17: BRNE at PC = PC + 1 + 1 = 19
	uC._flash.writeLineAt(n++, 0b1001'0101'1001'1000);	// 18: BREAK - current implementation stops the emulator
	uC._flash.writeLineAt(n++, 0b0011'0000'0000'0100);	// 19: CPI reg16-4
	uC._flash.writeLineAt(n++, 0b1111'0000'0001'0001);	// 20: BREQ at PC = PC + 2 + 1 = 23
	uC._flash.writeLineAt(n++, 0b0000'1111'0000'0001);	// 21: ADD reg16 = reg16 + reg17 (1)
	uC._flash.writeLineAt(n++, 0b1100'1111'1111'1100);	// 22: RJMP PC = PC - 4 + 1 = 19
	uC._flash.writeLineAt(n++, 0b1001'0101'1001'1000);	// 23: BREAK - current implementation stops the emulator


	std::cout << std::endl;
	std::cout << "------------------------------------------------------------------------------" << std::endl;
	std::cout << "    Launching the execution of a program, starting @ 0x0000 0000 0000 0000" << std::endl;
	std::cout << "------------------------------------------------------------------------------" << std::endl;
	int nClocks = n+10;
	for (int i = 0; i < nClocks; ++i) {
		displayRegisters(uC, i, 32);
		uC.fetch();
		uC.decode();
		uC.execute();
		std::cout << "-------------------------------------------" << std::endl;
	}
	



	return 0;
}