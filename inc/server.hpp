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
#include "server_data.hpp"

class server
{
    private:
        int m_socket;
        sockaddr_in server_address;
        sockaddr_in client_address;
        socklen_t client_address_size;

    public:
        server(server_data &data);
        sockaddr_in  &getClientAdtess() const ;
        socklen_t  &getClientAdtessSize() const ;
        int getSockert() const ;
        void setSockert( const int socket);
        int  startServer(server_data &data);
        void closeServer();
        int bindServer();
        int listenSrver();
        int acceptServer();
        ~server();
};

#endif
