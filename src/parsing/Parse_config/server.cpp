#include "../../../inc/server.hpp"

server::server()
{
	std::cout << "server constructor called" << std::endl;
}

server::server(const server &server)
{
	std::cout << "server copy constructor called" << std::endl;
}

server &server::operator=(const server &server)
{
	std::cout << "server assigned called" << std::endl;
	return *this;
}

server::~server()
{
	std::cout << "server destructor called" << std::endl;
}
