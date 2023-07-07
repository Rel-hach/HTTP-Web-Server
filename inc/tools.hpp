#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "status_codes.hpp"
#include <iostream>
#include <vector>
#include <map>
#define GO_NEXT 1

class tools
{
    public:
        static void    trimming_string(std::string& str)
        {
            str.erase(0, str.find_first_not_of(" \t"));
        
            if (str == "")
                return ;
        
            str.erase(str.find_last_not_of(" /t") + 1);
        }

        static int splitting_string(const std::string& str, const std::string& delim, std::vector<std::string>& tokens) 
        {
            size_t pos = 0;
            while ((pos = str.find(delim)) != std::string::npos)
            {
                tokens.push_back(str.substr(0, pos));
                str.erase(0, pos + delim.length());
            }
            if (!str.empty())
                tokens.push_back(str);
            return (tokens.size());
        }

        static int    allowed_chars(char c)
        {
            std::string allowd_ch = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!$&'()*+,/:;=?@[]-._~";

            if (allowd_ch.find(c) != std::string::npos)
                return (GO_NEXT);
            else
                return (400);

            return (GO_NEXT);
        }


// -----------------------------------------------------------------------

        // this function convert a string that has a hexadecimal value to decimal.

        static int    Converting_hexaToDecimal ( std::string str )
        {
            int lenght = str.length();
            
            int base = 1;  

            int decimalVal = 0;  
        
            for (int i = lenght - 1; i >= 0; i--) 
            {
                if (str[i] >= '0' && str[i] <= '9') 
                {
                    decimalVal += (str[i] - 48) * base;
                    base = base * 16;
                }
                else if (str[i] >= 'A' && str[i] <= 'F') 
                {
                    decimalVal += (str[i] - 55) * base;
                    base = base * 16; 
                }
            }
            return (decimalVal);
        }


    // -----------------------------------------------------------------------

        // this function is used to decode the url.
        // it checks if there is a '%' in the uri.
        // then convert the number after from hexadecimal to decimal.
        // if it's asccii then it remove the % and hexa number and
        // replace it with it's real ascci value. 
        // for examle if we have %20 this will be replaced with a space ' '

        static int            Decoding_url (std::string& uri)
        {
            int i = -1;
            int decimal;
            while (uri[++i])
            {
                if (uri[i] == '%')
                {
                    decimal = Converting_hexaToDecimal( uri.substr(i + 1, 2));
                    if (isascii(decimal))
                    {
                        uri.erase(i, 2);
                        uri[i] = decimal;
                    }
                    else
                        return (Bad_Request);
                }
                if (!isascii(uri[i]))
                    return (Bad_Request);
                if (allowed_chars(uri[i]) != GO_NEXT)
                    return(Bad_Request);
            }
            return (GO_NEXT);
        }


    // -----------------------------------------------------------------------

        // this function is about removing multiple slashes from the uri.
        // this will normalize the uri and make it valid to use.

        static int normalizing_uri(std::string& uri)
        {
            size_t position = uri.find("//");

            while (position != std::string::npos)
            {
                uri.erase(position, 1);
                position = uri.find("//");
            }
            return (1);
        }


    // --------------------------------------------------------------

        // this function is about counting the number of directories in the uri.
        // [if] the number is negativ then it returns bad request.
        // [else] it returns the number of directories.

        static int     counting_Directories (std::string& uri)
        {
            normalizing_uri(uri);
            std::vector<std::string> paths;
            splitting_string(uri, "/", paths);
            int count = 0;

            for (size_t i = 0; i < paths.size(); i++)
            {
                if (paths[i] == "..")
                    count--;
                else
                    count++;
            }
            if (count < 0)
                return (Bad_Request);
            return (count);
        }


};



#endif