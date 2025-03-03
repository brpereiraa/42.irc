#include "ACommands.hpp"

Pong::Pong(Server &server): ACommands(server){
	this->server = server;
}

void Pong::execute(int fd, const std::string &line){
	size_t spacePos = line.find(' ');  // Find the first space
	if (spacePos != std::string::npos) {
		std::string pingParam = line.substr(spacePos + 1);  // Extract the ping parameter

		// Create a PONG response message
		std::string pongMsg = ":myserver PONG myserver " + pingParam + "\r\n";

		// Send the PONG reply to the client
		send(fd, pongMsg.c_str(), pongMsg.size(), 0);
	}
}
