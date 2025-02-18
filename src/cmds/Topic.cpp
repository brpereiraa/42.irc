#include "ACommands.hpp"

Topic::Topic(Server &server) : ACommands(server) 
{
    this->server = server;
}

void Topic::execute(int fd, const std::string &line)
{
    cout << fd << endl;
    cout << line << endl;
    cout << "entrou topic" << endl;
}