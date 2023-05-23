#include "request.hpp"

    request::request()
    {
        this->position  = 0;
    }

    request::~request() {}

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


    bool            request::Processing_HttpRequest( void )
    {
        VEC_OF_STRS vec = Splitting_string (http_request, CRLFX2);
        position = vec.at(0).find("\r\n");
        if (position not_eq std::string::npos)
        {
            if (Checking_startLine(vec.at(0).substr(0, position)) == KO) return KO; 
            if (Checking_headers( vec.at(position + SKIP_CRLF)) == KO) return KO;
            if (Analysing_userRequest() == KO) return (KO);
        }
    }

    VEC_OF_STRS     request::Splitting_string(std::string str, std::string delim)
    {
        std::vector<std::string> requests;
        while ( (position = str.find(delim)) != std::string::npos )
        {
            requests.push_back(str.substr(0, position));
            str.erase(0, position + sizeof(delim));
        } 
        requests.push_back(str.substr(0));
        return (requests);
    }

    // START LINE CHECKING

    bool            request::Checking_startLine( std::string startLine )
    {
        std::vector<std::string> vec = Splitting_string(start_line, " ");
        std::vector<std::string>::iterator it;
        if (vec.size() != 3)
            return (false);
        method      = vec.at(0);
        path        = vec.at(1); // uri
        http_ver    = vec.at(2);

        bool    Method = Checking_methodIfSupported();
        bool    Version = Checking_httpVersion();
        if (Method && Version && path[0] == '/')
            return (OK);
        else
            return (KO);
    }

    bool        request::Checking_methodIfSupported( void )
    {

        if (method == "GET" || method == "POST" || method == "HEAD") return (OK);
        
        else    return (KO);
    }

    bool        request::Checking_httpVersion( void )
    {
        if (http_ver.compare("HTTP/1.1")) return (OK);
        
        else    return (KO);
    }

    // HEADERS CHECKING 
    bool            request::Checking_headers( std::string heads)
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
                return (KO);
        }
        return (OK);
    }

    bool        request::Checking_neededHeadersToStore( std::string key, std::string val )
    {
        if (key == "Host" || key == "Content-Type" ||
            key == "Content-Lenght" || key == "Transfer-Encoding")
            return (OK);
        return (KO);
    }

    // Knowing what user asking for

    bool        request::Analysing_userRequest()
    {
        setting_host( this->dictionary );
        setting_contentLenght( this->dictionary );
        setting_transferEncoding( this->dictionary );
        setting_contentType( this->dictionary );

        if (host != NOT_FOUND && method == GET) // and path exits in config file.
            Executing_GetCase();
        else if ((host != NOT_FOUND) and (contentLenght != NOT_FOUND) and (method == POST))
            Executing_PostCase();
        else if (host != NOT_FOUND && method == DELETE)
            Executing_delete_case();
    }

    // GET

    bool        request::Executing_GetCase()
    {

    }

    // file case | folder case
 
    bool        request::Executing_PostCase()
    {

    }

    bool        request::Executing_delete_case()
    {

    }

    //
    bool        request::Checking_body()
    {

    }

    // bool            Checking_ContentLenght();


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

    void request::setting_contentType( MAP_OF_VECS& dictionary )
    {
        MAP_OF_VECS::iterator it;
        it = dictionary.find(CONTENT_TYPE);
        if (it != dictionary.end())
            this->contentType = it->second;
        else
            this->contentType = NOT_FOUND;
    }

    



    /*
        
    */