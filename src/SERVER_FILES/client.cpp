#include "../../inc/client.hpp"

client::client()
{

}
client::~client()
{

}

client::client(int fd)
{
    this->req = "";
    this->fd =fd;
    this->contentlenght=0;
    this->contentread = 0;
    this->firstbuff = false;
    this->server_name="";
    this->sendLenth = 0;

}

int client::getfd() const
{
    return this->fd;
}

void client::setfd(int const fd)
{
    this->fd = fd;
}

std::string client::getreq() const
{
    return this->req;
}


void client::appendreq(char const *req, int count)
{
    if(!this->firstbuff)
    {
        std::string input(req);
        this->firstbuff = true;
        int firstposition = input.find("Content-Length:"); 
        if(firstposition !=  (int)std::string::npos)
        {
            int lastposition = input.find("\n",firstposition);
            this->contentread = (-1 * lastposition) ;
            std::string lent = input.substr(firstposition + 15,lastposition - firstposition);
            if(lent.length())
                this->contentlenght= std::stoi(lent);
            this->ischunked= false;
        }
        else if (input.find("Transfer-Encoding: chunked") !=  std::string::npos)
        {
            this->ischunked= true;
        }

        
    }
    this->req.append(req,count);
}

void client::setcontentlenght(long long const contentlenght)
{
    this->contentlenght = contentlenght;

}
long long client::getcontentlenght() const
{
    return this->contentlenght;
}
void client::setfirstbuff(const bool firstbuffer)
{
    this->firstbuff = firstbuffer;

}
bool client::getfirstbuff() const
{
    return this->firstbuff;
}

void client::addTocontentread(long long const contentread)
{
    this->contentread += contentread;

}
long long client::getcontentread() const
{
    return this->contentread;
}

void client::setservr_name(char const *first_req)
{
    if(!firstbuff)
    {
        std::string buff(first_req);
        int index1 = buff.find("Host:") + 6;
        int index2 = buff.find("\r\n", index1);
        if(index1 != (int)std::string::npos && index2 != (int)std::string::npos )
        {
            this->server_name = buff.substr(index1 , index2 - index1);
            
        }
    }
}