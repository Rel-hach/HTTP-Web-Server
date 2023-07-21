#include "../../../inc/server_data.hpp"
#include <cstddef>
#include <iostream>

server_data::server_data()
{
}

server_data::server_data(const server_data &server)
{
	*this = server;
}

server_data &server_data::operator=(const server_data &server)
{
	this->host = server.host;
	this->port = server.port;
	this->server_names = server.server_names;
	this->home = server.home;
	this->client_max_body_size = server.client_max_body_size;
	this->locations = server.locations;
	this->error_pages = server.error_pages;
	return *this;
}

void server_data::print(){
	std::cout << "host = " << host << std::endl;
	std::cout << "port = " << port << std::endl;
	std::cout << "home = " << home << std::endl;
	size_t i = 0;
	while (i < server_names.size())
	{
		std::cout << "server_name[" << i << "] = " << server_names[i] << std::endl;
		i++;
	}
	std::cout << "client_max_body_size = " << client_max_body_size << std::endl;
}

server_data::~server_data()
{
}
