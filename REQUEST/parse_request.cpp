#include "../INCLUDES/request.hpp"

void    request::parse_http_request( void )
{

}

void    request::parse_start_line( void )
{
    if (std::count(start_line.begin(), start_line.end(), ' ') != 2)
    {
        std::cout << "Error" << std::endl; return ;
    }

    
}

void    request::parse_headers ( void )
{

}