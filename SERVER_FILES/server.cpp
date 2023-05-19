#include "../INCLUDES/server.hpp"
#include "../INCLUDES/request.hpp"

#include <netinet/in.h>
#include <arpa/inet.h>


int main (int ac, char **av)
{
    int sockFd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in serveradd;

    serveradd.sin_port = htons(std::atoi(av[2]));
    serveradd.sin_family = AF_INET;
    serveradd.sin_addr.s_addr = inet_addr(av[1]);
    bind(sockFd, reinterpret_cast<sockaddr *>(&serveradd), static_cast<socklen_t>(sizeof(sockaddr_in)));
    listen(sockFd, 1);
    accept(sockFd, nullptr, nullptr);

    char buffer[1025];
    std::string req;
    size_t nbytes;
    request request;
    while ((nbytes = recv(sockFd, buffer, 1024, 0)) > 0)
    {
        buffer[nbytes] = '\0';
        req += buffer;
        //request.parse( req );
    }
}
