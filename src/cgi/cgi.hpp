#ifndef CGI
#define CGI
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

class cgi
{
    private:
        std::string body;
        std::string respense;
        std::string flag;
        // int stutes;
    public:
        cgi();
        cgi(std::string body, std::string respense, std::string flag);
        ~cgi();
        std::string getbody() const;
        std::string getrespense() const;
        std::string getflag() const;
        void setbody(std::string const body);
        void setrespense(std::string const respense);
        void setflag(std::string const flag);
};



#endif