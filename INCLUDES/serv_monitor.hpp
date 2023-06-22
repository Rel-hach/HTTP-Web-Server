#ifndef SERV_MONITOR_HPP
#define SERV_MONITOR_HPP


#include "ListeningSocket.hpp"
#include "status_codes.hpp"
#include <vector>
#include <map>
#include <poll.h>
#include "request.hpp"

#define EVENT_OCCURED       _ALLFDS[i].revents
#define LISTENING_SOCKET    checking_ifServerSocket(_ALLFDS[i].fd, listeners)
#define CONNECTING_SOCKET   !checking_ifServerSocket(_ALLFDS[i].fd, listeners)


// just for testing 
class Server
{
    int port;
    in_addr host;
    std::string server_name;
    std::string root;
    std::string autoindex;
    std::string upload_path;
    std::vector<std::string> index_vec;
    std::size_t client_max_body_size;
    std::map<int, std::string> map_of_error_page;
    std::vector<std::map<std::string, std::string> > vec_of_locations;
    std::string serverChunk;
};


class   Monitor
{
    public:
        // attributes :

        std::vector<pollfd>         _ALLFDS;
        std::vector<int>            _connectingFds;
        size_t                      _numOfFds;
        std::map<int, request>      _clients;

        // methods :

            void    starting_theProcess ( std::vector<Server> servers );
            bool    checking_ifServerSocket(int fd, std::vector<Listeningsock>& listener);
            int     getting_serverIndex(int fd, std::vector<Listeningsock> listeners);
            bool    accepting_newClient( Server& server, Listeningsock& listener );
            void    initializing_request (int& clientFd, Server& index);
            void    receiving_clientRequest( int& connFd, size_t& i, Server& server );
            void    sending_responseToClient();
            void    writing_errorMessage( pollfd& fd, int i );
};


#endif