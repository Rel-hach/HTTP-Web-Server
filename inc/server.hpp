#ifndef SERVER_HPP
# define SERVER_HPP
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "location.hpp"

class server
{
public:
	server();
	server(server&);
	server(const server &);
	server&operator=(const server&);
	void set_keys();
	~server();
 
	std::string host;
	int port;
	std::vector<std::string> server_names;
	std::string root;
	std::string index;
	std::string autoindex;
	std::string upload_path;
	std::string error_page;
	std::vector<std::string> methods;
	std::vector<std::string> cgi_extensions;
	std::string client_max_body_size;
	std::vector<location> locations;
	std::map<int, std::string> error_pages;
	// vector location *location;
protected:
private:

};
#endif
