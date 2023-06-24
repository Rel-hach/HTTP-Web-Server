// made by : rel-hach

#include "../../inc/serv_monitor.hpp"

// utils 


std::string    getting_errorPage(int status)
{
    std::string Badreq = "<HTTP/1.1 400 \r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title> Bad Request : 400 </title><style>body{font-family: Arial, sans-serif;background-color: #f7f7f7;}.container{margin: 50px auto;text-align: center;}h1{color: #d0021b;font-size: 36px;margin-bottom: 20px;}p{font-size: 18px;margin-bottom: 20px;}button{background-color: #d0021b;color: #fff;border: none;padding: 10px 20px;font-size: 18px;border-radius: 5px;cursor: pointer;}button:hover {background-color: #a30014;}</style></head><body><div class='container'><h1>Bad Request</h1><p>Sorry, we were unable to process your request.</p></div></body></html>"; 
    std::string unauthorized = "<HTTP/1.1 401 \r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title> Unauthorized : 401 </title><style>body{font-family: Arial, sans-serif;background-color: #f7f7f7;}.container{margin: 50px auto;text-align: center;}h1{color: #d0021b;font-size: 36px;margin-bottom: 20px;}p{font-size: 18px;margin-bottom: 20px;}button{background-color: #d0021b;color: #fff;border: none;padding: 10px 20px;font-size: 18px;border-radius: 5px;cursor: pointer;}button:hover {background-color: #a30014;}</style></head><body><div class='container'><h1>Unauthorized</h1><p>Sorry, you are not authorized.</p></div></body></html";
    std::string not_found = "<HTTP/1.1 404 \r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title> Not Found : 404 </title><style>body{font-family: Arial, sans-serif;background-color: #f7f7f7;}.container{margin: 50px auto;text-align: center;}h1{color: #d0021b;font-size: 36px;margin-bottom: 20px;}p{font-size: 18px;margin-bottom: 20px;}button{background-color: #d0021b;color: #fff;border: none;padding: 10px 20px;font-size: 18px;border-radius: 5px;cursor: pointer;}button:hover {background-color: #a30014;}</style></head><body><div class='container'><h1>Not Found</h1><p>Sorry, not found.</p></div></body></html";
    std::string method_notAllowd = "<HTTP/1.1 405 \r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title> Not Allowd : 405 </title><style>body{font-family: Arial, sans-serif;background-color: #f7f7f7;}.container{margin: 50px auto;text-align: center;}h1{color: #d0021b;font-size: 36px;margin-bottom: 20px;}p{font-size: 18px;margin-bottom: 20px;}button{background-color: #d0021b;color: #fff;border: none;padding: 10px 20px;font-size: 18px;border-radius: 5px;cursor: pointer;}button:hover {background-color: #a30014;}</style></head><body><div class='container'><h1>Not Allowed</h1><p>Sorry, method is not allowed.</p></div></body></html";
    std::string tooLong = "<HTTP/1.1 414 \r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title> Too Long Uri : 414 </title><style>body{font-family: Arial, sans-serif;background-color: #f7f7f7;}.container{margin: 50px auto;text-align: center;}h1{color: #d0021b;font-size: 36px;margin-bottom: 20px;}p{font-size: 18px;margin-bottom: 20px;}button{background-color: #d0021b;color: #fff;border: none;padding: 10px 20px;font-size: 18px;border-radius: 5px;cursor: pointer;}button:hover {background-color: #a30014;}</style></head><body><div class='container'><h1>Too Long Uri</h1><p>Sorry, uri is too long.</p></div></body></html";
    std::string httpVersionErr = "<HTTP/1.1 505 \r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title> Not Supported : 505 </title><style>body{font-family: Arial, sans-serif;background-color: #f7f7f7;}.container{margin: 50px auto;text-align: center;}h1{color: #d0021b;font-size: 36px;margin-bottom: 20px;}p{font-size: 18px;margin-bottom: 20px;}button{background-color: #d0021b;color: #fff;border: none;padding: 10px 20px;font-size: 18px;border-radius: 5px;cursor: pointer;}button:hover {background-color: #a30014;}</style></head><body><div class='container'><h1>Not Supported</h1><p>Sorry, version (HTTP) not supported.</p></div></body></html";
    std::string timeOut = "<HTTP/1.1 504 \r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title> Time Out : 504 </title><style>body{font-family: Arial, sans-serif;background-color: #f7f7f7;}.container{margin: 50px auto;text-align: center;}h1{color: #d0021b;font-size: 36px;margin-bottom: 20px;}p{font-size: 18px;margin-bottom: 20px;}button{background-color: #d0021b;color: #fff;border: none;padding: 10px 20px;font-size: 18px;border-radius: 5px;cursor: pointer;}button:hover {background-color: #a30014;}</style></head><body><div class='container'><h1>Timeout</h1><p>Sorry, Gateway timeout.</p></div></body></html";
    std::string internal_error = "<HTTP/1.1 500 \r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title> Internal server issue : 500 </title><style>body{font-family: Arial, sans-serif;background-color: #f7f7f7;}.container{margin: 50px auto;text-align: center;}h1{color: #d0021b;font-size: 36px;margin-bottom: 20px;}p{font-size: 18px;margin-bottom: 20px;}button{background-color: #d0021b;color: #fff;border: none;padding: 10px 20px;font-size: 18px;border-radius: 5px;cursor: pointer;}button:hover {background-color: #a30014;}</style></head><body><div class='container'><h1>Internal issue</h1><p>Sorry, there is an internal server issue.</p></div></body></html";
    
    switch (status)
    {
    case 400 :
        return (Badreq);
    
    case 401 :
        return (unauthorized);
    
    case 404 :
        return (not_found);
    
    case 405 :
        return (method_notAllowd);
    
    case 414 :
        return (tooLong);
    
    case 500 :
        return (internal_error);
    
    case 504 :
        return (timeOut);
    
    case 505 :
        return (httpVersionErr);
    
    default:
        return (Badreq);
    }
}


