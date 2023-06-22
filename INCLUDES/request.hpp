/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rel-hach <rel-hach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 17:10:47 by rel-hach          #+#    #+#             */
/*   Updated: 2023/05/30 00:01:45 by rel-hach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "status_codes.hpp"


#define CRLFX2              "\r\n\r\n"
#define VEC_OF_STRS         std::vector<std::string>
#define MAP_OF_VECS         std::map<std::string, std::string>
#define  SUPPORTED_HTTP_VER "HTTP/1.1"
#define SKIP_CRLF           2
#define CRLFX2              "\r\n\r\n"

#define HOST                "Host"
#define CONTENT_LENGHT      "Content-Lenght"
#define CONTENT_TYPE        "Content-Type"
#define TRANSFER_ENCODING   "Transfer-Encoding"
#define NOT_FOUND           "NOT_FOUND"
#define GO_NEXT             1

// METHODS

#define GET     "GET"
#define DELETE  "DELETE"
#define POST    "POST"


class Server ;
class request
{
  public:
    std::string     _request;
    std::string     _HeadRequest;
    std::string     _BodyRequest;

    std::string     _method;
    std::string     _path;
    std::string     _httpVer;
    std::string     _querry;

    std::string     _response;
    std::string     _connectionType;
    std::vector<std::string>     _headers;
    std::map<std::string, std::string> _mapHeaders;

    Server          conf;

    bool            _ischunked;
    bool            _requestIsParsed;
    bool            _hasContentLength;
    bool            _BodyIsFullyReceived;
    bool            _isReceived;
    bool            _errorFound;

    long long       _bytesToRead;
    long long       _readBytes;
    long long       _contentLength;
    int             _connFd;
    int             _status;

    // methods :
                                request(int fd, Server& conf);
    void                        Storing_convenientServerInfos(Server& config);
    int                         Processing_HttpRequest();
    int                         Checking_startLine(std::string sline);
    int                         Checking_headers(std::string headers);
    int                         Checking_uri(std::string& uri);
    int                         counting_Directories (std::string& uri);
    void                        unchunking();
    void                        checking_connectionType();
    void                        checking_chunkedOrhasContentLength();

        // GETTERS AND SETTERS :

        // std::string     getting_host() const;
        // std::string     getting_contentLenght() const;
        // std::string     getting_transferEncoding() const;
        // std::string     getting_contentType() const;

        // void            setting_host( MAP_OF_VECS& dictionary );
        // void            setting_contentLenght( MAP_OF_VECS& dictionary );
        // void            setting_transferEncoding( MAP_OF_VECS& dictionary );
        // void            setting_contentType( MAP_OF_VECS& dictionary );
};

#endif