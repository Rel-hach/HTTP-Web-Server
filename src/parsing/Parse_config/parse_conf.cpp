#include "../../../inc/server_data.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>
#include <cwctype>
#include <stdlib.h>
#include "../../../inc/tools.hpp"

void check_is_empty(std::vector<server_data> &servers){
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (servers[i].is_empty != ( EMPTY | SERVER_NAME | PORT | HOST ))
			throw std::invalid_argument("Error: empty interesting information");
	}
}

std::vector<server_data> parse_server(std::string config_file)
{
	std::ifstream file;
	std::vector<server_data> servers;
	std::string line;
	std::pair<std::string, std::string> uri_p;
	std::string uri;
	e_key flag = UNKNOWN;
	int pseudo_flag = 0;

	file.open(config_file.c_str());
	if (!file.is_open())
		throw std::invalid_argument("Error: cannot open config file");
	if (file.peek() == std::ifstream::traits_type::eof())
		throw std::invalid_argument("Error: empty config file");
	while (std::getline(file, line))
	{
		trim(line, " \t");
		if (line[0] == '#' || line.empty())
			continue;
		else if (line == "[[server]]")
		{
			servers.push_back(server_data());
			servers.back().is_empty = EMPTY;
			flag = SERVER;
		}
		else if (line == "[[server.location]]")
		{
			flag = LOCATION;
			pseudo_flag = 1;
		}
		else if (line == "[[server.error_page]]")
			flag = ERROR_PAGE;
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
	check_is_empty(servers);
	return servers;
}
