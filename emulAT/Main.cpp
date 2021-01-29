#include <iostream>

#include "ATMega328p.h"

int main(int argc, char** argv) {

	ATMega328p* uC = new ATMega328p();

	uC->_flash.writeLineAt(0, 0xCAFE);

	std::cout << std::hex << uC->_flash.readLineAt(0) << std::endl;

	delete uC;
	return 0;
}