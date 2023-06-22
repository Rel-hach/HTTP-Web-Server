#ifndef SERV_MONITOR_HPP
#define SERV_MONITOR_HPP

class   Monitor
{
    public:
        // attributes :

        std::vector<pollfd>         _ALLFDS;
        std::vector<int>            _connectingFds;
        size_t                      _numOfFds;

        // methods :

            void    starting ( std::vector<Server> servers );
};

#endif