#include "Validation.hpp"

Validation::Validation()
{
	std::cout << "Validation constructor called" << std::endl;
}

Validation::Validation(const Validation &validation)
{
	std::cout << "Validation copy constructor called" << std::endl;
}

Validation &Validation::operator=(const Validation &validation)
{
	std::cout << "Validation assigned called" << std::endl;
	return *this;
}

Validation::~Validation()
{
	std::cout << "Validation destructor called" << std::endl;
}
