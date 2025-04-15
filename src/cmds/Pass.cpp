#include "ACommands.hpp"

Pass::Pass(Server &server): ACommands(server){
	this->server = server;
}

void Pass::execute(int fd, const std::string &line){
	std::string word;
	std::istringstream stream(line);
	std::map<int, Client *>::iterator it = server.getClients()->find(fd);
	int i = -1;
	
	while (stream >> word) {
		++i;
		if (i == 2) break;
		if (i == 1) {
			if (it->second->GetPassword() == server.getPassword()){
				server.sendResponse(":myserver 462 " +  
					(it->second->GetNickname().empty() ? "*" : it->second->GetNickname()) + 
					" :You may not reregister\r\n", fd);
				return ;
			}
			else if (word == server.getPassword())
				it->second->SetPassword(word);
			else
				server.sendResponse(":myserver 464 " +  
					(it->second->GetNickname().empty() ? "*" : it->second->GetNickname()) + 
					" :Password mismatch. Please provide the correct password\r\n", fd);
		}
	}
	if (i == 0)
		server.sendResponse(":myserver 464 " +  it->second->GetNickname() + " : No password has been provided.\r\n", fd);
}
