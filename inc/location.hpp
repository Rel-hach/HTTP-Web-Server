#ifndef LOCATION_HPP
# define LOCATION_HPP
#include <iostream>
#include <string>
#include <vector>
class location
{
public:
	location();
	location(location&);
	location(const location &);
	location&operator=(const location&);
	~location();
 
	std::string uri;
	std::vector<std::string> allow_methods;
	std::string cgi_path;
	std::string autoindex;
	std::string return_code;
protected:
private:

};
#endif
