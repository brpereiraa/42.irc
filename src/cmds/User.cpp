#include "ACommands.hpp"

User::User(Server &server): ACommands(server){
	this->server = server;
}

void User::execute(int fd, const std::string &line){
    (void)fd;
	std::cout << "Line is " << line << std::endl;
}
