#include "ACommands.hpp"

void Kick::execute(int fd, const std::string &line)
{
    cout << fd << endl;
    cout << line << endl;
    cout << "entrou kick" << endl;
}