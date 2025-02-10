#include "ACommands.hpp"

void Topic::execute(int fd, const std::string &line)
{
    cout << fd << endl;
    cout << line << endl;
    cout << "entrou topic" << endl;
}