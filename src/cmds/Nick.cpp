#include "ACommands.hpp"

//Check existing user
//

Nick::Nick(Server &server): ACommands(server){
	this->server = server;
}

void Nick::execute(int fd, const std::string &line){
	std::istringstream stream(line);
	std::string word;
	Client client;
	std::map<int, Client>::iterator it;
	int i;
	
	it = server.getClients().find(fd);
	client = it->second;
	it = server.getClients().begin();
	i = -1;
	
	while (stream >> word) {
		++i;
		if (i == 2) break;
		if (i == 1) {
			if(word[0] == ':')
				std::cout << "Invalid nick" << std::endl;
			while (it != server.getClients().end()) {
				if (it->second.GetNickname() == word && fd != it->second.GetFd()) {
					server.sendResponse(":myserver 433 " + client.GetNickname() + " :Nickname given is already in use.\r\n", fd);
				}
				it++;
			}
			it = server.getClients().find(fd);
			if (it == server.getClients().end())
				return ;
			it->second.SetNickname(word);
			server.sendResponse(":myserver 001 " + word + " :Nickname changed successfully.\r\n", fd);
		}
	}
	if (i == 0)
		server.sendResponse(":myserver 431 " +  client.GetNickname() + " : No nickname has been provided.\r\n", fd);
}


// void Nick::CheckExistx(std::string name){

// }