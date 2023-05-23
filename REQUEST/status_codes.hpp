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

#define Bad_Request_Server_did_not_understand_the_URL_you_gave_it. 400
#define Unauthorized_Must_be_authenticated 401
#define Payment_Required_Not_used_really 402
#define Forbidden_Server_refuses_to_give_you_a_file_authentication_won_t_help 403
#define Not_Found_A_file_doesn_t_exist_at_that_address 404
#define Method_Not_Allowed 405
#define Not_Acceptable 406
#define Proxy_Authentication_Required 407
#define Request_Timeout_Browser_took_too_long_to_request_something 408
#define Conflict 409
#define Gone 410
#define Lengh_Required 411
#define Precondition_Failed 412
#define Reqeust_Entity_Too_Large 413
#define Unsupported_Media_Type 414
#define Request_Range_Not_Satisfiable 415
#define Expectation_Failed 416

/* Server Error */

#define Internal_Server_Error_Something_on_the_server_didn_t_work_right. 500
#define Not_Implemented 501
#define Bad_Gateway 502
#define Service_Unavailable_Too_busy_to_respond_to_a_client 503
#define Gateway_Timeout 504
#define HTTP_Version_Not_Supported 505


#endif