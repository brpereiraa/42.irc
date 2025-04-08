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
	Client *client = server.GetClient(fd);
    bool inviter_in_channel = false;
	Client *i_client;
	int i;
	
    channel = NULL;
	client = server.GetClient(fd);
	i = 0;

	//Check auth handling
	if (!client->GetLoggedIn()) {
		server.sendResponse(":myserver 464 " + client->GetNickname() + " :Password incorrect\r\n", fd);
		return ;
	}
	

	while (stream >> word){
        i++;
        if (i == 2){
            i_client = server.GetClientByNickname(word);

            //Check if client being invited exists
            if (!i_client){
                server.sendResponse(ERR_NOSUCHNICKCHAN(client->GetNickname(), channel->GetName()), fd);
                return ;
            }
        }

        if (i == 3)
            channel = server.GetChannel(word);
    };

    //verifica os parametros
    if (i != 3) {
        server.sendResponse(":myserver 461 " + client->GetNickname() + " INVITE :Not enough parameters\r\n", fd);
        return;
    }

    //verifica se o inviter esta no channel
    std::map<int, Client> channel_clients = channel->GetClients();
    std::map<int, Client>::const_iterator it = channel_clients.begin();
    while (it != channel_clients.end()) {
        if (it->second.GetNickname() == client->GetNickname()) {
            inviter_in_channel = true;
            break;
        }
        ++it;
    }

    //verifica se o inviter esta no channel a qual quer convidar
    if (!inviter_in_channel) {
        server.sendResponse(ERR_INVITERINCHANNEL(client->GetNickname(), channel->GetName()), fd);
        return;
    }

    //verifica se o channel e invite only e se o inviter pode convidar
    if (channel->GetInvite() && channel->GetAdmins().find(fd) == channel->GetAdmins().end()) {
        server.sendResponse(ERR_USERCANINVITE(client->GetNickname(), channel->GetName()), fd);
        return;
    }

    // if (channel_clients.size() >= channel->GetLimit()) {
    //     server.sendResponse(":myserver 471 " + client->GetNickname() + " " + channel->GetName() + " :Cannot invite, channel is full\r\n", fd);
    //     return;
    // }

    //verifica se o channel existe
    if (!channel){
        server.sendResponse(":server 403 " + client->GetNickname() + " :No such channel exists\r\n", fd);
        return ;
    }
    
    if (channel->GetClientInChannel(i_client->GetNickname())){
        server.sendResponse(ERR_ALREADYINCHANNEL(client->GetNickname(), channel->GetName()), fd);
        return ;
    }

    server.sendResponse((":" + client->GetNickname() + "!" + client->GetUsername() + "@localhost INVITE " + i_client->GetNickname() + channel->GetName() + "\r\n"), fd);
    server.sendResponse(":" + client->GetNickname() + " INVITE " +  i_client->GetNickname() + " :" + channel->GetName() + "\r\n",  i_client->GetFd());

    channel->AddInvited(*i_client);
}