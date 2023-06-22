#include "../INCLUDE/ListeningSocket.hpp"




int     Listeningsock::creating_ListeningSockets( Server& servers )
{
     int option = 1;

    // retrieving Host and Port

    if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        printing_error("socket creation failed ");
    
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