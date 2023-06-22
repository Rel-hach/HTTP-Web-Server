#include "../INCLUDES/request.hpp"


    int    allowed_chars(char c)
    {
        std::string allowd_ch = "abcdefghijklmnopqrstuvwxyzABCDEF\
        GHIJKLMNOPQRSTUVWXYZ0123456789!$&'()*+,/:;=?@[]-._~";
        if (allowd_ch.find(c) != std::string::npos)
            return (GO_NEXT);
        else
            return (400);

        return (GO_NEXT);
    }


// --------------------------------------------------------------------


    int    Converting_hexaToDecimal ( std::string str )
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


    int            Decoding_url (std::string& uri)
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


    // -------------------------------------------------- UTILS



// utils


    int normalizing_uri(std::string& uri)
    {
        size_t position = uri.find("//");

        while (position != std::string::npos)
        {
            uri.erase(position, 1);
            position = uri.find("//");
        }
        return (1);
    }

    // ------------------------------------------------


    VEC_OF_STRS             Splitting_string(std::string str, std::string delim)
    {
        size_t position = 0;

        // GET / HTTP/1.1
        VEC_OF_STRS splitted;
        while ( (position = str.find(delim)) != std::string::npos )
        {
            splitted.push_back(str.substr(0, position));
            str.erase(0, position + delim.size());
        }
        splitted.push_back(str.substr(0));
        return (splitted);
    }


// -----------------------------------


    request::request(Server& conf)
    {
        _request = "";
        _HeadRequest = "";
        _BodyRequest = "";

        _method = "";
        _path = "";
        _httpVer = "";
        _querry = "";
        _response = "";

        _status = 1;
        _isReceived = false;
        _ischunked = false;
        _requestIsParsed = false;
        _hasContentLength = false;
        _BodyIsFullyReceived = false;
        _errorFound = false;

        _bytesToRead = 0;
        _readBytes = 0;
        Storing_convenientServerInfos(conf);
    }

    void    request::Storing_convenientServerInfos(Server& config)
    {
        conf.port = config.port;
        conf.autoindex = config.autoindex;
        conf.host = config.host;
        conf.root = config.root;
        conf.map_of_error_page = config.map_of_error_page;
        conf.server_name = config.server_name;
        conf.upload_path = config.upload_path;
        conf.vec_of_locations = config.vec_of_locations;
        conf.client_max_body_size = config.client_max_body_size;
        conf.serverChunk = config.serverChunk;
    }



// -----------------------------------


    int     request::Processing_HttpRequest()
    {
        size_t position = this->_HeadRequest.find("\r\n");

        _status = GO_NEXT;
        if (position not_eq std::string::npos)
        {
            if ((Checking_startLine(_HeadRequest.substr(0, position)) != GO_NEXT))
                return (_status);

            if ((_status == Checking_headers(this->_HeadRequest.substr(position + SKIP_CRLF))) != GO_NEXT)
                return (_status); 

            checking_chunkedOrhasContentLength();
            checking_connectionType();
        }
        return (GO_NEXT);
    }


// ------------------------------------------------------------------

int                         request::Checking_startLine(std::string sline)
{
        int retStatus;

        std::cout << sline << std::endl;
        VEC_OF_STRS vec = Splitting_string(sline, " ");
        VEC_OF_STRS::iterator it;
        if (vec.size() != 3) return (Bad_Request);
        this->_method = vec.at(0);
        this->_path = vec.at(1);
        this->_httpVer = vec.at(2);
        std::cout << "path :" << vec.at(1) << std::endl;


        if (this->_method != "GET" && this->_method != "POST" && this->_method != "DELETE")
            return (Method_Not_Allowed);
        
        if (this->_httpVer != SUPPORTED_HTTP_VER)
            return (HTTP_Version_Not_Supported);

        if ((retStatus = Checking_uri(_path)) != GO_NEXT)
            return (retStatus);

        return (GO_NEXT);   
}


