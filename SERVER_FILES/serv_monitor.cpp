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

    while (true)
    {
        // using poll to wait for an event to come.
        int ret = poll(&_ALLFDS[0], _numOfFds, -1);

        if (ret < 0)
        {   std::cerr << "Poll function has been failed " << std::endl;
            break ; }

        size_t i = 0;
        for (i = 0; i <_numOfFds; i++)
        {
            if (IS_SERVER_FD && _ALLFDS[i].revents & POLLIN)
            {
                // getting server index ..
                int index = getting_serverIndex();
                // accepting the new client 
                accepting_newClient();
            }
            else if (!IS_SERVER_FD && _ALLFDS[i].revents & POLLIN)
            {
                // getting server index 
                int index = getting_serverIndex();
                // receiving request from client
                receiving_request(_ALLFDS[i].fd, i, servers[index]);
            }
            else if (!IS_SERVER_FD && _ALLFDS[i].revents & POLLOUT)
            {
                // socket is ready for writing
                // sending request

                // deleting fd from pollfds
                // close fd
                // delete request oject.
            }
            else
            {
                // there is an error.
                // writing_error 
            }
        }
    }

}


