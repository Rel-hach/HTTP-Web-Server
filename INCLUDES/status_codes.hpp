#ifndef STATUS_CODES_HPP
#define STATUS_CODES_HPP


#define Continue 100
#define Switching_protocols

/* Successful : Everything is normal */ 

#define  OK 200     // Everything is normal 
#define  Created 201
#define  Accepted 202
#define  Non_Authoritative_Information 203
#define  No_Content 204
#define  Reset_Content 205
#define  Partial_Content 206

/* Redirection : */

#define Multiple_Choices 300
#define Moved_Permanently // Update your URL, this has moved for good. 301
#define Found 302 
#define See_Other 303
#define Not_Modified 304
#define Use_Proxy 305
#define Unused 306
#define Temporary_Redirect 307 

/* Client Error */

#define Bad_Request 400
#define Unauthorized 401
#define Payment_Required_Not_used_really 402
#define Forbidden_Server_refuses_to_give_you_a_file_authentication_won_t_help 403
#define Not_Found 404
#define Method_Not_Allowed 405
#define Not_Acceptable 406
#define Proxy_Authentication_Required 407
#define Request_Timeout_Browser_took_too_long_to_request_something 408
#define Conflict 409
#define Gone 410
#define Lengh_Required 411
#define Precondition_Failed 412
#define Reqeust_Entity_Too_Large 413
#define URI_Too_Long 414
#define Request_Range_Not_Satisfiable 415
#define Expectation_Failed 416

/* Server Error */

#define Internal_Server 500
#define Not_Implemented 501
#define Bad_Gateway 502
#define Service_Unavailable_Too_busy_to_respond_to_a_client 503
#define Gateway_Timeout 504
#define HTTP_Version_Not_Supported 505


// error pages :


char Badreq[] = "<HTTP/1.1 400 \r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title> Bad Request : 400 </title><style>body{font-family: Arial, sans-serif;background-color: #f7f7f7;}.container{margin: 50px auto;text-align: center;}h1{color: #d0021b;font-size: 36px;margin-bottom: 20px;}p{font-size: 18px;margin-bottom: 20px;}button{background-color: #d0021b;color: #fff;border: none;padding: 10px 20px;font-size: 18px;border-radius: 5px;cursor: pointer;}button:hover {background-color: #a30014;}</style></head><body><div class='container'><h1>Bad Request</h1><p>Sorry, we were unable to process your request.</p></div></body></html>"; 


char unauthorized[] = "<HTTP/1.1 401 \r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title> Unauthorized : 401 </title><style>body{font-family: Arial, sans-serif;background-color: #f7f7f7;}.container{margin: 50px auto;text-align: center;}h1{color: #d0021b;font-size: 36px;margin-bottom: 20px;}p{font-size: 18px;margin-bottom: 20px;}button{background-color: #d0021b;color: #fff;border: none;padding: 10px 20px;font-size: 18px;border-radius: 5px;cursor: pointer;}button:hover {background-color: #a30014;}</style></head><body><div class='container'><h1>Unauthorized</h1><p>Sorry, you are not authorized.</p></div></body></html";


char not_found[] = "<HTTP/1.1 404 \r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title> Not Found : 404 </title><style>body{font-family: Arial, sans-serif;background-color: #f7f7f7;}.container{margin: 50px auto;text-align: center;}h1{color: #d0021b;font-size: 36px;margin-bottom: 20px;}p{font-size: 18px;margin-bottom: 20px;}button{background-color: #d0021b;color: #fff;border: none;padding: 10px 20px;font-size: 18px;border-radius: 5px;cursor: pointer;}button:hover {background-color: #a30014;}</style></head><body><div class='container'><h1>Not Found</h1><p>Sorry, not found.</p></div></body></html";


char method_notAllowd[] = "<HTTP/1.1 405 \r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title> Not Allowd : 405 </title><style>body{font-family: Arial, sans-serif;background-color: #f7f7f7;}.container{margin: 50px auto;text-align: center;}h1{color: #d0021b;font-size: 36px;margin-bottom: 20px;}p{font-size: 18px;margin-bottom: 20px;}button{background-color: #d0021b;color: #fff;border: none;padding: 10px 20px;font-size: 18px;border-radius: 5px;cursor: pointer;}button:hover {background-color: #a30014;}</style></head><body><div class='container'><h1>Not Allowed</h1><p>Sorry, method is not allowed.</p></div></body></html";


char tooLong[] = "<HTTP/1.1 414 \r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title> Too Long Uri : 414 </title><style>body{font-family: Arial, sans-serif;background-color: #f7f7f7;}.container{margin: 50px auto;text-align: center;}h1{color: #d0021b;font-size: 36px;margin-bottom: 20px;}p{font-size: 18px;margin-bottom: 20px;}button{background-color: #d0021b;color: #fff;border: none;padding: 10px 20px;font-size: 18px;border-radius: 5px;cursor: pointer;}button:hover {background-color: #a30014;}</style></head><body><div class='container'><h1>Too Long Uri</h1><p>Sorry, uri is too long.</p></div></body></html";


char httpVersionErr[] = "<HTTP/1.1 505 \r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title> Not Supported : 505 </title><style>body{font-family: Arial, sans-serif;background-color: #f7f7f7;}.container{margin: 50px auto;text-align: center;}h1{color: #d0021b;font-size: 36px;margin-bottom: 20px;}p{font-size: 18px;margin-bottom: 20px;}button{background-color: #d0021b;color: #fff;border: none;padding: 10px 20px;font-size: 18px;border-radius: 5px;cursor: pointer;}button:hover {background-color: #a30014;}</style></head><body><div class='container'><h1>Not Supported</h1><p>Sorry, version (HTTP) not supported.</p></div></body></html";


char timeOut[] = "<HTTP/1.1 504 \r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title> Time Out : 504 </title><style>body{font-family: Arial, sans-serif;background-color: #f7f7f7;}.container{margin: 50px auto;text-align: center;}h1{color: #d0021b;font-size: 36px;margin-bottom: 20px;}p{font-size: 18px;margin-bottom: 20px;}button{background-color: #d0021b;color: #fff;border: none;padding: 10px 20px;font-size: 18px;border-radius: 5px;cursor: pointer;}button:hover {background-color: #a30014;}</style></head><body><div class='container'><h1>Timeout</h1><p>Sorry, Gateway timeout.</p></div></body></html";


char internal_error[] = "<HTTP/1.1 500 \r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title> Internal server issue : 500 </title><style>body{font-family: Arial, sans-serif;background-color: #f7f7f7;}.container{margin: 50px auto;text-align: center;}h1{color: #d0021b;font-size: 36px;margin-bottom: 20px;}p{font-size: 18px;margin-bottom: 20px;}button{background-color: #d0021b;color: #fff;border: none;padding: 10px 20px;font-size: 18px;border-radius: 5px;cursor: pointer;}button:hover {background-color: #a30014;}</style></head><body><div class='container'><h1>Internal issue</h1><p>Sorry, there is an internal server issue.</p></div></body></html";


char getTest[]  = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World! I AM THE GET METHOD</h1></body></html>";


#endif