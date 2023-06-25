

	try {
		file.open(config_file);
		while (std::getline(file, line)){
			if (line[0] == '#' || line.empty())
				continue;
			else if (line == "[[server]]"){
				std::cout << "found server" << std::endl;
				servers.push_back(server());
				while (std::getline(file, line)){
					if (line[0] == '#' || line.empty())
						continue;
					else if (line == "[[server]]")
						break;
					else {
						std::stringstream ss(line);
						ss >> key;
						if (key == "[[server.location]]"){
							std::cout << "found location" << std::endl;
							servers.back().locations.push_back(location());
							while (std::getline(file, line)){
								if (line[0] == '#')
									continue;
								else if (line == "[[location]]")
									break;
								fill_location(servers.back().locations.back(), line);
							}
						}
						else if (key == "[[server.error_page]]"){
							std::cout << "found error_page" << std::endl;
							//TODO: fill error_page
						}
					}
				}
					std::cout << "line = " << line << std::endl;
					fill_server(servers.back(), line);
			}
		}
	}






	try {
		file.open(config_file);
		while (std::getline(file, line)){
			if (line[0] == '#' || line.empty())
				continue;
			else if (line == "[[server]]"){
				std::cout << "found server" << std::endl;
				servers.push_back(server());
				while (std::getline(file, line)){
					if (line[0] == '#' || line.empty())
						continue;
					else if (line == "[[server]]")
						break;



					else if (line == "[[location]]"){
						std::cout << "found location" << std::endl;
						servers.back().locations.push_back(location());
						while (std::getline(file, line)){
							if (line[0] == '#')
								continue;
							else if (line == "[[location]]")
								break;
							fill_location(servers.back().locations.back(), line);
						}
					}



					std::cout << "line = " << line << std::endl;
					fill_server(servers.back(), line);
						
				}
			}
		}
	}
