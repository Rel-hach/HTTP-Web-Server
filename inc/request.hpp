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

#include "parse_config.hpp"
#include "status_codes.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>


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

typedef struct  data
{
  std::vector<std::string>            contents;
  size_t                              Lenght;
  std::map<std::string, std::string>  types;
} f_data;



// to be deleted after 
class client
{
    private:
        std::string _request;
    std::string     _response;
    public:
    bool            _responseIsReady;
        std::string     getreq() const
        {
            return (_request);
        }
};

class request
{
  public:
    // request
    std::string _request;
    std::string _body;
    std::vector<std::string> _headers;
    std::map<std::string, std::string>    _mapOfHeaders; 
  
    // start line header :
    std::string _method;
    std::string _path;
    std::string _version;
    std::string _querry;
  
    // headers :
    // HOST : GET | DELETE -> required.
    long long   _maxBodySize;
    std::string _host;
    std::string _hostname;
    std::string _cookie;
    int         _port;
    int         _status;

    // CONTENT_TYPE IS REQUIRED IN POST
    std::string _transferEncoding;
    long long   _contentLength;
    std::string _contentType;
    std::string _boundry; // multipart/form-data

    // ACCEPT : type client are waiting for.
    std::string _accept;
    // CONNECTION : close or keep-alive
    std::string _connection;




    public:

      bool  _isCgi; 
      bool  _isRedirect;
    

    // Headers :

    bool  _isChunked;
    bool  _isMultipart;
    bool  _hasHostHeader;
    bool  _hasContentLenght;


    // -------------------------- METHODS 



    // constructors : 


      request () {}
      request(Server& conf);


      // processing :

      int   processing_request( client client,  Server& serv );
      int   checking_startLine( std::string sline );
      int   checking_method(std::string& sline);
      int   checking_version(std::vector<std::string> headers );
      int   checking_uri();
      int   checking_headers( std::vector<std::string> headers );
      int   checking_headerByHeader(std::string& key, std::string& value);
      void  checking_connectionType( std::string& connectionType );



      // setters and getters 

      // void  settingMethod( std::string& method);
      // void  settingPath( std::string& path);
      // void  settingVersion( std::string& version);
      // void  settingQuerry( std::string& querry);
      // void  settingHostName( std::string& hostName) ;
      // void  settingPort( int port);
      // void  settingContentLength( long long contentLength);
      // void  settingBoundry( std::string boundary);
      // void  settingAccept( std::string accept);
      // void  settingConnection( std::string connection);

      // std::string  gettingMethod() const;
      // std::string  gettingPath() const;
      // std::string  gettingVersion() const;
      // std::string  gettingQuerry() const;
      // std::string  gettingHostName() const;
      // std::string  gettingPort() const;
      // std::string  gettingContentLength() const;
      // std::string  gettingBoundry() const;
      // std::string  gettingAccept() const;
      // std::string  gettingConnection() const;
};


#endif