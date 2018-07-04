#pragma once

#include <stdexcept>

class ApiAccessException : public std::runtime_error {
public:
	ApiAccessException(std::string mess) : std::runtime_error(mess) {}
};
