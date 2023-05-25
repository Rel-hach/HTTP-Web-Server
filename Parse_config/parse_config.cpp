#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#define configFile "webserv.conf"


void    config_file()
{
    std::ifstream ifs(configFile, std::ios::in);
    std::string line;
    if (!ifs.is_open () || ifs.fail()) { std::cout << "config File is missing or need permission.. " << std::endl; return ; }

    while (getline(ifs, line))
    {
        
    }
}