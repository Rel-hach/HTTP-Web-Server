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
#include "location.hpp"
#include <cstring>
#include <algorithm>


class server
{
    // just for test
    public:
        int port;
        in_addr host;
        std::string server_name;
        std::string root;
        std::string autoindex;
        std::string upload_path;
        std::vector<std::string> index_vec;
        std::size_t client_max_body_size;
        std::map<int, std::string> map_of_error_page;
        std::vector<std::map<std::string, std::string> > location;
    private:
        int m_socket;
        sockaddr_in server_address;
        sockaddr_in client_address;
        socklen_t client_address_size;

    public:
        server() {}
        // server(server&);
        // server(const server &);
       //  server&operator=(const server&);
        // void set_keys();
        server(int port);
        ~server();
        int  startServer();
        void closeServer();
        int bindServer();
        int listenSrver();
        int acceptServer();

        int getSockert() const ;
        void setSockert( const int socket);

        sockaddr_in  &getClientAdtess() const ;
        socklen_t  &getClientAdtessSize() const ;
        // std::string host;
};

#endif
