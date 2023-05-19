#include "../INCLUDES/request.hpp"

void    request::parse_http_request( void )
{
    recv
}

void    request::parse_start_line( void )
{
    if (std::count(start_line.begin(), start_line.end(), ' ') != 2)
    {
        std::cout << "Error" << std::endl; return ; }

    position = start_line.find(' ');
    method = start_line.substr(0, position);
    path = start_line.substr(position + 1);
    if (position = start_line.find(' ') != std::string::npos)
    { 
        http_version = path.substr(position + 1);
        path = path.substr(0, position);
    }
}

void    trim_spaces(std::string& str)
{
    str.erase();   
}

void    request::parse_headers ( void )
{
    std::vector<std::string>::iterator it = requests.begin() + 1;

    for (it; it != requests.end(); it++)
    {
        if (position = it->find(':') != std::string::npos)
        {
            key = it->substr(0, position);
            value = it->substr(position + 1);
        }
    }
}

void    request::parse(std::string& str)
{
    if (str.find("\t\n"))
        // somethig .. 
    
}