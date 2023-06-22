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
                if (accepting_newClient( servers[index], listeners[index] ))
                {
                    request Request(_ALLFDS[i].fd, servers[index]);
                    _clients.insert(std::make_pair(_ALLFDS[i].fd, Request));
                }
            }

            else if (CONNECTING_SOCKET && EVENT_OCCURED & POLLIN)
            {
                // getting server index 
                int index = getting_serverIndex();
                // receiving request from client
                //receiving_clientRequest();
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

bool    Monitor::accepting_newClient( Server& server, Listeningsock& listener )
{
    int connFd = accept(listener._fd, NULL, NULL);
    if (connFd > 0)
    {
        _ALLFDS.push_back( {connFd, POLLIN, 0} );
        fcntl(connFd, F_SETFL, O_NONBLOCK);
        _numOfFds++;
        return (true)
    }
    else
        close(connFd);
    return (false);
}

void    Monitor::receiving_clientRequest( int& FD, size_t& i, Server& server )
{
    // int  retValue;

    // char    buffer[1024 * 3];
    // std::memset(buffer, '\0', sizeof(buffer));
    // retValue = recv(FD, buffer, strlen(buffer) - 1, 0);
    // if (retValue > 0)
    // {
    //     _client[FD]._request.append(buffer, retValue);
    //     _client[FD]._readBytes += retValue;

    //     std::cout << _client[FD]._request << std::endl;
    //     std::cout << _client[FD]._readBytes << std::endl;
    //     // requestIsFullyReceived must be implemented
    //     // if (requestIsFullyReceived(_client[FD]) && _client[FD]._isReceived == false)
    //     // {
    //     //     _client[FD].Processing_HttpRequest();
    //     //     _client[FD]._requestIsParsed = true;
    //     // }
    //     if (_client[FD]._status != GO_NEXT || _client[FD]._errorFound == true)
    //     {
    //         // getting_errorPage must be imlemented
    //         _client[FD]._response = getting_errorPage(_client[FD]._status);
    //         _ALLFDS[i].events = POLLOUT;
}
    //}
//}

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