#include "../../inc/ListeningSocket.hpp"


int     Listeningsock::printing_error( std::string error )
{
    std::cerr << "Error : " << error << std::endl;
    exit (EXIT_FAILURE);
}


void    Listeningsock::retrieving_hostAndPort(Server &infos)
{
    host = infos.host;
    port = infos.port;
}

int     Listeningsock::creating_ListeningSockets( Server& serv )
{
     int option = 1;

    // retrieving Host and Port
    retrieving_hostAndPort( serv );

    if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        printing_error("socket creation failed ");
    
    fcntl(_fd, F_SETFL, O_NONBLOCK);

    // setting socket option 
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1)
        printing_error("socket option could not be set .. ");
    
    _socketInfos.sin_family = AF_INET;
    _socketInfos.sin_port = htons(port);
    _socketInfos.sin_addr.s_addr = INADDR_ANY;


    // binding adress to socket.
    if (bind(_fd, (struct sockaddr*)&_socketInfos, sizeof(_socketInfos)) == -1)
        printing_error("Port and host could not be bount to socket");

    // listening to incomig connection.
    if (listen(_fd, SOMAXCONN) == -1)
        printing_error("socket coudn't listen for incoming connection");
    
    static int i;
    std::cout << "Server number " << i++ << " IS LISTENING ON PORT : " << this->port << std::endl;
        
    return (_fd);
}
