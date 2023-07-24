#include "../../inc/server.hpp"
#include "../../inc/server_data.hpp"
#include "../../inc/request.hpp"
#include "../../inc/client.hpp"
#include "../../inc/tools.hpp"
#include "../../inc/response.hpp"

int main(int argc,char **argv) 
{
    (void) argc;
    (void)argv;
    // int size = 0;÷
    // std::vector<server_data> servers;
	// try
	// {
	// 	if (argc != 2)
	// 		throw std::invalid_argument("Error: invalid number of arguments");
	// 	servers = parse_server(argv[1]);
	// }
	// catch(const std::exception& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// }


    std::vector<pollfd> all_df;
    std::vector<server> all_server;
    std::vector<client> all_client;
    std::vector<int> fd_server;
    std::string req;

    // create number of server
    server Server = server(8000);
    server Server2 = server(8001);
    server Server3 = server(8002);
    server Server4 = server(8003);
    all_server.push_back(Server);
    all_server.push_back(Server2);
    all_server.push_back(Server3);
    all_server.push_back(Server4);


    //staart server and bind and lesten
    for (size_t i = 0; i < all_server.size(); i++)
    {
        if(all_server[i].startServer())
            return 1;
        struct pollfd fds;
        fds.fd= all_server[i].getSockert();
        fds.events = POLLIN;
        all_df.push_back(fds);
        fd_server.push_back(all_server[i].getSockert());
        // i need bind of all port
        if(all_server[i].bindServer())
            return 1;
        if(all_server[i].listenSrver())
            return 1;
    }
    // pooling and accept connection and parsing requist
    signal(SIGPIPE, SIG_IGN);
    while (true)
    {
        int rev =  poll(&all_df[0], all_df.size(), 1);
        if (rev == -1) {
            std::cerr << "Error: Polling failed\n";
            return 1;
        }
        for (size_t i = 0; i < all_df.size(); i++)
        {
            if (all_df[i].revents & POLLIN ) {
                std::vector<int>::iterator it = std::find(fd_server.begin(), fd_server.end(), all_df[i].fd); 
                if (it != fd_server.end()) {
                    int client_socket = accept(all_df[i].fd, (sockaddr*) &all_server[std::distance(fd_server.begin(), it)].getClientAdtess(), &all_server[std::distance(fd_server.begin(), it)].getClientAdtessSize()); 
                    if (client_socket == -1)
                    {
                        std::cerr << "Error: client connection failed\n";
                        close(all_df[i].fd);
                        return 1;
                    }
                    struct pollfd fds;
                    fds.fd=  client_socket;
                    fds.events = POLLIN;
                    all_df.push_back(fds);
                    client newclient = client(fds.fd);
                    newclient._serverIndex = i;
                    all_client.push_back(newclient);
                    std::cout<< "server ==> "<< all_df[i].fd << "   accept connection cleient ==> " << client_socket<<std::endl;
                }
                else
                {
                    char buff[1024];
                    std::memset(buff, '\0', sizeof(buff));
                    int content = read(all_df[i].fd,buff,1024);
                    for (size_t j = 0; j < all_client.size(); j++)
                    {
                        if(all_client[j].getfd() == all_df[i].fd)
                        {
                            all_client[j].appendreq(buff,content);
                            all_client[j].addTocontentread(content);

                            if((all_client[j].ischunked && all_client[j].getreq().find("\r\n0\r\n\r\n") != std::string::npos)
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
                for (size_t j = 0; j < all_client.size(); j++)
                {
                    if(all_client[j].getfd() == all_df[i].fd)
                    {
                        request req;
                        int ret_status = req.processing_request(all_client[j],  all_server[all_client[j]._serverIndex]);
                        
                        if (all_client[j]._requestIsParsed == true)
                        {
                            response resp;
                            all_client[j]._response = resp.generating_response(req, ret_status);
                            write(all_df[i].fd,all_client[j]._response.c_str(), all_client[j]._response.length());
                        }
                        close(all_df[i].fd);
                        all_df.erase(all_df.begin() + i);
                        all_client.erase(all_client.begin() + j);         
                        break;      
                    }
                }        
            }
            else if (all_df[i].revents  & POLLHUP) {
                close(all_df[i].fd);
                all_df.erase(all_df.begin() + i);
            } else if (all_df[i].revents  & POLLERR) {
                std::cerr << "Error on socket " << all_df[i].fd << std::endl;
                close(all_df[i].fd);
                all_df.erase(all_df.begin() + i);
            } else if (all_df[i].revents  & POLLNVAL) {
                close(all_df[i].fd);
                all_df.erase(all_df.begin() + i);
            }
        }
    }
    return 0;
}