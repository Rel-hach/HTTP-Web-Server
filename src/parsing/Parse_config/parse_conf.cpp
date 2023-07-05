#include "../../../inc/server.hpp"
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <sstream>

void fill_server(server &server, std::string line){
	std::string key;
	std::string value;
	char eq;
	std::stringstream ss(line);
	ss >> key;
	ss >> eq;
	ss >> value;
	std::cout << "keyuuu = " << key << std::endl;
	std::cout << "valueuuu = " << value << std::endl;
//	key = line.substr(0, line.find('='));
//	value = line.substr(line.find('=') + 1, line.length());
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

void fill_location(location &location, std::string line){
	std::string key;
	std::string value;
	char eq;
	std::stringstream ss(line);
	ss >> key;
	ss >> eq;
	ss >> value;
	std::cout << "key = " << key << std::endl;
	std::cout << "value = " << value << std::endl;
//	key = line.substr(0, line.find('='));
//	value = line.substr(line.find('=') + 1, line.length());
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

void fill_error_page(std::map<int , std::string> error_page, std::string line){
	int key;
	std::string value;
	char eq;
	std::stringstream ss(line);
	ss >> key;
	ss >> eq;
	ss >> value;
//TODO: check if key is int
//TODO: check if value is string
//TODO: read until end of line for value
	std::cout << "key = " << key << std::endl;
	std::cout << "value = " << value << std::endl;
	if (key < 400 || key > 599)
		throw std::invalid_argument("Error: invalid error_page key");
	error_page[key] = value;
}

std::string handle_key(std::ifstream& file, std::string old_key, server &server){

	std::string line;
	std::string key;
	std::stringstream ss;
	while (std::getline(file, line)){
		ss.str("");
		ss.clear();
		ss << line;
		ss >> key;
		std::cout << "key in handle = " << key << std::endl;
		if (line[0] == '#' || line.empty())
			continue;
		else if (key == "[[server.error_page]]" || key == "[[server]]" || key == "[[server.location]]")
			return key;
		//	break;
		if (old_key == "[[server.error_page]]")
			fill_error_page(server.error_pages, line);
		else if (old_key == "[[server.location]]")
			fill_location(server.locations.back(), line);
		else
			return key;
//			fill_server(server, line);
	}
	return key;
}

std::vector<server> parse_server(std::string config_file){
	std::ifstream file;//(config_file);
	std::string line;
	std::string key;
	std::string value;
	std::vector<server> servers;
	std::stringstream ss;

	try {
		file.open(config_file);
		while (std::getline(file, line)){
			if (key != "[[server]]" && key != "[[server.location]]" && key != "[[server.error_page]]")
			{
				ss.str("");
				ss.clear();
				ss << line;
				ss >> key;
			}
			if (line[0] == '#' || line.empty())
				continue;
			else if (key == "[[server]]"){
				std::cout << "\033[4;33mfound server\033[0m" << std::endl;
				servers.push_back(server());
				while (std::getline(file, line)){
					if (key != "[[server.location]]" && key != "[[server.error_page]]")
					{
						ss.str("");
						ss.clear();
						ss << line;
						ss >> key;
					}
					std::cout << "key in try = " << key << std::endl;
					if (line[0] == '#' || line.empty())
						continue;
					else{

						if (key == "[[server.error_page]]"){
							std::cout << "\033[4;35mfound error_page\033[0m" << std::endl;
							key = handle_key(file, key, servers.back());
							//	while (std::getline(file, line)){
							//		ss.str("");
							//		ss.clear();
							//		ss << line;
							//		ss >> key;
							//		if (line[0] == '#' || line.empty())
							//			continue;
							//		else if (key == "[[server.error_page]]" || key == "[[server]]" || key == "[[server.location]]")
							//			break;
							//		fill_error_page(servers.back().error_pages, line);
							//	}
							}

						if (key == "[[server.location]]"){
								std::cout << "\033[4;35mfound location\033[0m" << std::endl;
								key = handle_key(file, key, servers.back());
							//	servers.back().locations.push_back(location());
							//	while (std::getline(file, line)){
							//		ss.str("");
							//		ss.clear();
							//		ss << line;
							//		ss >> key;
							//		if (line[0] == '#' || line.empty())
							//			continue;
							//		else if (key == "[[server.location]]" || key == "[[server]]" || key == "[[server.error_page]]")
							//			break;
							//		fill_location(servers.back().locations.back(), line);
							//	}
							}
						if (key == "[[server.location]]" || key == "[[server.error_page]]")
							continue;
						if (key == "[[server]]")
							break;
						}
					std::cout << "line = " << line << std::endl;
					fill_server(servers.back(), line);
				}
			}
		}
		file.close();
	}

	catch (const std::exception&) {
		std::cout << "Error: could not open/read file" << std::endl;
	//	exit(1);
	}
	return servers;
}
//	try {
//		file.open(config_file);
//		while (std::getline(file, line)){
//			if (line[0] == '#' || line.empty())
//				continue;
//			else if (line == "[[server]]"){
//				std::cout << "found server" << std::endl;
//				servers.push_back(server());
//				while (std::getline(file, line)){
//					if (line[0] == '#' || line.empty())
//						continue;
//					else if (line == "[[server]]")
//						break;
//
//
//
//					else if (line == "[[location]]"){
//						std::cout << "found location" << std::endl;
//						servers.back().locations.push_back(location());
//						while (std::getline(file, line)){
//							if (line[0] == '#')
//								continue;
//							else if (line == "[[location]]")
//								break;
//							fill_location(servers.back().locations.back(), line);
//						}
//					}
//
//
//
//					std::cout << "line = " << line << std::endl;
//					fill_server(servers.back(), line);
//						
//				}
//			}
//		}
//	}

int main (int argc, char *argv[])
{
	std::vector<server> servers;
	servers = parse_server(argv[1]);
	std::cout << "servers[0].host = " << servers[0].host << std::endl;
//	std::cout << "servers[0].server_names = " << servers[0].server_names[0] << std::endl;
	return 0;
}
