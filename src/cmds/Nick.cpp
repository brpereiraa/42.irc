#include "ACommands.hpp"

Nick::Nick(Server &server): ACommands(server){
	this->server = server;
}

void Nick::execute(int fd, const std::string &line){
	(void)fd;
	(void)line;
}