void    Monitor::starting_theProcess ( std::vector<Server> servers )
{
    std::vector<Listeningsock> listeners(servers.size());

    for (size_t i = 0; i < servers.size(); i++)
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
            break ;  }

        for (size_t i = 0; i <_numOfFds; i++)
        {
            if (LISTENING_SOCKET && EVENT_OCCURED & POLLIN)
            {
                int index = getting_serverIndex(_ALLFDS[i].fd, listeners);
                if (accepting_newClient( _ALLFDS[i].fd ))
                {
                    request Request(servers[index]);
                    _clients.insert(std::make_pair(_ALLFDS[i].fd, Request));
                }
            }

            else if (CONNECTING_SOCKET && EVENT_OCCURED & POLLIN)
            {
                receiving_clientRequest(_ALLFDS[i].fd, i);
            }

            else if (CONNECTING_SOCKET && EVENT_OCCURED & POLLOUT)
            {
                //  socket is ready for writing
                //  sending request
                sending_responseToClient(_ALLFDS[i].fd, i);


                // deleting fd from pollfds
                // close fd
                // delete request oject.
            }

            else if (EVENT_OCCURED & POLLERR)
                writing_errorMessage(_ALLFDS[i], i);
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


// ------------------------------- [ACCEPTING]---------------

bool    Monitor::accepting_newClient( int listenFd)
{
    int connFd = accept(listenFd, NULL, NULL);
    if (connFd > 0)
    {
        _ALLFDS.push_back( {connFd, POLLIN, 0} );
        fcntl(connFd, F_SETFL, O_NONBLOCK);
        _numOfFds++;
        return (true);
    }
    else
        close(connFd);
    return (false);
}

// -----------------------------------------------------------




// --------------------- [ RECEIVING ] ------------------------


void    Monitor::receiving_clientRequest( int& connFd, size_t& i)
{
    int     ret;
    char    buffer[1024 * 3];

    std::memset(buffer, '\0', sizeof(buffer));
    ret = recv(connFd, buffer, strlen(buffer) - 1, 0);
    if (ret > 0)
    {   
        _clients[connFd]._request.append(buffer, ret);
        _clients[connFd]._readBytes += ret;

        // checking if the request reched its end.
        if (requestIsFullyReceived(_clients[connFd]) && _clients[connFd]._isReceived == false)
        {
            _clients[connFd].Processing_HttpRequest();
            _clients[connFd]._requestIsParsed = true;
        }

        // checking if there an errors after parsing the request.
        if (_clients[connFd]._status != GO_NEXT || _clients[connFd]._errorFound == true)
        {
            _clients[connFd]._response = getting_errorPage(_clients[connFd]._status);
            _ALLFDS[i].events = POLLOUT;
        }

        // ------------------------ POST CASE 

        else if (_clients[connFd]._ischunked && _clients[connFd]._requestIsParsed == true)
        {
            _clients[connFd].unchunking();
            if (_clients[connFd]._BodyIsFullyReceived == true)
            {
                // preparing_response ();
                _ALLFDS[i].events = POLLOUT;
            }
        }
        else if (_clients[connFd]._hasContentLength && _clients[connFd]._bytesToRead >= _clients[connFd]._readBytes && _clients[connFd]._requestIsParsed)
        {
            _clients[connFd]._BodyIsFullyReceived = true; // ghir zayed.
            _ALLFDS[i].events = POLLOUT;
        }
        
        // ----------------------------------------

        else if (_clients[connFd]._method == GET && _clients[connFd]._requestIsParsed)
        {
            // handle GET case;

            std::string getTest = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World! I AM THE GET METHOD</h1></body></html>";
            _clients[connFd]._response = getTest;
            _ALLFDS[i].events = POLLOUT;
            _ALLFDS[i].events = POLLOUT; 
        }

        else if (_clients[connFd]._method == DELETE && _clients[connFd]._requestIsParsed)
        {
            // handle Delete case;
            // _clients[connFd]._response = handling response.
            _ALLFDS[i].events = POLLOUT;
        }
    }
}

// ----------------------------------------------

bool    Monitor::requestIsFullyReceived( request& request )
{
    size_t ret = request._request.find("\r\n\r\n");
    if ((ret != std::string::npos && request._isReceived) == false)
    {
        request._HeadRequest = request._request.substr(0, ret + 4);
            request._request.erase(0, ret + 4);
        request._isReceived = false;
        return (true);
    }
    return (false);    
}

// ---------------------- [SENDING] ----------


void    Monitor::sending_responseToClient( int& fd, size_t& i)
{
    size_t size = _clients[fd]._response.size();
    if (send(fd, _clients[fd]._response.c_str(), size - 1, 0) == -1)
    {
        std::cout << "Sending fail .. " << std::endl;
        std::vector<pollfd>::iterator it = _ALLFDS.begin() + i;
        int to_close = it->fd;
        _ALLFDS.erase(it);

        std::map<int, request>::iterator it_map = _clients.find(fd);
        _clients.erase(it_map);
        close (to_close);
    }

    std::cout << "response is sent" << std::endl;
    std::vector<pollfd>::iterator it = _ALLFDS.begin() + i;
    int to_close = it->fd;
    _ALLFDS.erase(it);
    // if connection is not 'close' is keep alive
    // initliaze request and don't close fd
    std::map<int, request>::iterator it_map = _clients.find(fd);
    _clients.erase(it_map);
    close (to_close);
}

// --------------------------------------


// -- error 

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



