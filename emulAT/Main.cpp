#include <iostream>

#include "ATMega328p.h"

int main(int argc, char** argv) {

	ATMega328p uC;

	uC._flash.writeLineAt(0, 0xCAFE);
	std::cout << std::hex << uC._flash.readLineAt(0) << std::endl;

	uC.SREG[StatusRegister::N] = true;
	std::cout << "SREG Negative flag is " << (uC.SREG[StatusRegister::Negative] ? "true" : "false") << std::endl;
	uC.SREG[StatusRegister::N] = false;
	std::cout << "SREG Negative flag is " << (uC.SREG[StatusRegister::Negative] ? "true" : "false") << std::endl;

	uC._isVerbose = true;
	// Write a dummy program
	uC._registers[0] = 0x0F;
	uC._registers[1] = 0xF0;
	uC._flash.writeLineAt(0, 0b0000110000000001);


	int nClocks = 2;
	for (int i = 0; i < nClocks; ++i) {
		uC.fetch();
		uC.decode();
		// uC.execute();
	}
	


	return 0;
}