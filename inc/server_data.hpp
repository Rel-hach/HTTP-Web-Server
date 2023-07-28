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
	std::vector<int> port;
	std::vector<std::string> server_names;
	std::string home;
	std::string client_max_body_size;
	std::map<std::string, location> locations;
	std::map<int, std::string> error_pages;
protected:
private:

};
std::vector<server_data> parse_server(std::string config_file);
#endif
