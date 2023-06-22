#include "../INCLUDES/serv_monitor.hpp"



void    Monitor::starting ( std::vector<Server> servers )
{
    std::vector<Listeningsock> listeners(servers.size());

    for (int i = 0; i < servers.size(); i++)
    {
        int fd = listeners[i].creating_ListeningSockets(servers[i]);
        fcntl(fd, F_SETFL, O_NONBLOCK);
        _ALLFDS.push_back({fd, POLLIN, 0});
    }

    _numOfFds = _ALLFDS.size();
    
}


