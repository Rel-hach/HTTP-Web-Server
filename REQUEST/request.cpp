#include "../INCLUDES/request.hpp"
#include "status_codes.hpp"

    request::request()
    {
        this->position  = 0;
    }

    request::~request() {}


// split 

    VEC_OF_STRS     request::Splitting_string(std::string str, std::string delim)
    {
        VEC_OF_STRS splitted;
        while ( (position = str.find(delim)) != std::string::npos )
        {
            splitted.push_back(str.substr(0, position));
            str.erase(0, position + sizeof(delim));
        } 
        splitted.push_back(str.substr(0));
        return (splitted);
    }


/*--------------------------------------    START OF THE PROCESS :  ------------------------------------------*/


    int            request::Processing_HttpRequest( void )
    {
        VEC_OF_STRS vec = Splitting_string (http_request, CRLFX2);
        position = vec.at(0).find("\r\n");
        if (position not_eq std::string::npos)
        {
            if ( status == Checking_startLine(vec.at(0).substr(0, position)) != GO_NEXT)
                return status;
            if (Checking_headers( vec.at(position + SKIP_CRLF)) != GO_NEXT)
                return (status);
            if (Analysing_userRequest() != GO_NEXT)
                return (status);
        }
    }

/*--------------------------------------- CHECKING THE FIRST LINE OF THE REQUEST -----------------------------------------*/


    int            request::Checking_startLine( std::string startLine )
    {
        VEC_OF_STRS vec = Splitting_string(start_line, " ");
        VEC_OF_STRS::iterator it;
        if (vec.size() != 3) return (Bad_Request);
        method      = vec.at(0);
        uri         = vec.at(1);
        http_ver    = vec.at(2);

        // checking if the Method is supported ..

        status = Checking_methodIfSupported();
        if (status != GO_NEXT)   return (Method_Not_Allowed);

        // checking if the HTTP version is supported ..

        status = Checking_httpVersion();
        if (status != GO_NEXT)   return (HTTP_Version_Not_Supported);

        status = Checking_uri();
        if (status != GO_NEXT) return (status);

        return (GO_NEXT);
    }


// ---------------------------------FUNCTION : CONVERTING TO HEXADECIMAL -----------------------------------------------




    int    request::Converting_hexaToDecimal ( std::string str )
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

    int    allowed_chars(char c)
    {
        std::string allowd_ch = "abcdefghijklmnopqrstuvwxyzABCDEF\
        GHIJKLMNOPQRSTUVWXYZ0123456789!$&'()*+,/:;=?@[]-._~";
        if (allowd_ch.find(c) != std::string::npos)
            return (GO_NEXT);
        else
            (Bad_Request);
    }

// ---------------------------------FUNCTION : DECODING URI -----------------------------------------------

    int            request::Decoding_url ()
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
    }

// ---------------------------------FUNCTION : LOOK AFTER URI IN CONFIG FILE -----------------------------------------------



    // std::vector<std::map<std::string, std::map<std::string, std::string>>> VectorOfservers;

    // int         request::LookingFor_uriInConfFile()
    // {
    //     VEC_OF_STRS splitted;
    //     int i = 0;
    //     int count = std::count(uri.begin(), uri.end(), '/');
    //     splitted = Splitting_string(uri, "/");
    //     if (count > 1)
    //     {
    //         for (i = 0; i < count; i++)
    //             splitted[i].insert(splitted[i].begin(), '/');
    //     }
    //     return (0);
    // }


// ---------------------------------FUNCTION : GET QUERRY FROM URI -----------------------------------------------


    int         request::Checking_uri()
    {
        int status = GO_NEXT; 
        position = uri.find('?');
        if (position != std::string::npos)
        {
            querry = uri.substr(position + 1);
            uri = uri.substr(0, position);
        }

        if (uri[0] != '/')
            return (Bad_Request);

        status = Decoding_url();
        if (status != GO_NEXT) return (Bad_Request);

        if (uri.size() > 2048)
            return (URI_Too_Long);

        // status = LookingFor_uriInConfFile();
        // if (status != GO_NEXT) return (Not_Found);

            return (Not_Found);
        return (GO_NEXT);
    }


/* ------------------------------------------------------------------------------------------------------------------   */

    int get_uri_location()
    {

    }

