#include "../../../inc/server.hpp"
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <sstream>

void trim(std::string &line)
{
	if (line.empty())
		return;
	size_t start = line.find_first_not_of(" \t");
	size_t end = line.find_last_not_of(" \t");
	line = line.substr(start, end - start + 1);
}

void fill_server(server &server, std::string &line){
	std::string key;
	std::string value;
	char eq;
	std::stringstream ss(line);
	ss >> key;
	ss >> eq;
	std::getline(ss, value);

	std::cout << "keyuuu = " << key << std::endl;
	std::cout << "valueuuu = " << value << std::endl;

	if (key == "host")
		server.host = value;
	else if (key == "port")
		server.port = std::stoi(value);
	else if (key == "server_name")
		server.server_names.push_back(value);
	else if (key == "root")
		server.root = value;
	else if (key == "index")
		server.index = value;
	else if (key == "autoindex")
		server.autoindex = value;
	else if (key == "upload_path")
		server.upload_path = value;
	else if (key == "error_page")
		server.error_page = value;
	else if (key == "methods")
		server.methods.push_back(value);
	else if (key == "cgi_extensions")
		server.cgi_extensions.push_back(value);
	else if (key == "client_max_body_size")
		server.client_max_body_size = value;
	else 
		throw std::invalid_argument("Error: invalid server key");
}

void fill_location(location &location, std::string &line){
	std::string key;
	std::string value;
	char eq;
	std::stringstream ss(line);
	ss >> key;
	ss >> eq;
	std::getline(ss, value);
	trim(value);
	std::cout << "key = " << key << std::endl;
	std::cout << "value = " << value << std::endl;
	if (key == "uri")
	{
		location.uri = value;
		return;
	}
	else if (key == "allow_methods")
	{
		location.allow_methods.push_back(value);
		return;
	}
	else if (key == "cgi_path")
	{
		location.cgi_path = value;
		return;
	}
	else if (key == "autoindex")
	{
		location.autoindex = value;
		return;
	}
	else if (key == "return_code")
	{
		location.return_code = value;
		return;
	}
	else
		throw std::invalid_argument("Error: invalid location key");
}

void fill_error_page(std::map<int , std::string> &error_page, std::string &line){
	std::string k;
	k = line.substr(0, line.find_first_of(" "));
	int key;
	std::string value;
	char eq;
	std::stringstream ss(line);
	ss >> key;
	ss >> eq;
	std::getline(ss, value);
	trim(value);
	std::cout << "key = " << key << std::endl;
	std::cout << "value = " << value << std::endl;
	if (key < 400 || key > 599 || k.find_first_not_of("0123456789") != std::string::npos || k.empty())
		throw std::invalid_argument("Error: invalid error_page key");
	else if (value.empty() || value[0] != '"' || value[value.size() - 1] != '"')
		throw std::invalid_argument("Error: invalid error_page value");
	error_page[key] = value;
}

void parse_server(std::string config_file, std::vector<server> &servers)
{
	std::ifstream file;
	std::string line;
	std::pair<std::string, std::string> key_value;
	e_key flag = UNKNOWN;

	file.open(config_file);
	if (!file.is_open())
		throw std::invalid_argument("Error: cannot open config file");
	while (std::getline(file, line))
	{
		trim(line);
		if (line[0] == '#' || line.empty())
			continue;
		else if (line == "[[server]]")
		{
			std::cout << "\033[4;33mfound server\033[0m" << std::endl;
			servers.push_back(server());
			flag = SERVER;
		}
		else if (line == "[[server.location]]")
		{
			std::cout << "\033[4;35mfound location\033[0m" << std::endl;
			servers.back().locations.push_back(location());
			flag = LOCATION;
		}
		else if (line == "[[server.error_page]]")
		{
			std::cout << "\033[4;35mfound error_page\033[0m" << std::endl;
			flag = ERROR_PAGE;
		}
		else if (flag == UNKNOWN)
			throw std::invalid_argument("Error: invalid key");
		else 
		{
			if (flag == SERVER)
				fill_server(servers.back(), line);
			else if (flag == LOCATION && line != "[[server.location]]")
				fill_location(servers.back().locations.back(), line);
			else if (flag == ERROR_PAGE && line != "[[server.error_page]]")
				fill_error_page(servers.back().error_pages, line);
			servers.back().print();
			std::cout << "uplooooood" << servers.back().upload_path << std::endl;
		}
	}
	servers[0].server_names.push_back("localhost");
	std::cout << "parse_server : servers[0].server_names = " << servers[0].server_names.back() << std::endl;
}


int main (int argc, char *argv[])
{
	std::vector<server> servers;
	parse_server(argv[1], servers);
	std::cout << "servers[1].upload_path = " << servers[0].port << std::endl;
//	std::cout << "servers[0].server_names = " << servers[0].server_names[0] << std::endl;
	return 0;
}
