#include "./cgi.hpp"

cgi::cgi()
{
}

cgi::~cgi()
{
}

cgi::cgi(std::string body, std::string respense, std::string flag){
    this->body=body;
    this->respense=respense; // amine=raqube
    this->flag=flag;
}
std::string cgi::getbody() const{
    return this->body;
}
std::string cgi::getrespense() const{
    return this->respense;
}
std::string cgi::getflag() const{
    return this->flag;
}
void cgi::setbody(std::string const body){
    this->body=body;
    
}
void cgi::setrespense(std::string const respense){
    this->respense=respense;
}
void cgi::setflag(std::string const flag){
    this->flag=flag;

}