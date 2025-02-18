#include "ACommands.hpp"

Kick::Kick(Server &server) : ACommands(server) 
{
    this->server = server;
}

void Kick::execute(int fd, const std::string &line)
{
    cout << fd << endl;
    cout << line << endl;
    cout << "entrou kick" << endl;
}