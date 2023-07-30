#include "../../inc/server.hpp"
#include "../../inc/server_data.hpp"
#include "../../inc/request.hpp"
#include "../../inc/client.hpp"
#include "../../inc/tools.hpp"
#include "../../inc/response.hpp"

// #ifndef __IFDEFINED
// #define __IFDEFINED 1
// #define if(x) if((printf("BEFORE: LINE: %i, FILE: %s\n", __LINE__, __FILE__)) && (x) && (printf("AFTER: LINE: %i, FILE: %s\n", __LINE__, __FILE__)))
// #endif



std::vector<std::string>  find_server_name(client client, std::string port)
{
    std::vector<std::string> clean_name;
    int index = client.server_name.find(":");
    if(index !=(int) std::string::npos)
    {
        tools::splitting_string(client.server_name, ":", clean_name);
        if(clean_name[0].length() && clean_name[1].length())
            return clean_name;
    }
    clean_name.push_back(client.server_name);
    clean_name.push_back(port);
    return clean_name;
}

server_data find_server(std::vector<server_data>& servers,std::vector<server_data>& deplicate, client client)
{
    server_data server_exicte;
    std::vector<std::string> server_name =find_server_name(client,std::to_string(servers[0].port[0]));
    std::cout<<server_name[0]<<server_name[1]<<std::endl;
    for (size_t i = 0; i < servers.size(); i++)
    {

        if(std::to_string(servers[i].port[0]) == server_name[1] && servers[i].server_names[0] == server_name[0])
        {
            server_exicte=servers[i];
            return server_exicte;
        }
    }
    for (size_t i = 0; i < deplicate.size(); i++)
    {
        if(std::to_string(deplicate[i].port[0]) == server_name[1] && deplicate[i].server_names[0] == server_name[0])
        {
            server_exicte=deplicate[i];
            return server_exicte;
        }
    } 
    for (size_t i = 0; i < servers.size(); i++)
    {
        if(std::to_string(servers[i].port[0]) == server_name[1])
        {
            server_exicte=servers[i];
            return server_exicte;
        }
    }
    server_exicte = servers[0];
    return server_exicte;
}
std::vector<server_data> filter_server(std::vector<server_data>& servers) {
    std::vector<server_data> deplicate;
    for (size_t i = servers.size(); i > 0; i--) {
        bool is_duplicate = false;
        for (size_t j = i - 1; j > 0; j--) {
            if (i != j && servers[i - 1].port == servers[j - 1].port && servers[i - 1].host == servers[j - 1].host) {
                is_duplicate = true;
                break;
            }
        }
        if (is_duplicate) {
            deplicate.push_back(servers[i - 1]);
            servers.erase(servers.begin() + i - 1);
        }
    }
    return deplicate;
}
int main(int argc,char **argv) 
{   
    std::vector<server_data> servers;
	try
	{
		if (argc != 2)
			throw std::invalid_argument("Error: invalid number of arguments");
		servers = parse_server(argv[1]);
        std::vector<server_data> deplicate = filter_server(servers); 
        std::vector<pollfd> all_df;
        std::vector<server> all_server;
        std::vector<client> all_client;
        std::vector<int> fd_server;
        std::string req;
        std::vector<int> port_bind;
        //staart server and bind and lesten
        for (size_t i = 0; i < servers.size(); i++)
        {
            for (size_t j = 0; j < servers[i].port.size(); j++)
            {
                std::vector<int>::iterator it = std::find(port_bind.begin(), port_bind.end(), servers[i].port[i]); 
                if(it == fd_server.end())
                {
                    server Server = server(servers[i].port[i],servers[i].host);
                    all_server.push_back(Server);
                    if(Server.startServer())
                        return 1;
                    struct pollfd fds;
                    fds.fd= Server.getSockert();
                    fds.events = POLLIN;
                    all_df.push_back(fds);
                    fd_server.push_back(Server.getSockert());
                    if(Server.bindServer())
                        return 1;
                    if(Server.listenSrver())
                        return 1;
                    std::cout<<servers[i].port[i]<<std::endl;
                    port_bind.push_back(servers[i].port[i]);
                }   
            }
        }
        signal(SIGPIPE, SIG_IGN);
        while (true)
        {
            int rev =  poll(&all_df[0], all_df.size(),-1);
            if (rev == -1) {
                std::cerr << "Error: Polling failed\n";
                return 1;
            }
            for (size_t i = 0; i < all_df.size(); i++)
            {
                if (all_df[i].revents  & POLLHUP) {
                    
                    for (size_t j = 0; j < all_client.size(); j++)
                    {
                        if(all_client[j].getfd() == all_df[i].fd)
                        {
                                close(all_df[i].fd);
                                all_df.erase(all_df.begin() + i);
                                all_client.erase(all_client.begin() + j);         
                                break; 
                        }
                    }        
                } 
                else if (all_df[i].revents & POLLIN ) {
                    std::vector<int>::iterator it = std::find(fd_server.begin(), fd_server.end(), all_df[i].fd); 
                    if (it != fd_server.end()) {
                        int client_socket = accept(all_df[i].fd, (sockaddr*) &all_server[std::distance(fd_server.begin(), it)].getClientAdtess(), &all_server[std::distance(fd_server.begin(), it)].getClientAdtessSize()); 
                        if (client_socket == -1)
                        {
                            std::cerr << "Error: client connection failed\n";
                            close(all_df[i].fd); 
                            return 1;
                        }
                        // fcntl(client_socket, F_SETFL, O_NONBLOCK); // commanted by ahabachi
                        struct pollfd fds;
                        fds.fd=  client_socket;
                        fds.events = POLLIN;
                        all_df.push_back(fds);
                        all_client.push_back(client(fds.fd));
                        std::cout<< "server ==> "<< all_df[i].fd << "   accept connection cleient ==> " << client_socket<<std::endl;
                    }
                    else
                    {
                        char buff[1024]; 
                        std::memset(buff, '\0', sizeof(buff));
                        int content = read(all_df[i].fd,buff,1024);
                        if (content == -1) {
                            std::cerr << "Error: read failed\n";
                            return 1;
                        }
                        for (size_t j = 0; j < all_client.size(); j++)
                        {
                            if(all_client[j].getfd() == all_df[i].fd)
                            {
                                all_client[j].setservr_name(buff);
                                all_client[j].appendreq(buff,content);
                                all_client[j].addTocontentread(content);
                                if(( all_client[j].ischunked && all_client[j].getreq().find("\r\n0\r\n\r\n") != std::string::npos)
                                    || (all_client[j].getreq().length() && !all_client[j].ischunked && all_client[j].getcontentlenght() <= all_client[j].getcontentread()))
                                {
                                    all_df[i].events = POLLOUT;
                                }
                            }
                        }
                    }
                }
              
                else if(all_df[i].revents & POLLOUT)
                { 
                    size_t j = 0;
                    for (; j < all_client.size(); j++)
                    {
                        if(all_client[j].getfd() == all_df[i].fd)
                        {
                            break;
                        }
                    }
                        if(!all_client[j].proccessing)
                        {
                            request req;
                            server_data ser=find_server(servers, deplicate, all_client[j]);
                            int ret_status = req.processing_request(all_client[j], ser );
                            if (all_client[j]._requestIsParsed == true)
                            {
                                response resp;
                                all_client[j]._response = resp.generating_response(req, ret_status, ser);
                            }
                            all_client[j].proccessing=true;
                        }
                        int buffwrite = 0;
                        if(all_client[j].sendLenth < (long)all_client[j]._response.length())
                        {
                            if( (long)all_client[j]._response.length() - all_client[j].sendLenth < 10240)
                                buffwrite =  (long)all_client[j]._response.length() - all_client[j].sendLenth;
                            else
                                buffwrite = 1024;
                                int res = write(all_df[i].fd, all_client[j]._response.c_str() + all_client[j].sendLenth, buffwrite);
                                if(res == -1)
                                {
                                        std::cerr << "Error: write failed\n";
                                        return 1;
                                }
                            all_client[j].sendLenth+=buffwrite;
                        }
                        else
                        {
                                close(all_df[i].fd);
                                all_df.erase(all_df.begin() + i);
                                all_client.erase(all_client.begin() + j);         
                        } 
                        continue;      
                }
                 else if (all_df[i].revents  & POLLERR) {
                    std::cerr << "Error on socket " << all_df[i].fd << std::endl;
                    close(all_df[i].fd);
                    all_df.erase(all_df.begin() + i);
                }  else if (all_df[i].revents  & POLLNVAL) {
                    close(all_df[i].fd);
                    all_df.erase(all_df.begin() + i);
                }
            }
        }
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
    return 0;
}