// ----------------------------------------------------------------------


    int         request::Checking_uri(std::string& uri)
    {
        int status = GO_NEXT; 
        size_t position = uri.find('?');
        if (position != std::string::npos)
        {
            _querry = uri.substr(position + 1);
            uri = uri.substr(0, position);
        }

        if (uri[0] != '/')
            return (Bad_Request);

        status = Decoding_url( uri );
        if (status != GO_NEXT) return (Bad_Request);

        if (uri.size() > 2048)
            return (URI_Too_Long);

        if (counting_Directories(uri) == Bad_Request)
            return (Bad_Request);
        
        
        return (GO_NEXT);
    }
 

    // -----------------------------------------------

    /*

    Having multiple slashes in a URI (path) can cause issues with how the URI is 
    interpreted and resolved by servers and clients. 
    This is because the presence of multiple slashes can create ambiguity
    about the structure of the URI, making it difficult to determine the intended resource.

    For example, consider the URI "example.com//path/to/resource".
    This URI contains two slashes between the domain name and the start of the path component, 
    which may be interpreted differently by different servers and clients. Some servers and clients may treat this as a 
    relative URI with an empty path component, while others may interpret it as an absolute URI 
    with a double slash indicating the root directory.

    To avoid these kinds of ambiguities, it is generally recommended to normalize URIs 
    by removing any unnecessary or redundant slashes. This can be done using a simple string 
    manipulation function that replaces consecutive slashes with a single slash.
    
    */


    // ------------------------------------------------

    int     request::counting_Directories (std::string& uri)
    {
        normalizing_uri(uri);
        std::vector<std::string> paths = Splitting_string(uri, "/");
        int count = 0;

        // /root/aaa/bbb/cccc /../../aaa 
        
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



    // ------------------------------------------------------- UTILS




    // --------------------------------------------- UTILS



    // ---------------------------------------------------------


    int     request::Checking_headers(std::string heads)
    {
        this->_headers = Splitting_string(heads, "\r\n");
        VEC_OF_STRS::iterator it;
        for (it = this->_headers.begin(); it != this->_headers.end(); ++it)
        {
            std::pair<std::string, std::string> couple;
            size_t position = it->find(":");
            if (position != std::string::npos)
            {
                couple.first = it->substr(0, position);
                couple.second = it->substr(position + 2);
                if (!couple.first[0] || !couple.second[0])
                    return (Bad_Request);
                this->_mapHeaders.insert(couple);
            }
            else
                return (Bad_Request);
        }

        return (GO_NEXT);
    }

    // ------------------------------------------------------------ 


void        request::checking_chunkedOrhasContentLength()
{
    int nline;
    std::string contentLength;
    std::stringstream iss;

    size_t pos = this->_HeadRequest.find("Transfer-Encoding: chunked");
    if (pos != std::string::npos)
        this->_ischunked = true;

    pos = this->_HeadRequest.find("Content-Length: ");
    if (pos != std::string::npos)
    {
        nline = this->_HeadRequest.find('\n', pos);
        contentLength = this->_HeadRequest.substr(pos + 16, nline);
        iss << contentLength;
        iss >> this->_contentLength;
        this->_bytesToRead = _contentLength + this->_HeadRequest.length();
        this->_hasContentLength = true;
    }
    
    if (_ischunked && _hasContentLength)
        _errorFound = true;
    if (_method != POST && (_ischunked || _hasContentLength))
        _errorFound = true;
}

// -----------------------------------------


void    request::checking_connectionType()
{
    size_t ret1 = this->_HeadRequest.find("Connection: keep-alive");
    size_t ret2 = this->_HeadRequest.find("Connection: close");

    if (ret1 != std::string::npos)
        _connectionType = "keep-alive";
    else if (ret2 != std::string::npos)
        _connectionType = "close";
    else
         _connectionType = "keep-alive";
}



// ------------------------------------------------------------------


void request::unchunking()
{
    std::ostringstream oss;

    std::istringstream iss(_request);
    std::string line;
    while (std::getline(iss, line))
    {
        long size = strtol(line.c_str(), NULL, 16);
        if (size == 0)
        {
            _BodyIsFullyReceived = true;
            break;
        }
        std::string chunk__request(size, ' ');
        if (!iss.read(&chunk__request[0], size))
            break;
        oss << chunk__request;

        iss.ignore(2);

        _request.erase(0, line.length() + size + 4);
    }
    this->_BodyRequest += oss.str();
    oss.clear();
}