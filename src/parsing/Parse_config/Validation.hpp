#ifndef VALIDATION_HPP
# define VALIDATION_HPP
#include <exception>
#include <iostream>
#include <sstream>
class Validation
{
public:
	class InvalidKeyException: public std::exception{
		public :
			const char * what() const _NOEXCEPT{
				return "Error: invalid key";
			}
	};
	class InvalidValueException: public std::exception{
		public :
			const char * what() const _NOEXCEPT{
				return "Error: invalid value";
			}
	};
	static void server_valid_key(std::string &key){
		if (key == "host" || key == "port" || key == "server_name" || key == "root" || key == "index" || key == "autoindex" || key == "upload_path" || key == "error_page" || key == "methods" || key == "cgi_extensions" || key == "client_max_body_size")
			return;
		throw InvalidKeyException();
	}

	static void trim(std::string &line)
	{
		if (line.empty())
			return;
		size_t start = line.find_first_not_of(" \t");
		size_t end = line.find_last_not_of(" \t");
		line = line.substr(start, end - start + 1);
	}

	static void split(std::string &line, std::string &key, std::string &value){
		char eq;
		std::stringstream ss(line);
		ss >> key;
		ss >> eq;
		std::getline(ss, value);
		trim(value);
	}

	static void server_valid_value(std::string &value){
		if (value.empty())
			throw InvalidValueException();
	}
	static void error_page_valid_key(std::string &key){
		int key_int;
		key_int = std::stoi(key);
		if (key.find_first_not_of("0123456789", 0) == std::string::npos || key.empty() || key_int < 400 || key_int > 599)
			throw InvalidKeyException();
	} 
	static void error_page_valid_value(std::string &value){
		if (value.empty() || value[0] != '"' || value[value.size() - 1] != '"')
			throw InvalidValueException();
	}
protected:
private:

};
#endif
