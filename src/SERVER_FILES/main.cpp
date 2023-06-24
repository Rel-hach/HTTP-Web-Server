#include "INCLUDES/serv_monitor.hpp"

int main() 
{
    std::ifstream file("conf/default.toml");

    if (!file.is_open())
    {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    std::string line;
    std::ostringstream bufferStream;
    while (std::getline(file, line))
        bufferStream << line << '\n';

    std::string buffer = bufferStream.str();

    std::vector<Server> servers;
    splitByServer(buffer, servers);
    parseDirectives(servers);
    Monitor monitor;
    monitor.starting_theProcess(servers);


    return 0;
}
