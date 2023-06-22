#ifndef SERV_MONITOR_HPP
#define SERV_MONITOR_HPP


#include "ListeningSocket.hpp"
#include "request.hpp"
#include <vector>
#include <map>
#include <poll.h>

#define EVENT_OCCURED       _ALLFDS[i].revents
#define LISTENING_SOCKET    checking_ifServerSocket(_ALLFDS[i].fd, listeners)
#define CONNECTING_SOCKET   !checking_ifServerSocket(_ALLFDS[i].fd, listeners)



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
            bool    accepting_newClient( int listenFd );
            void    initializing_request (int& clientFd, Server& index);
            void    receiving_clientRequest( int& connFd, size_t& i );
            bool    requestIsFullyReceived( request& request );
            void    sending_responseToClient(int& fd, size_t& i);
            void    writing_errorMessage( pollfd& fd, int i );
};


// ERROR PAGES :



#endif