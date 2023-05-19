#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sys/socket.h>
#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <vector>

class request
{
    private:
        std::string                         method;
        std::string                         path;
        std::string                         http_version;
        std::string                         start_line;
        size_t                              position;
        std::vector<std::string>            requests;
        std::map<std::string, std::string>  headers;
        std::string                         key;
        std::string                         value;
    public:
        void    parse( std::string& str );
        void    parse_http_request( void );
        void    parse_start_line( void );
        void    parse_headers ( void );
};

#endif