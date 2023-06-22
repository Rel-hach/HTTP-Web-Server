#ifndef SERV_MONITOR_HPP
#define SERV_MONITOR_HPP


#include <ListeningSocket.hpp>
#include <vector>
#include <map>
#include <poll.h>


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
            int     getting_serverIndex();
            void    accepting_newClient();
            void    receiving_clientRequest();
            void    sending_responseToClient();
            void    writing_errorMessage();
};

#endif