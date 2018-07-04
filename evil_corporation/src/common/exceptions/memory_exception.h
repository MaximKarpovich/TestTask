#pragma once

#include <stdexcept>

class MemoryException : public std::runtime_error {
public:
	MemoryException(std::string mess) : std::runtime_error(mess) {}
};