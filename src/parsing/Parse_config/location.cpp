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

location::~location()
{
}
