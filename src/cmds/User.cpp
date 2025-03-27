#include "ACommands.hpp"

User::User(Server &server): ACommands(server){
	this->server = server;
}

void User::execute(int fd, const std::string &line){
	std::istringstream stream(line);
	std::string word;
	Client client;
	std::map<int, Client>::iterator it;
	int i;
	
	it = server.getClients().find(fd);
	client = it->second;
	it = server.getClients().begin();
	i = -1;

	//Check password handling
	if (server.getPassword() != "" && client.GetPassword() != server.getPassword()) {
		server.sendResponse(":myserver 464 " 
			+ (!client.GetNickname().empty() ? client.GetNickname() : "*")  
			+ " :Password incorrect\r\n", fd);
		return ;
	}

	//Check if already logged
	if (this->server.GetClient(fd)->GetLoggedIn()){
		this->server.sendResponse(":myserver 462 " + client.GetNickname() + " :You may not reregister\r\n", fd);
		return ;
	}

	//Change username
	while (stream >> word) {

		++i;
		if (i == 4) break;
		if (i == 1) {
			it = server.getClients().find(fd);
			if (it == server.getClients().end())
				return ;
			it->second.SetUsername(word);
		}

	}

	//Missing argument
	if (i == 0)
		server.sendResponse(":myserver 431 " +  client.GetNickname() + " :No user has been provided.\r\n", fd);
}
