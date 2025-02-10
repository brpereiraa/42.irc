#include "ACommands.hpp"

void Mode::execute(int fd, const std::string &line)
{
    cout << fd << endl;
    cout << line << endl;
    cout << "entrou mode" << endl;
}