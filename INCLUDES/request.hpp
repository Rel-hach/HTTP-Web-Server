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

#define YES                 1
#define NO                  0
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



class request
{
    private:
        std::string                         http_request;
        std::string                         method;
        std::string                         http_ver;
        std::string                         uri;
        std::string                         start_line;
        std::string                         headers;
        std::string                         querry;


        // ConfigFile data

        std::string                         root;
        VEC_OF_STRS                         locations;
        

        //-------------------------------------------

        std::string                         host;
        std::string                         contentType;
        std::string                         contentLenght;
        std::string                         transferEncoding;
        size_t                              position;
        int                                 status;
        VEC_OF_STRS                         Headers;
        MAP_OF_VECS                         dictionary;
    public: 
        request();  
        ~request(); 
        int              Processing_HttpRequest( void );
        VEC_OF_STRS      Splitting_string(std::string str, std::string delim);
        void             check_request( void );
        // START LINE CHECKING
        int             Checking_startLine ( std::string startLine );
        int             Checking_methodIfSupported ( void );
        int             Checking_httpVersion ( void );
        int             Checking_uri ( void );
        int             Decoding_url ( void );
        int             counting_Directories (std::string& uri);
        int             Converting_hexaToDecimal ( std::string str );

        int             LookingFor_uriInConfFile();
        // HEADERS CHECKING 
        int             Checking_headers( std::string heads);
        int             Checking_neededHeadersToStore( std::string key, std::string val );
        // Knowing what user asking for
        int             setting_headers();
        int             executing_request();


        // GETTERS AND SETTERS :

        std::string     getting_host() const;
        std::string     getting_contentLenght() const;
        std::string     getting_transferEncoding() const;
        std::string     getting_contentType() const;

        void            setting_host( MAP_OF_VECS& dictionary );
        void            setting_contentLenght( MAP_OF_VECS& dictionary );
        void            setting_transferEncoding( MAP_OF_VECS& dictionary );
        void            setting_contentType( MAP_OF_VECS& dictionary );
};

#endif