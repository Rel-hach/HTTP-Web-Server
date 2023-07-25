#include "../../inc/server.hpp"
#include "../../inc/server_data.hpp"
#include "../../inc/request.hpp"
#include "../../inc/client.hpp"
#include "../../inc/tools.hpp"
#include "../../inc/response.hpp"
std::string  find_server_name(client client)
{
    std::string clean_name;
    int index = client.server_name.find(":");
    if(index !=(int) std::string::npos)
        clean_name=client.server_name.substr(0,index);
    else
       clean_name=  client.server_name;
    return clean_name;
}

server_data find_server(std::vector<server_data>& servers,std::vector<server_data>& deplicate, client client)
{
    server_data server_exicte;
    std::string server_name =find_server_name(client);
    for (size_t i = 0; i < servers.size(); i++)
    {
        std::cout<<servers[i].server_names[0]<<std::endl;
        if(servers[i].server_names[0] == server_name)
        {
            server_exicte=servers[i];
            break;
        }
    }
    for (size_t i = 0; i < deplicate.size(); i++)
    {
        if(deplicate[i].server_names[0] == server_name)
        {
            server_exicte=deplicate[i];
            break;
        }
    }
    return server_exicte;
}

std::vector<server_data> felter_server(std::vector<server_data>& servers) {
    std::vector<server_data> deplicate;
    for (size_t i = 0; i < servers.size(); i++) {
        bool is_duplicate = false;
        for (size_t j = 0; j < servers.size(); j++) {
            if (i != j && servers[i].port == servers[j].port && servers[i].host == servers[j].host) {
                is_duplicate = true;
                break;
            }
        }
        if (is_duplicate) {
            deplicate.push_back(servers[i]);
            servers.erase(servers.begin() + i);
            i--; 
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
        std::cout<<"hi"<<std::endl;
		servers = parse_server(argv[1]);
        std::cout<<"hi"<<std::endl;
        std::vector<server_data> deplicate = felter_server(servers); 
        // for (size_t i = 0; i < deplicate.size(); i++)
        // {
        //     std::cout<< deplicate[i].host <<"------"<< deplicate[i].port << "--------"<< deplicate[i].server_names[0]<<std::endl;
        // }
        std::vector<pollfd> all_df;
        std::vector<server> all_server;
        std::vector<client> all_client;
        std::vector<int> fd_server;
        std::string req;

        //staart server and bind and lesten
        for (size_t i = 0; i < servers.size(); i++)
        {
            server Server = server(servers[i].port,servers[i].host);
            all_server.push_back(Server);
            if(Server.startServer())
                return 1;
            struct pollfd fds;
            fds.fd= Server.getSockert();
            fds.events = POLLIN;
            all_df.push_back(fds);
            fd_server.push_back(Server.getSockert());
            // i need bind of all port
            if(Server.bindServer())
                return 1;
            if(Server.listenSrver())
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
                        fcntl(client_socket, F_SETFL, O_NONBLOCK);
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
                        if(content <= 0)
                            continue;
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
                    for (size_t j = 0; j < all_client.size(); j++)
                    {
                        if(all_client[j].getfd() == all_df[i].fd)
                        {
                            std::ofstream file("output.txt");
                            if (file.is_open()) {
                                file<<all_client[j].getreq();
                            } else {
                                std::cout << "Unable to open the file." << std::endl;
                            }
                            std::cout<<find_server(servers, deplicate, all_client[j]).server_names[0]<<std::endl;
                            // request req;
                            // int ret_status = req.processing_request(all_client[j],  all_server[all_client[j]._serverIndex]);
                            
                            // if (all_client[j]._requestIsParsed == true)
                            // {
                            //     response resp;
                            //     all_client[j]._response = resp.generating_response(req, ret_status);
                            //     write(all_df[i].fd,all_client[j]._response.c_str(), all_client[j]._response.length());
                            // }
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
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}


    return 0;
}

