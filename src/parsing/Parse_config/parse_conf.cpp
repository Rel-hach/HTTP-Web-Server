#include "../../../inc/server_data.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <cwctype>
#include <stdlib.h>
#include "../../../inc/tools.hpp"

std::vector<server_data> parse_server(std::string config_file)
{
	std::ifstream file;
	std::vector<server_data> servers;
	std::string line;
	std::pair<std::string, std::string> uri_p;
//	std::map<std::string, location>::iterator it;
	std::string uri;
	e_key flag = UNKNOWN;
	int pseudo_flag = 0;

	file.open(config_file.c_str());
	if (!file.is_open())
		throw std::invalid_argument("Error: cannot open config file");
	while (std::getline(file, line))
	{
		trim(line, " \t");
		std::cout << "line = " << line << std::endl;
		if (line[0] == '#' || line.empty())
			continue;
		else if (line == "[[server]]")
		{
//			it = servers.back().locations.begin();
			std::cout << "\033[4;33mfound server\033[0m" << std::endl;
			servers.push_back(server_data());
			flag = SERVER;
		}
		else if (line == "[[server.location]]")
		{
			std::cout << "\033[4;35mfound location\033[0m" << std::endl;
			flag = LOCATION;
			pseudo_flag = 1;
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
			{
				if (pseudo_flag){
					uri_p = fill_pair(line);
					if (uri_p.first != "uri" || uri_p.second.empty())
						throw std::invalid_argument("Error: invalid location key");
					servers.back().locations.insert(std::make_pair(uri_p.second, location()));
					pseudo_flag = 0;
				}
				else
					fill_location(servers.back().locations.at(uri_p.second), line);
			}
			else if (flag == ERROR_PAGE && line != "[[server.error_page]]")
				fill_error_page(servers.back().error_pages, line);
		}
	}
	file.close();
	std::cout << "size ="<< servers.size() << std::endl;
	servers[0].locations["%.go$"].print_location();
//	servers[0].print();
	return servers;
}

// int main (int argc, char *argv[])
// {
// 	std::vector<server_data> servers;
// 	try
// 	{
// 		if (argc != 2)
// 			throw std::invalid_argument("Error: invalid number of arguments");
// 		servers = parse_server(argv[1]);
// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << e.what() << '\n';
// 		return 1;
// 	}
// 	}
// 	return 0;
// }
