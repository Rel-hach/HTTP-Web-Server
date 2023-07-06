#include "../inc/request.hpp"
#include "../inc/tools.hpp"


int   request::processing_request( client& client,  Server& serv )
{
     std::string request = client.getreq();
        // if the request is empty or has spaces at its start, return a bad request.
        if (request.empty() || isspace(request[0]))
          return (Bad_Request);

        std::vector<std::string> tokens;

        _headers.clear();
        _body.clear();

        // split the request with "/r/n/r/n" to get headers and body.
        // if there is a body the split function will return 2 the number of tokens [headers] - [request].
        if (tools::splitting_string(request, "\r\n\r\n", tokens) == 2)
          _body.assign(tokens[1]);

        // split the headers with "\r\n" to get a vector of string headers.
        if (tools::splitting_string(tokens[0], "\r\n", _headers) > 1)
        {
            // here check if the start line is valid.
            if ((_status = checking_startLine(_headers[0])) != GO_NEXT)
                return (_status);
            
            // here check if the headers are valid.
            if ((_status = checking_headers(_headers)) != GO_NEXT)
                return (_status);
            
        }
        else
            return (Bad_Request);

}


    int   request::checking_startLine( std::string sline )
    {
        // if the line is empty or has a space at the beginning.
        if (sline.empty() || isspace(sline[0]))
            return (Bad_Request);

        std::vector<std::string> tokens;

        // split the line with spaces to start line components.
        if (tools::splitting_string(sline, " ", tokens) != 3)
            return (Bad_Request);
        
        _method     = tokens[0];
        _path       = tokens[1];
        _version    = tokens[2];


        // those methods have not been implemented.
        if (_method == "HEAD" || _method == "PATCH" || _method == "PUT" || _method == "OPTIONS")
            return (Not_Implemented);

        // something else means not a method, return bad request.
        if (_method != "GET" && _method != "DELETE" && _method != "POST")
            return (Bad_Request);

        // check if the url has a slash at the beginning. and see if there is a '..'
        if (_path[0] != '/' || _path.find("..") != std::string::npos)
            return (Bad_Request);
        
        // the size of the request shouldn't be more than 2048 chars long. 
        if (_path.size() > 2048)
            return (URI_Too_Long);

        // check if there is a ? if yes : then split the url into [querry] & [path].
        int position = _path.find('?');
        if (position != std::string::npos)
        {
            _querry = _path.substr(position + 1);
            _path   = _path.substr(0, position);
        }
        // compare the version of HTTP ! it should be HTTP/1.1
        if (_version.compare(SUPPORTED_HTTP_VER) != 0)
            return (HTTP_Version_Not_Supported);

        return (GO_NEXT);
    }



    int   request::checking_headers( std::vector<std::string> headers )
    {
        std::vector<std::string>::iterator it;
        int   statuuus = GO_NEXT;
        for (it = this->_headers.begin(); it != this->_headers.end(); ++it)
        {
            std::pair<std::string, std::string> couple;
            size_t position = it->find(":");
            if (position != std::string::npos)
            {
                couple.first = it->substr(0, position);
                couple.second = it->substr(position + 2);
                tools::trimming_string(couple.first);
                tools::trimming_string(couple.second);
                if ((statuuus = checking_headerByHeader(couple.first,couple.second)) != GO_NEXT)
                  return (statuuus);
                this->_mapOfHeaders.insert(couple);
            }
            else
                return (Bad_Request);
        }

        // if (_hasHostHeader == false)
        //   return (Bad_Request);

        // if (_method == "POST")
        // {
        //   if (_isChunked == false && _hasContentLenght == false)
        //       return (Lengh_Required);

        //   if (_isChunked != false && _hasContentLenght != false)
        //       return (Bad_Request);
        // }
        // return (GO_NEXT);
    }


        int    request::checking_headerByHeader(std::string& key, std::string& value)
    {
        // the host contains the hostname and the port of the server, the client want to connect to
        if (key == "" || value == "")
            return Bad_Request;

        if ((key.find_first_of(" \t") != std::string::npos))
            return Bad_Request;
        

        // if ((value.find_first_of(" \t") != std::string::npos))
        //     return Bad_Request;

        if (key == "Host")
        {
            int position = value.find(':');
            if (position != -1 && !_hasHostHeader)
            {
                _host = value;
                _hostname = _host.substr(0, position);
                std::string port = _host.substr(position + 1);
                if (port.size() > 5 || port.find_first_not_of("0123456789") != std::string::npos)
                    return (Bad_Request);

                if (std::stoul(port) <= 65535 && std::stoul(port) > 0)
                    _port = atoi(port.c_str());
                else
                    return (Bad_Request);
                
                _hasHostHeader = true;
            }
            else
                return (Bad_Request);
        }

        
        // connection type - close or keep-alive
        if (key == "Connection")
            _connection = value;

        // transfer-encoding and chunked then ischunked else NOT_IMPLEMENTED
        if (key == "Transfer-Encoding" && value == "chunked")
        {
            if (_isChunked == false)
                _isChunked = true;
            else
                return (Bad_Request);
        }
        else
            return (Not_Implemented);


        if (key == "Content-Length")
        {
            if (_hasContentLenght || (value.empty() || value.find_first_not_of("0123456789") != std::string::npos))
                return (Bad_Request);
            _contentLength = atoll(value.c_str());
            _hasContentLenght = true;
        }

        if (key == "Cookie")
            _cookie = value;

        // content type => multipart/form-data or application/x-www-form-urlencoded else not supported.
        if (key == "Content-Type")
        {
            int multilpart = value.find("multipart/form-data");
            int app = value.find("application/x-www-form-urlencoded");

            if (multilpart == -1 && app == -1)
                return (Not_Implemented);
            
            if (_isMultipart || _isCgi)
                return (Bad_Request);

            int position = value.find(';');
            if (position != -1)
            {
                _contentType = value.substr(0, position);
                if ((position = value.find("boundry=")) != -1)
                    _boundry = value.substr(position + 9);

                if (_boundry == "" && _contentType == "multipart/form-data")
                    return (Bad_Request);

                else if (_contentType == "application/x-www-form-urlencoded")
                    _isCgi = true;
                
                else
                    _isMultipart = true;
            }
        }
        return (GO_NEXT);
    }