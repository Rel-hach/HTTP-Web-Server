#include "../../../inc/server.hpp"

server::server()
{
}

server::server(const server &server)
{
}

server &server::operator=(const server &server)
{
	return *this;
}

void server::print(){
	std::cout << "host = " << host << std::endl;
	std::cout << "port = " << port << std::endl;
	std::cout << "root = " << root << std::endl;
	std::cout << "index = " << index << std::endl;
	std::cout << "autoindex = " << autoindex << std::endl;
	std::cout << "upload_path = " << upload_path << std::endl;
	std::cout << "error_page = " << error_page << std::endl;
}

server::~server()
{
}
