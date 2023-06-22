#include "../INCLUDES/serv_monitor.hpp"



void    Monitor::starting_theProcess ( std::vector<Server> servers )
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
            if (LISTENING_SOCKET && EVENT_OCCURED & POLLIN)
            {
                // getting server index ..
                int index = getting_serverIndex (_ALLFDS[i].fd, listeners);
                // accepting the new client 
                accepting_newClient( servers[index], listeners[index]  );
            }
            else if (CONNECTING_SOCKET && EVENT_OCCURED & POLLIN)
            {
                // getting server index 
                int index = getting_serverIndex();
                // receiving request from client
                receiving_clientRequest();
            }
            else if (CONNECTING_SOCKET && EVENT_OCCURED & POLLOUT)
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


bool     Monitor::checking_ifServerSocket(int fd, std::vector<Listeningsock>& listeners)
{
    for (size_t i = 0; i < listeners.size(); i++)
    {
        if (fd == listeners[i]._fd)
            return (true);
    }
    return (false);   
}



int     Monitor::getting_serverIndex(int fd, std::vector<Listeningsock> listeners)
{
    for (size_t i = 0; i < listeners.size(); i++)
    {
        if (fd == listeners[i]._fd)
            return (i);
    }
    return (0);
}

void    Monitor::accepting_newClient( Server& server, Listeningsock& listener )
{
    int connFd = accept(listener._fd, NULL, NULL);
    if (connFd > 0)
    {
        _ALLFDS.push_back( {connFd, POLLIN, 0} );
        fcntl(connFd, F_SETFL, O_NONBLOCK);
        _numOfFds++;
    }
    else
        close(connFd);
}

void    Monitor::receiving_clientRequest( int& connFd, size_t& i, Server& server )
{

}

void    Monitor::sending_responseToClient()
{

}

void    Monitor::writing_errorMessage( pollfd& fd, int i)
{
    if (fd.revents & POLLERR)
        std::cerr << "POLLER: An error occurred on the file descriptor ";
    else
        std::cerr << "Error: An error occurred";
    // closing & removing _ALLFDS[i] from pollfd
    close (_ALLFDS[i].fd);
    std::vector<pollfd>::iterator it_p = _ALLFDS.begin() + i;
    _ALLFDS.erase(it_p);
}




