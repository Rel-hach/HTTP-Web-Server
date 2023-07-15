#ifndef CGI_HPP
#define CGI_HPP
#include <sys/socket.h> 
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <fcntl.h>


class cgi
{
private:
   std::string body;
   
public:
    cgi(/* args */);
    ~cgi();
};




#endif
