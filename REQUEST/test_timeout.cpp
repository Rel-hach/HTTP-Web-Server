#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

int main()
{
    // Create a socket
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        std::cerr << "Error: Failed to create socket" << std::endl;
        return -1;
    }

    // Make the server socket non-blocking
    int flags = fcntl(server_sock, F_GETFL, 0);
    fcntl(server_sock, F_SETFL, flags | O_NONBLOCK);

    // Bind the server socket to a specific port
    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Error: Failed to bind socket" << std::endl;
        return -1;
    }

    // Listen for incoming client connections
    if (listen(server_sock, 128) < 0)
    {
        std::cerr << "Error: Failed to listen for incoming connections" << std::endl;
        return -1;
    }

    // Set the timeout value for incoming client connections to 5 seconds
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    while (true)
    {
        // Initialize a file descriptor set for the server socket
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server_sock, &read_fds);

        // Wait for incoming client connections with a timeout
        int result = select(server_sock + 1, &read_fds, NULL, NULL, &timeout);
        if (result < 0)
        {
            std::cerr << "Error: Failed to wait for incoming connections" << std::endl;
            return -1;
        }
        else if (result == 0)
        {
            // Handle the "Request Timeout" error here
            std::cerr << "Error: Request Timeout" << std::endl;
            continue;
        }

        // Accept the incoming client connection
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_len);
        if(client_sock < 0)
        {
            std::cerr << "Error: Failed to accept incoming connection" << std::endl;
            continue;
        }

        // Handle the incoming client connection here
        // ...

        // Close the client socket
        close(client_sock);
    }

    // Close the server socket
    close(server_sock);

    return 0;
}