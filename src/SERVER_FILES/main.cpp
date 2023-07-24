#include "../../inc/server.hpp"
#include "../../inc/server_data.hpp"
#include "../../inc/request.hpp"
#include "../../inc/client.hpp"
#include "../../inc/tools.hpp"

int main(int argc,char **argv) 
{ 
    std::vector<server_data> servers;
	try
	{
		if (argc != 2)
			throw std::invalid_argument("Error: invalid number of arguments");
		servers = parse_server(argv[1]);
        std::vector<pollfd> all_df;
        std::vector<server> all_server;
        std::vector<client> all_client;
        std::vector<int> fd_server;
        std::string req;

        for (size_t i = 0; i <servers.size(); i++)
        {
            server Server = server(servers[i]);
            all_server.push_back(Server);
            if(all_server[i].startServer(servers[i]))
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
                        for (size_t i = 0; i < servers.size(); i++)
                        {
                            if(servers[i].fd_server == *it)
                                break;
                        }
                        client newclient = client(fds.fd , i );
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
                            // std::ofstream file("output.txt");
                            // if (file.is_open()) {
                            //     file<<all_client[j].getreq();
                            // } else {
                            //     std::cout << "Unable to open the file." << std::endl;
                            // }
                                std::ifstream file("root/index.html");
                                std::string html;
                                std::string line; 
                                if (!file.is_open()) {
                                    std::cerr << "Failed to open the file." << std::endl;
                                } 
                                if (file.is_open()) 
                                {
                                    while (std::getline(file, line))
                                        html+= line;
                                    file.close();
                                }
                                
                                std::string headers = "HTTP/1.1 200 OK\r\n";
                                headers += "Content-Type: text/html\r\n";
                                headers += "Set-Cookie: Darkmode=true\r\n";
                                headers += "Set-Cookie: sessionID=abc123; Path=/; Secure; HttpOnly\r\n";

                                headers += "Content-Length: " + std::to_string(html.length()) + "\r\n";
                                headers += "Connection: Closed\r\n\r\n";

                                headers +=  headers + html;  
                            write(all_df[i].fd,headers.c_str(),headers.length());
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
