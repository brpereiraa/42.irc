#include "ACommands.hpp"

Mode::Mode(Server &server) : ACommands(server) 
{
    this->server = server;
}

void Mode::execute(int fd, const std::string &line)
{
    // cout << fd << endl;
    // cout << line << endl;
    // cout << "entrou mode" << endl;

    (void) fd;
    (void) line;
}