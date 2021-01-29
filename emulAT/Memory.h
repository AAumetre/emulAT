#pragma once
#include <string>
#include <vector>

template <typename S>
class Memory {
private:
	Memory(size_t size_);
	Memory() {};
public:
	Memory(size_t size_, std::string name_ );

public:
	void reserveAndZeroOut();
	void zeroOut();

public:
	S readLineAt(size_t address_);
	bool writeLineAt(size_t address_, S val_);

private:
	size_t _size;
	std::vector<S> _storage;
	std::string _name;
};



// Specializations
template <typename S>
class Ram : public Memory<S> {
public:
	Ram(size_t size_) : Memory<S>(size_, "SRAM") {
	}
};

template <typename S>
class Eeprom : public Memory<S> {
public:
	Eeprom(size_t size_) : Memory<S>(size_, "EEPROM") {
	}

};

template <typename S>
class Flash : public Memory<S> {
public:
	Flash(size_t size_) : Memory<S>(size_, "Flash"){
	}
};