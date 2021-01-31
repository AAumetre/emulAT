#include <iostream>

#include "Memory.h"

template <typename S>
Memory<S>::Memory(size_t size_) : _size(size_), END(size_) {
	_storage.resize(_size);
	reserveAndZeroOut();
}

template <typename S>
Memory<S>::Memory(size_t size_, std::string name_) : Memory(size_){
	_name = name_;

	std::cout << _name << " initialized with " << _size <<
		" lines, of " << sizeof(S) << " bytes each, totalling "
		<< _size*sizeof(S) << " bytes." << std::endl;
}

template <typename S>
void Memory<S>::reserveAndZeroOut() {
	S zero = 0;
	// Reserve and initialize the space
	for (size_t i = 0; i < _size; ++i) {
		_storage.push_back(zero);
	}
}

template <typename S>
void Memory<S>::zeroOut() {
	S zero = 0;
	// Initialize the space
	for (size_t i = 0; i < _size; ++i) {
		_storage.at(i) = zero;
	}
}

template <typename S>
S Memory<S>::readLineAt(size_t address_) {
	if (address_ < _size) {
		return _storage.at(address_);
	}
	else {
		std::cout << "Invalid read access in "<< _name << " at address: " << address_ << std::endl;
		return S(0);
	}
}

template <typename S>
S Memory<S>::readLineAt(std::bitset<8*sizeof(S)> address_) {
	if (address_.to_ulong() < _size) {
		return _storage.at(address_.to_ulong());
	}
	else {
		std::cout << "Invalid read access in " << _name << " at address: " << address_ << std::endl;
		return S(0);
	}
}

template <typename S>
bool Memory<S>::writeLineAt(size_t address_, S val_) {
	if (address_ < _size) {
		_storage.at(address_) = val_;
		return true;
	}
	else {
		std::cout << "Invalid write access in " << _name << " at address: " << address_ << std::endl;
		return false;
	}
}

template <typename S>
bool Memory<S>::writeBitsAt(size_t address_, S val_, S mask_) {
	if (address_ < _size) {
		S prev = readLineAt(address_);
		_storage.at(address_) = (prev & !mask_) & val_;
		return true;
	}
	else {
		std::cout << "Invalid write access in " << _name << " at address: " << address_ << std::endl;
		return false;
	}
}