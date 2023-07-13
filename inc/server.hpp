#ifndef SERVER_HPP
#define SERVER_HPP
#include "request.hpp"
#include <sys/socket.h> 
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <map>
#include <signal.h>
#include <fcntl.h>
#include <poll.h>
#include "client.hpp"
#include <cstring>
#include <algorithm>

class server_data
{
    private:
        int m_socket;
        sockaddr_in server_address;
        sockaddr_in client_address;
        socklen_t client_address_size;

    public:
        server_data(int port);
        sockaddr_in  &getClientAdtess() const ;
        socklen_t  &getClientAdtessSize() const ;
        int getSockert() const ;
        void setSockert( const int socket);
        int  startServer();
        void closeServer();
        int bindServer();
        int listenSrver();
        int acceptServer();
        ~server_data();
};

#endif
