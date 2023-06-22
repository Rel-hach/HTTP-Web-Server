#ifndef SERV_MONITOR_HPP
#define SERV_MONITOR_HPP


#include <ListeningSocket.hpp>
#include <vector>
#include <map>
#include <poll.h>

#define EVENT_OCCURED _ALLFDS[i].revents
#define LISTENING_SOCKET    checking_ifServerSocket(_ALLFDS[i].fd, listeners)
#define CONNECTING_SOCKET   !checking_ifServerSocket(_ALLFDS[i].fd, listeners)


class Server
{

};


class   Monitor
{
    public:
        // attributes :

        std::vector<pollfd>         _ALLFDS;
        std::vector<int>            _connectingFds;
        size_t                      _numOfFds;

        // methods :

            void    starting_theProcess ( std::vector<Server> servers );
            bool    checking_ifServerSocket(int fd, std::vector<Listeningsock>& listener);
            int     getting_serverIndex(int fd, std::vector<Listeningsock> listeners);
            void    accepting_newClient( Server& server, Listeningsock& listener );
            void    receiving_clientRequest( int& connFd, size_t& i, Server& server );
            void    sending_responseToClient();
            void    writing_errorMessage( pollfd& fd, int i );
};


#endif