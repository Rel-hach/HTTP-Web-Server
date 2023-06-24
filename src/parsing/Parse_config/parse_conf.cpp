#include "../../../inc/server.hpp"
#include <fstream>
#include <string>
#include <vector>


void fill_server(std::vector<server> &servers, std::string line){
	std::string key;
	std::string value;
	key = line.substr(0, line.find('='));
	value = line.substr(line.find('=') + 1, line.length());
	if (key == "host")
		servers.back().host = value;
	else if (key == "port")
		servers.back().port = std::stoi(value);
	else if (key == "server_name")
		servers.back().server_names.push_back(value);
	else if (key == "root")
		servers.back().root = value;
	else if (key == "index")
		servers.back().index = value;
	else if (key == "autoindex")
		servers.back().autoindex = value;
	else if (key == "upload_path")
		servers.back().upload_path = value;
	else if (key == "error_page")
		servers.back().error_page = value;
	else if (key == "methods")
		servers.back().methods.push_back(value);
	else if (key == "cgi_extensions")
		servers.back().cgi_extensions.push_back(value);
}

std::vector<server> parse_server(std::string config_file){
	std::ifstream file;//(config_file);
	std::string line;
	std::string key;
	std::string value;
	std::vector<server> servers;

	try {
		file.open(config_file);
		while (std::getline(file, line)){
			if (line[0] == '#')
				continue;
			if (line == "[[server]]"){
				servers.push_back(server());
				while (std::getline(file, line)){
					if (line[0] == '#')
						continue;
					else if (line == "[[server]]")
						break;
					else if (line == "[[location]]"){
						servers.back().locations.push_back(location());
						while (std::getline(file, line)){
							if (line[0] == '#')
								continue;
							else if (line == "[[location]]")
								break;
							fill_location(servers.back().locations.back(), line);
						}
					fill_server(servers.back(), line);
					}
						
				}
			}
		}
	}
	catch (const std::exception&) {
		std::cout << "Error: could not open/read file" << std::endl;
		exit(1);
	}
	return servers;
}
