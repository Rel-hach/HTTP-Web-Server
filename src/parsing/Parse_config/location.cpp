#include "../../../inc/location.hpp"

location::location()
{
	std::cout << "location constructor called" << std::endl;
}

location::location(const location &location)
{
	std::cout << "location copy constructor called" << std::endl;
}

location &location::operator=(const location &location)
{
	std::cout << "location assigned called" << std::endl;
	return *this;
}

location::~location()
{
	std::cout << "location destructor called" << std::endl;
}
