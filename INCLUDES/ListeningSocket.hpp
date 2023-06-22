#ifndef LISTENINGSOCKET_HPP
#define LISTENINGSOCKET_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <map>
#include <iostream>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include "cstring"


class Monitor;
class Server;

class Listeningsock
{
    public:

        // attributes :

        int _fd;
        struct sockaddr_in  _socket_addr;
        struct sockaddr_in  _socketInfos;
        socklen_t           _length;
        int     port;
        in_addr host;

        // methods :
        
        int     creating_ListeningSockets( Server& servers );
        void    retrieving_hostAndPort( Server &infos );
        int     printing_error( std::string error );
};

#endif