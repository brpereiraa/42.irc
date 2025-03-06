#include "ACommands.hpp"

Pass::Pass(Server &server): ACommands(server){
	this->server = server;
}

void Pass::execute(int fd, const std::string &line){
	(void)fd;
	(void)line;
}