/*-----------------------------------FUNCTION: CHECKING IF METHOD SUPPORTED---------------------------------------------*/

               
    int        request::Checking_methodIfSupported( void )
    {

        if (method == "GET" || method == "POST" || method =="DELETE")
            return (GO_NEXT);
        return (Method_Not_Allowed);
    }



/*---------------------------------FUNCTION: CHECKING HTTP VERSION-----------------------------------------------*/



    int        request::Checking_httpVersion( void )
    {
        if (http_ver.compare("HTTP/1.1"))
            return (GO_NEXT);
        return (HTTP_Version_Not_Supported);
    }


/*----------------------------------FUNCTION : CHECKING HEADERS----------------------------------------------*/


    int            request::Checking_headers( std::string heads)
    {
        Headers =   Splitting_string(heads, "\r\n");
        VEC_OF_STRS::iterator it;
        for (it = Headers.begin(); it != Headers.end(); ++it)
        {
            std::pair<std::string, std::string> couple;
            int position = it->find(":");
            if (position != std::string::npos)
            {
                couple.first = it->substr(0, position);
                couple.second = it->substr(position + 2);
                if (!couple.first[0] || !couple.second[0])
                    return (Bad_Request);
                if (Checking_neededHeadersToStore(couple.first, couple.second) == OK)
                    dictionary.insert(couple);
                else
                    continue;
            }
            else
                return (Bad_Request);
        }
        return (OK);
    }


// ---------------------------------FUNCTION : CHECKING IF THE HEADER WORTH TO BE STORED--------------------------------


    int        request::Checking_neededHeadersToStore( std::string key, std::string val )
    {
        // here we can add more headers .
        if (key == "Host" || key == "Content-Type" ||
            key == "Content-Lenght" || key == "Transfer-Encoding")
            return (YES);
        return (NO);
    }



// ------------------------------------------------------------------------------------------------------


    int        request::Analysing_userRequest()
    {
        setting_host( this->dictionary );
        setting_contentLenght( this->dictionary );
        setting_transferEncoding( this->dictionary );
        setting_contentType( this->dictionary );

        if ((this->transferEncoding != "not_found") && (this->transferEncoding != "chunked"))
            return (Bad_Request);
        

        // if (host != NOT_FOUND and method == GET) // && 'uri' ---> exits in config file.
        //     Executing_GetCase();
        // else if ( (host not_eq NOT_FOUND) and (contentLenght != NOT_FOUND) and (method == POST) )
        //     Executing_PostCase();
        // else if (host not_eq NOT_FOUND and method == DELETE)  // && 'uri' to be deleted exist in config file ---> 
        //     Executing_delete_case();
    }


// --------------------------------------------------------------------------------


    std::string request::getting_host() const
    {
        return (host);
    }

    std::string request::getting_contentLenght() const
    {
        return (contentLenght);
    }

    std::string request::getting_transferEncoding() const
    {
        return (transferEncoding);
    }

    std::string request::getting_contentType() const
    {
        return (contentType);
    }

    void request::setting_host( MAP_OF_VECS& dictionary )
    {
        MAP_OF_VECS::iterator it;
        it = dictionary.find(HOST);
        if (it != dictionary.end())
            this->host = it->second;
        else
            this->contentLenght = NOT_FOUND;
    }

    void request::setting_contentLenght( MAP_OF_VECS& dictionary )
    {
        MAP_OF_VECS::iterator it;
        it = dictionary.find(CONTENT_LENGHT);
        if (it != dictionary.end())
            this->contentLenght = it->second;
        else
            this->contentLenght = NOT_FOUND;
    }

    void request::setting_transferEncoding( MAP_OF_VECS& dictionary )
    {
        MAP_OF_VECS::iterator it;
        it = dictionary.find(TRANSFER_ENCODING);
        if (it != dictionary.end())
            this->transferEncoding = it->second;
        else
            this->transferEncoding = NOT_FOUND;
    }

    void request::setting_contentType( MAP_OF_VECS& HEADERS )
    {
        MAP_OF_VECS::iterator it;
        it = HEADERS.find(CONTENT_TYPE);
        if (it != HEADERS.end())
            this->contentType = it->second;
        else
            this->contentType = NOT_FOUND;
    }



    /* ------------------------------------------------------------------------ */
