#include <vector>
#include <string>
#include <sstream>
#include "../../../inc/server_data.hpp"
#include "../../../inc/location.hpp"
#include "../../../inc/tools.hpp"

void fill_vector(std::vector<std::string> &vec, std::string &values){
	std::string value;
	int i = 0;
	trim(values, " \t[]");
	while (values[i])
	{
		if (values[i] == ',' || values[i + 1] == '\0' || values[i] == '\"')
		{
			if (values[i + 1] == '\0')
				value += values[i];
			trim(value, " \t\"");
			if (!value.empty())
				vec.push_back(value);
			value.clear();
		}
		else
			value += values[i];
		i++;
	}
}

std::pair<std::string, std::string> fill_pair(std::string &line){
	std::string key;
	std::string value;
	char eq;
	std::stringstream ss(line);
	ss >> key;
	ss >> eq;
	std::getline(ss, value);
	check_value_key(value, key);
	trim(value, " \t\"");	
	return (std::make_pair(key, value));
}

void check_int(std::string &value){
	if (atoi(value.c_str()) < 0 || atoi(value.c_str()) > 65535 || value.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument("Error: invalid port");
}

void fill_server(server_data &server, std::string &line){
	std::string key;
	std::string value;
	char eq;
	std::stringstream ss(line);
	ss >> key;
	ss >> eq;
	std::getline(ss, value);
	check_value_key(value, key);
	trim(value, " \t\"");	

	if (key == "host")
		server.host = value;
	else if (key == "port")
	{
		std::vector<std::string> ports;
		tools::splitting_string(value, " ,\"", ports);
		for (size_t i = 0; i < ports.size(); i++)
		{
			check_int(value);
			server.port.push_back(atoi(ports[i].c_str()));
		}
	}
	else if (key == "server_name")
		fill_vector(server.server_names, value);
	else if (key == "root")
		server.home = value;
	else if (key == "client_max_body_size")
		server.client_max_body_size = value;
	else 
		throw std::invalid_argument("Error: invalid server key at `" + line + "`");
}

void fill_location(location &location, std::string &line){
	std::string key;
	std::string value;
	char eq;
	static bool _upload = false;
	std::stringstream ss(line);
	ss >> key;
	ss >> eq;
	std::getline(ss, value);
	check_value_key(value, key);
//	trim(value, " \t\"");
	location.upload = _upload;
	std::cout << "key = " << key;
	std::cout << " value = " << value << std::endl;
	if (key == "allow_methods")
		fill_vector(location.allow_methods, value);
	else if (key == "cgi_path")
		fill_vector(location.cgi_path, value);
	else if (key == "autoindex")
		location.autoindex = value;
	else if (key == "root")
		location.root = value;
	else if (key == "upload" && value == "ok")
	{
		location.upload = true;
		_upload = true;
	}
	else if (key == "index")
		fill_vector(location.index, value);
	else if (key == "cgi_extensions")
		fill_vector(location.cgi_extensions, value);
	else if (key == "return")
	{
		std::vector<std::string> tokens;
		tools::splitting_string(value, " ", tokens);
		location.return_code = atoi(tokens[0].c_str());
		location.return_path = tokens[1];
	}
	else
	{
		std::cout << "key exception = " << key;
		throw std::invalid_argument("Error: invalid location key");
	}
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
	trim(value, " \t\"");
	std::cout << "key = " << key;
	std::cout << " value = " << value << std::endl;
	if (key < 400 || key > 599 || k.find_first_not_of("0123456789") != std::string::npos || k.empty())
		throw std::invalid_argument("Error: invalid error_page key");
	else if (value.empty() || value[0] != '/' || value.substr(value.find_last_of(".")) != ".html")
		throw std::invalid_argument("Error: invalid error_page value");
	error_page[key] = value;
}
