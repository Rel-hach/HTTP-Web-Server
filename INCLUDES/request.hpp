/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rel-hach <rel-hach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 17:10:47 by rel-hach          #+#    #+#             */
/*   Updated: 2023/05/23 17:10:48 by rel-hach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>

#define OK                  true
#define KO                  false
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
        std::string                         path;
        std::string                         start_line;
        std::string                         headers;

        //-------------------------------------------

        std::string                         host;
        std::string                         contentType;
        std::string                         contentLenght;
        std::string                         transferEncoding;
        size_t                              position;
        VEC_OF_STRS                         Headers;
        MAP_OF_VECS                         dictionary;
    public:
        request();
        ~request();
        bool            Processing_HttpRequest( void );
        VEC_OF_STRS     Splitting_string(std::string str, std::string delim);
        // START LINE CHECKING
        bool            Checking_startLine( std::string startLine );
        bool            Checking_methodIfSupported( void );
        bool            Checking_httpVersion( void );
        // HEADERS CHECKING 
        bool            Checking_headers( std::string heads);
        bool            Checking_neededHeadersToStore( std::string key, std::string val );
        // Knowing what user asking for
        bool            Analysing_userRequest();
        // GET
        bool            Executing_GetCase();
        // file case | folder case 
        bool            Executing_PostCase();
        //
        bool            Checking_body();
        // bool            Checking_ContentLenght();
        bool            Executing_delete_case();
        // 


        // GETTERS AND SETTERS :

        std::string getting_host() const;
        std::string getting_contentLenght() const;
        std::string getting_transferEncoding() const;
        std::string getting_contentType() const;

        void setting_host( MAP_OF_VECS& dictionary );
        void setting_contentLenght( MAP_OF_VECS& dictionary );
        void setting_transferEncoding( MAP_OF_VECS& dictionary );
        void setting_contentType( MAP_OF_VECS& dictionary );
};

#endif