#include "Server.hpp"

void    Server::HandleRegistration(Client &client, std::string buffer){
	std::vector<std::string> cmd;
    
	// if (buffer.find("\r") || buffer.find("\n")){
	// 	cmd = this->SplitBuffer(buffer);

    //     std::vector<std::string>::iterator it = cmd.begin();
	// 	while(it != cmd.end()){
	// 		if (it->substr(0,4) == "NICK") 
	// 			HandleNick(client, *it);
	// 		else if (it->substr(0,4) == "USER") 
	// 			HandleUser(client, *it);
	// 		else if (it->substr(0,4) == "PASS") 
	// 			HandlePass(client, *it);
    //     }
		
    //     return ;
    // }

	// if (it->substr(0,4) == "NICK") 
	// 	HandleNick(client, *it);
	// else if (it->substr(0,4) == "USER") 
	// 	HandleUser(client, *it);	
	// else if (it->substr(0,4) == "PASS") 
	// 	HandlePass(client, *it);

	return ;
}

// void    Server::HandleNick(Client &client, std::string buffer){

// }

// void    Server::HandleUser(Client &client, std::string buffer){

// }

// void	Server::HandlePass(Client &pass, std::string buffer){

// }