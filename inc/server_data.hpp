#ifndef SERVER_DATA_HPP
# define SERVER_DATA_HPP
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "location.hpp"

enum e_key{
	SERVER,
	LOCATION,
	ERROR_PAGE,
	UNKNOWN
};

class server_data
{
public:
	server_data();
	server_data(server_data&);
	server_data(const server_data &);
	server_data&operator=(const server_data&);
	void set_keys();
	void print();
	~server_data();
 
	std::string host;
	int port;
	std::vector<std::string> server_names;
	std::string root;
	std::string index;
	std::string autoindex;
	std::string upload_path;
	std::string error_page;
	std::vector<std::string> allow_methods;
	std::vector<std::string> cgi_extensions;
	std::string client_max_body_size;
	std::vector<location> locations;
	std::map<int, std::string> error_pages;
	// vector location *location;
protected:
private:

};
#endif
