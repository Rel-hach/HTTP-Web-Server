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
        
        void    retrieve_hostAndPort(Server &infos);
        int     creating_ListeningSockets( Server& servers );
};

#endif