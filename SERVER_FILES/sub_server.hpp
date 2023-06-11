#ifndef SUB_SERVER_HPP
#define SUB_SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <map>
#include <iostream>
#include <poll.h>

struct  infos
{
        int port;
        in_addr host;
        std::string server_name;
        std::string root;
        std::string autoindex;
        std::size_t client_max_body_size;
        std::string upload_path;
        std::vector<std::string> index_vec;
        std::map<int, std::string> map_of_error_page;
        std::vector<std::map<std::string, std::string> > vec_of_locations;
        std::string serverChunk;
};

class   sub_serv
{
    public:

        // variables :
        int                             _socket_fd;
        struct sockaddr_in              _socketAddr;
        std::vector<int, std::string>   _request;
        socklen_t                       _addrLenght;
        std::vector<pollfd>             _clientsFd;
        
    
        // methods :
        sub_serv( std::vector<Server>& infos );

        
};



#endif