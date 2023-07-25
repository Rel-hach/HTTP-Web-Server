#include "../../../inc/location.hpp"

location::location()
{
}

location::location(location &location)
{
	*this = location;
}

location::location(const location &location)
{
	*this = location;
}

location &location::operator=(const location &location)
{
	if (this == &location)
		return *this;
	this->allow_methods = location.allow_methods;
	this->cgi_path = location.cgi_path;
	this->autoindex = location.autoindex;
	this->root = location.root;
	this->upload = location.upload;
	this->cgi_extensions = location.cgi_extensions;
	this->return_code = location.return_code;
	this->allow_methods = location.allow_methods;
	return *this;
}

void location::print_location(){
	std::cout << "allow_methods = ";
	for (size_t i = 0; i < allow_methods.size(); i++)
		std::cout << allow_methods[i] << " ";
	std::cout << std::endl;
	std::cout << "cgi_path = ";
	for (size_t i = 0; i < cgi_path.size(); i++)
		std::cout << cgi_path[i] << " ";
	std::cout << std::endl;
	std::cout << "autoindex = " << autoindex << std::endl;
	std::cout << "root = " << root << std::endl;
	std::cout << "upload = " << upload << std::endl;
	std::cout << "index = ";
	for (size_t i = 0; i < index.size(); i++)
		std::cout << index[i] << " ";
	std::cout << std::endl;
	std::cout << "cgi_extensions = ";
	for (size_t i = 0; i < cgi_extensions.size(); i++)
		std::cout << cgi_extensions[i] << " ";
	std::cout << std::endl;
	std::cout << "return_code = " << return_code << std::endl;
	std::cout << "return_path = " << return_path << std::endl;
}

location::~location()
{
}
