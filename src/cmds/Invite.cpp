#include "ACommands.hpp"

Invite::Invite(Server &server) : ACommands(server) 
{
    this->server = server;
}

void Invite::execute(int fd, const std::string &line)
{
    std::istringstream stream(line);
	std::string word;
    Channel *channel;
	Client client;
	Client *i_client;
	std::map<int, Client>::iterator it;
	int i;
	
    channel = NULL;
	it = server.getClients().find(fd);
	client = it->second;
	it = server.getClients().begin();
	i = 0;

	//Check auth handling
	if (!client.GetLoggedIn()) {
		server.sendResponse(":myserver 464 " + client.GetNickname() + " :Password incorrect\r\n", fd);
		return ;
	}
	
	while (stream >> word){
        if (i == 1){
            i_client = server.GetClientByNickname(word);

            //Check if client being invited exists
            if (!i_client){
                server.sendResponse(":server 401 " + client.GetNickname() + " " + word + " :No such nick\r\n", fd);
                return ;
            }
        }

        if ( i == 2 )
            channel = server.GetChannelByName(word);
        i++;
    };

    if ( i != 3 ){
        std::cout << "Wrong number of args" << std::endl; 
        return;
    }

    if (!channel){
        server.sendResponse(":server 403 " + client.GetNickname() + " :No such channel exists\r\n", fd);
        return ;
    }

    i = 0;

}