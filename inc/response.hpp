#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <string>
#include <vector>
#include "request.hpp"


class Location
{
    public:
        Location();
        Location(std::string Name, std::vector<std::string>& Index, std::string Homepage, std::string Root, std::vector<std::string>& Methods, bool redirec)
        {
            this->locationName  = Name;
            this->index = Index;
            this->homePage = Homepage;
            this->root = Root;
            this->methods = Methods;
            this->redirection = redirec;
            this->autoIndex = "OFF";

        }
        std::string                 locationName;
        std::string                 homePage;
        std::vector<std::string>    index;
        std::vector<std::string>    methods;             
        std::string                 root;
        std::string                 redirectionPath;
        bool                        isUpload;
        std::string                 autoIndex;
        bool                        redirection;
        size_t                      clientMaxBodySize; 
        std::string                 cgiExtention;
};

class response
{
    public:
        // config
        std::string                 _homePage;
        std::vector<std::string>    _index;
        std::vector<std::string>    _methods;             
        std::string                 _root;
        std::string                 _autoIndex;
        bool                        _redirection;
        size_t                      _clientMaxBodySize;
        std::string                 _realPath;
        std::string                 _cgiExtention;
        std::string                 _fileName;
    public:
        std::string _contentType;
        bool        _method_isFound;
        std::string _bheaders;
        std::string _fileContent;
        std::string _method;
        std::string _path;
        size_t      _contentLength;
        std::string _referer;
        std::string _body;
        std::string _extension;
        std::string _locationName;
        int         _status;
        int         _respCode;
        std::string _responseCode;
        std::string _respHeaders;
        std::string _redirectionPath;
        std::string _uploadLocation;
        std::string _boundry;
        std::map<int, std::string> _errorPages;
        bool        _iscgi;
        bool        _ischunked;
        bool        _isMultipart;
        bool        _isupload;
        // locations 
        // supportedMethods;


    public:
                response();

        void    store_requestInfos(request& req);
        bool    method_isFound();
        int     executing_method (); // executing_method ();
        bool    permissionForReading();
        bool    permissionForExecuting();
        bool    permissionForDeleting();
        int     reading_fileContent();
        int     listing_requestDirectory();
        int     stroring_requestBody();
        int     storing_multipleParts();
        void    determine_contentType();
        void    preparing_responseHeaders();
        void    getting_responsCode();
        std::string    generating_response(request& request);
        void    get_pathAndLocationInfos(std::map<std::string, Location> locations, std::string uri);
        std::string getErrorPage();
        std::string readPage(std::string page);
};


#endif
