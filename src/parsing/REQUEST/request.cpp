#include"../../../inc/request.hpp"
#include"../../../inc/tools.hpp"


// -------------------------------------------------------------------------

    request::request ()
    {
        this->_method = "";
        this->_path = "";
        this->_version = "";
        this->_querry = "";
        this->_maxBodySize = -1;
        this->_host = "";
        this->_hostname = "";
        this->_cookie = "";
        this->_port = 80;
        this->_status = GO_NEXT;
        this->_referer = "";

        this->_isChunked = false;
        this->_hasContentLenght = false;
        this->_isMultipart = false;
        this->_isRedirect = false;
        this->_isCgi = false;
    }

    int   request::processing_request( client& clientt, server_data & serv )
    {
        (void)serv;

        std::string request = clientt.getreq();


        // if the request is empty or has spaces at its start, return a bad request.
        if (request.empty() || isspace(request[0]))
        {
             return (clientt._requestIsParsed = true, Bad_Request);
        }

        std::vector<std::string> tokens;

        _headers.clear();
        _headersBody.clear();
        _body.clear();

        int i = 0;
        if ((i = tools::splitting_string(request, "\r\n\r\n", tokens)) > 1)
        {
            std::cout << "_request LENGHT : " << request.length() << std::endl;
            _body = request.substr(request.find("\r\n\r\n") + 4);
            std::cout << "_body LENGHT : " << _body.length() << std::endl;

        }
        
        if (tools::splitting_string(tokens[0], "\r\n", _headers) > 1)
        {
            // here check if the start line is valid.
            if ((_status = checking_startLine(_headers[0])) != GO_NEXT)
            {
                return (clientt._requestIsParsed = true, _status);
            }
            
            if ((_status = checking_headers(_headers)) != GO_NEXT)
            {
                return (clientt._requestIsParsed = true, _status);
            }   
        }
        else
        {
            return (clientt._requestIsParsed = true, _status);
        }

        if (_method == POST)
        {
            // if (_contentLength > serv.clienMaxBody)
            //     return (Reqeust_Entity_Too_Large);
            // if ((_body.length() != _contentLength ) || _body.empty())
            //     return (clientt._requestIsParsed = true, Bad_Request);
        }
        std::cout << _status << std::endl;
        return (clientt._requestIsParsed = true, GO_NEXT);
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

        
        // the size of the request shouldn't be more than 2048 chars long. 
        if (_path.size() > 2048)
            return (URI_Too_Long);

        // check if there is a ? if yes : then split the url into [querry] & [path].
        size_t position = _path.find('?');
        if (position != std::string::npos)
        {
            _querry = _path.substr(position + 1);
            _path   = _path.substr(0, position);
        }

        if (_path.empty() || _path[0] != '/')
            return (Bad_Request);
        
        if (_path.find("..") != std::string::npos)
            return (Bad_Request);

        // compare the version of HTTP ! it should be HTTP/1.1
        if (_version.compare(SUPPORTED_HTTP_VER) != 0)
            return (HTTP_Version_Not_Supported);

        return (GO_NEXT);
    }


    int   request::checking_headers( std::vector<std::string> headers )
    {
        std::vector<std::string>::iterator it;
        int   statuuus = GO_NEXT;
        for (it = headers.begin() + 1; it != headers.end(); ++it)
        {
            std::pair<std::string, std::string> couple;
            size_t position = it->find(":");
            if (position != std::string::npos)
            {
                couple.first = it->substr(0, position);
                couple.second = it->substr(position + 2);
                tools::trimming_string(couple.second);
                if ((statuuus = checking_headerByHeader(couple.first,couple.second)) != GO_NEXT)
                {
                    return (statuuus);
                }
                this->_mapOfHeaders.insert(couple);
            }
            else
                return (Bad_Request);
        }

        if (_hasHostHeader == false)
          return (Bad_Request);

        if (_method == "POST")
        {
          if (_isChunked == false && _hasContentLenght == false)
              return (Lengh_Required);

          if (_isChunked != false && _hasContentLenght != false)
              return (Bad_Request);
        }
        return (GO_NEXT);
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
        }

        // connection type - close or keep-alive
        if (key == "Connection")
            _connection = value;

        // transfer-encoding and chunked then ischunked else NOT_IMPLEMENTED
        if (key == "Transfer-Encoding")
        {
            if (_isChunked == false && value == "chunked")
            {
                if (handling_chunked())
                    _isChunked = true;
                else
                   return (Bad_Request);
            }
            else
                return (Not_Implemented);
        }

        if (key == "Content-Length")
        {
            if (_hasContentLenght || (value.empty() || value.find_first_not_of("0123456789") != std::string::npos))
                return (Bad_Request);
            _contentLength = std::stoul(value);
            std::cout << "CONTENT-LENGTH : " << _contentLength << std::endl;
            _hasContentLenght = true;
        }

        if (key == "Cookie")
            _cookie = value;

        if (key == "Referer")
        {
            value = value.substr(value.find_first_of("0123456789"));
            if (!value.empty() && value.find('/') != std::string::npos)
            {
                value = value.substr(value.find('/'));
                _referer.assign(value);
            }
        }
     
        // content type => multipart/form-data or application/x-www-form-urlencoded else not supported.
        if (key == "Content-Type")
        {
            int multilpart = value.find("multipart/form-data");
           
            if (multilpart == -1)
            {
                _contentType = value;
                return (GO_NEXT);
            }
            
            int position = value.find(';');

            if (position != -1)
            {
                _contentType = value.substr(multilpart, position);
                int boundpos;
                if ((boundpos = value.find("boundary="), position) != -1)
                    _boundry = value.substr(boundpos + 9);
                
                if (_boundry == "" && _contentType == "multipart/form-data")
                    return (Bad_Request);
                else
                    _isMultipart = true;
            }
        }
        return (GO_NEXT);
    }


    bool    request::check_hexadecimalLine(std::string& line)
    {
        int found = line.find_first_not_of("0123456789ABCDEFabcdef");
        if (found != -1)
            return false;
        return (true);
    }


    bool    request::chunk_ending_correctly( std::string end )
    {
        if (end == "\r\n")
            return (true);
        else
            return (false);
    }

    bool    request::handling_chunked()
    {

        std::vector<std::string> tokens;
        _unchunked_body = "";
        size_t start = 0;
        int n = _body.find("\r\n", start);
        std::string temp;
        std::string end;
        while (n != -1)
        {
            std::string hexa = _body.substr(start, n - start);
            if (check_hexadecimalLine(hexa) == false)
                return false;
            size_t size = std::strtol(hexa.c_str(), NULL, 16);
            if (size == 0)
                break ;
            temp = _body.substr(n + 2, size);
            end = _body.substr(n + 2 + size, 2);
            if (temp.size() == size && chunk_ending_correctly(end) == true)
            {
                _unchunked_body += temp;
                start += hexa.size() + temp.size() + 4;
            }
            else
                return (false);
            n = _body.find("\r\n", start);
        }
        return true;
    }
