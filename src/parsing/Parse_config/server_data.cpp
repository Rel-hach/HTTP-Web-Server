#include "../../../inc/server_data.hpp"
#include <cstddef>

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
	this->root = server.root;
	this->index = server.index;
	this->autoindex = server.autoindex;
	this->upload_path = server.upload_path;
	this->error_page = server.error_page;
	this->allow_methods = server.allow_methods;
	this->cgi_extensions = server.cgi_extensions;
	this->client_max_body_size = server.client_max_body_size;
	this->locations = server.locations;
	this->error_pages = server.error_pages;
	return *this;
}

void server_data::print(){
	std::cout << "host = " << host << std::endl;
	std::cout << "port = " << port << std::endl;
	std::cout << "root = " << root << std::endl;
	std::cout << "index = " << index << std::endl;
	size_t i = 0;
	while (i < server_names.size())
	{
		std::cout << "server_name[" << i << "] = " << server_names[i] << std::endl;
		i++;
	}
	i = 0;
	while (i < allow_methods.size())
	{
		std::cout << "allow_methods[" << i << "] = " << allow_methods[i] << std::endl;
		i++;
	}
	i = 0;
	while (i < cgi_extensions.size())
	{
		std::cout << "cgi_extensions[" << i << "] = " << cgi_extensions[i] << std::endl;
		i++;
	}
	std::cout << "autoindex = " << autoindex << std::endl;
	std::cout << "upload_path = " << upload_path << std::endl;
	std::cout << "error_page = " << error_page << std::endl;
}

server_data::~server_data()
{
}
