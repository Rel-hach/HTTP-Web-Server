#include "../INCLUDES/request.hpp"
#include "status_codes.hpp"

    request::request()
    {
        this->position  = 0;
    }

    request::~request() {}

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

    /*

        HTTP STATUS CODES :

        200 OK

        201 OK created

        301 moved to new URL

        304 Not modified

        400 Bad request

        401 Unauthorized

        404 Not found

        500 Internal server error

    */

// -----------------------------------------------------------------------

    int            request::Processing_HttpRequest( void )
    {
        VEC_OF_STRS vec = Splitting_string (http_request, CRLFX2);
        position = vec.at(0).find("\r\n");
        if (position not_eq std::string::npos)
        {
            if ( status == Checking_startLine(vec.at(0).substr(0, position)) != GO_NEXT)
                return status;
            if (Checking_headers( vec.at(position + SKIP_CRLF)) == 1) return 1;
            if (Analysing_userRequest() == 1) return (1);
        }
    }

// -----------------------------------------------------------------------

// ----------------------------------------------------------------------

    // START LINE CHECKING

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

        /*
            checking the URI ??
        */

        return (GO_NEXT);
    }

    int    request::Converting_hexaToDecimal ( std::string str )
    {
        int len = str.length();
        
        int base = 1;  

        int decimalVal = 0;  
    
        for (int i = len - 1; i >= 0; i--) 
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

        }
    }

    int         request::LookingFor_uriInConfFile()
    {
        // check if the uri in CONF file.

        // algorithms :

        /*
            for example uri = a/b/c 
            step 1 : get the /c and see if you can find it 
            if no ?

            step 2 : get the /b and see if you can find it

            step 3 : get the /a and see if you can find it

            if you find it ? then -> ignore it. delete it 

            just 
        */
    }

    int         request::Checking_uri()
    {
        position = uri.find('?');
        if (position != std::string::npos)
        {
            querry = uri.substr(position + 1);
            uri = uri.substr(0, position);
        }

        if (uri[0] != '/') //  || uri.find("&#%<>" the Uri must start with a / and doesn't have to contain those chars "&#%<>"
            return (Bad_Request);

        Decoding_url();

            return (Not_Found);
        return (GO_NEXT);
    }

    int        request::Checking_methodIfSupported( void )
    {

        if (method == "GET" || method == "POST" || method == "HEAD")
            return (GO_NEXT);
        return (Method_Not_Allowed);
    }

    void    Checking_uriIsThere( void )
    {
        
    }

    int        request::Checking_httpVersion( void )
    {
        if (http_ver.compare("HTTP/1.1"))
            return (GO_NEXT);
        else
            return (HTTP_Version_Not_Supported);
    }

    // HEADERS CHECKING 
    int            request::Checking_headers( std::string heads)
    {
        Headers = Splitting_string(heads, "\r\n");
        VEC_OF_STRS::iterator it;
        for (it = Headers.begin(); it != Headers.end(); ++it)
        {
            std::pair<std::string, std::string> couple;
            int position = it->find(":");
            if (position != std::string::npos)
            {
                couple.first = it->substr(0, position);
                couple.second = it->substr(position + 2);
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

    int        request::Checking_neededHeadersToStore( std::string key, std::string val )
    {
        // here we can add more headers .
        if (key == "Host" || key == "Content-Type" ||
            key == "Content-Lenght" || key == "Transfer-Encoding")
            return (YES);
        return (NO);
    }

    // Knowing what user asking for

    int        request::Analysing_userRequest()
    {
        setting_host( this->dictionary );
        setting_contentLenght( this->dictionary );
        setting_transferEncoding( this->dictionary );
        setting_contentType( this->dictionary );

        if (host != NOT_FOUND and method == GET) // && 'uri' ---> exits in config file.
            Executing_GetCase();
        else if ( (host not_eq NOT_FOUND) and (contentLenght != NOT_FOUND) and (method == POST) )
            Executing_PostCase();
        else if (host not_eq NOT_FOUND and method == DELETE)  // && 'uri' to be deleted exist in config file ---> 
            Executing_delete_case();

        // then do a small checker function to check if everything is okay ? then execute 
        /*
            CASE 1 : GET
                - Check if the file exit ? & host
                - 


        */
    }

    // GET

    int    get_path()
    {

    }

    void    request::check_request()
    {

    }

    int        request::Executing_GetCase()
    {
        // Let's check if it's a valid request. 
    }

    // file case | folder case
 
    int        request::Executing_PostCase()
    {

    }

    int        request::Executing_delete_case()
    {

    }

    //
    int        request::Checking_body()
    {

    }

    // int            Checking_ContentLenght();


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

    



    /*
        
    */