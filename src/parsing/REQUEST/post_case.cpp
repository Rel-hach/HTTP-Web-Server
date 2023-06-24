#include "../../../inc/request.hpp"
#include "../../../inc/status_codes.hpp"


int    checking_uploadIsAuthorized()
{
    return (GO_NEXT);
}

int     retrieving_specifiedPort()
{
    int Port = 1202;
    return (Port);
}

int checking_methodIsAllowed()
{
    return (GO_NEXT);
}

int     checking_maxBodyNotExceeded()
{
    if (1 < 5)
        return (Reqeust_Entity_Too_Large);
    else
        return (GO_NEXT);
}

int     checking_DataFromConfigFile()
{
    int Port = retrieving_specifiedPort();

    if (checking_uploadIsAuthorized() != GO_NEXT)
        return (Unauthorized);
        
    if (checking_methodIsAllowed() != GO_NEXT)
        return (Method_Not_Allowed);
    
    if (checking_maxBodyNotExceeded() != GO_NEXT)
        return (Reqeust_Entity_Too_Large);
}

int     checking_ifCgiIsEnabled()
{
    if (2 > 1)
        return (YES);
    else
        return (NO);
}


void     performing_postCgiCase()
{
    // cgi 
}

void     performing_Post()
{
    // upload 
}

int    handling_postCase()
{
    int status = GO_NEXT;
    if (status = checking_DataFromConfigFile() != GO_NEXT)
        return (status);
    
    if (checking_ifCgiIsEnabled() == YES)
        performing_postCgiCase();
    else
        performing_Post();
    
    return (Created);
}
