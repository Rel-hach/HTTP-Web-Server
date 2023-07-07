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

        this->_isChunked = false;
        this->_hasContentLenght = false;
        this->_isMultipart = false;
        this->_isRedirect = false;
        this->_isCgi = false;
    }

    int   request::processing_request( client& clientt, server& serv )
    {
        (void)serv;
        std::string request = clientt.getreq();
        // if the request is empty or has spaces at its start, return a bad request.
        if (request.empty() || isspace(request[0]))
        {
            clientt._response = tools::getting_errorPage(Bad_Request);
            clientt._response_isReady = true;   return _status;
        }

        std::vector<std::string> tokens;

        _headers.clear();
        _body.clear();

        // split the request with "/r/n/r/n" to get headers and body.
        // if there is a body the split function will return 2 the number of tokens [headers] - [request].
        if (tools::splitting_string(request, "\r\n\r\n", tokens) == 2)
          _body.assign(tokens[1], tokens[1].length());

        // split the headers with "\r\n" to get a vector of string headers.
        // in HTTP/1.1 the minimum number of headers is 2 [start_line] [host]
        if (tools::splitting_string(tokens[0], "\r\n", _headers) > 1)
        {
            // here check if the start line is valid.
            if ((_status = checking_startLine(_headers[0])) != GO_NEXT)
            {
                clientt._response = tools::getting_errorPage(_status);
                clientt._response_isReady = true;   return _status;
            }
            
            
            // here check if the headers are valid.
            if ((_status = checking_headers(_headers)) != GO_NEXT)
            {
                clientt._response = tools::getting_errorPage(_status);
                clientt._response_isReady = true;   return _status;
            }   
        }
        else
        {
                clientt._response = tools::getting_errorPage(Bad_Request);
                clientt._response_isReady = true;   return _status;
        }

        // if (_method == POST)
        // {
        //     // if (_contentLength > serv.clienMaxBody)
        //     //     return (Reqeust_Entity_Too_Large);
        //     if ((_body.length() != _contentLength )|| _body.empty())
        //         return (Bad_Request);
            
        //     f_data file;

        //     if (_isMultipart)
        //     {
        //         if (_body.find(_boundry) == std::string::npos)
        //             return (Bad_Request);
        //         tools::splitting_string(_body, _boundry, file.contents);
        //         // creating_contentsFiles ( file.contents );
        //     }
        //     else if (_isCgi)
        //     {
        //         // handle_cgi(_body);
        //     }
        // }
        if (_method == GET)
        {
            clientt._response = tools::generateHtmlPage();
            clientt._response_isReady = true;   return (GO_NEXT);
        }

        return (GO_NEXT);
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

        if (_path[0] != '/')
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
                tools::trimming_string(couple.first);
                tools::trimming_string(couple.second);
                if ((statuuus = checking_headerByHeader(couple.first,couple.second)) != GO_NEXT)
                    return (statuuus);
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
            else
                return (Bad_Request);
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
                _contentType = value;
            
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

    bool request::handling_chunked()
    {
        std::string temp(_body);
        _body.clear();
        std::vector<std::string> parts;

        tools::splitting_string(temp, "\r\n", parts);
        temp.clear();
        int size = -1;
        for (size_t i = 0; i < parts.size(); i+=2)
        {
            size = tools::Converting_hexaToDecimal(parts[i]);
            if (static_cast<size_t>(size) == parts[i + 1].size())
                _body += parts[i + 1];
            else
                return (false);
        }
        if (size != 0)
            return (false);
        return (true);
    }