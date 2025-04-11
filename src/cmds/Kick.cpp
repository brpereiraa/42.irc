#include "ACommands.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Parser.hpp"

Kick::Kick(Server &server) : ACommands(server) 
{
    this->server = server;
}

//verifica se o cliente e admin
bool Kick::isAdmin(const Client &client, const Channel &channel) 
{
    const std::map<int, Client>& admins = channel.GetAdmins();
    for (std::map<int, Client>::const_iterator it = admins.begin(); it != admins.end(); ++it) 
    {
        if (it->second.GetFd() == client.GetFd() &&
            it->second.GetNickname() == client.GetNickname() &&
            it->second.GetUsername() == client.GetUsername()) 
            {
            return true;
        }
    }
    return false;
}


void Kick::execute(int fd, const std::string& line)
{
    // Split the line into arguments
    std::stringstream line_stream(line);
    std::string temp;
    std::vector<std::string> tokens;
    std::vector<std::string> users;
    Channel *channel;

    Client *client;
    Client *client_kicker;

    while (line_stream >> temp) {
        tokens.push_back(temp);
    }
 
    if (tokens.size() < 3)
    {
        // this->server.sendResponse(ERR_NEEDMOREPARAMS)
        std::cerr << ":myserver 461: Not enough parameters" << std::endl;
        return ;
    }

    std::stringstream token_stream(tokens[2]);
    while (std::getline(token_stream, temp, ','))
        users.push_back(temp);

    
    channel = this->server.GetChannelByName(tokens[1]); 
    if (!channel)
    {
        std::cerr << ":myserver 403: No such channel" << std::endl;
        return ;
    }
    
    for (std::vector<std::string>::iterator it = users.begin(); it != users.end(); ++it) {
        std::cout << "User: " << *it << std::endl;

        client = channel->GetClientByNick(*it);
        if (!client)
        {
            std::cerr << ":myserver 401: No such nick" << std::endl;
            return ;
        }

        if (!channel->GetClientByNick(*it) && !channel->GetAdminByNick(*it))
        {
            std::cerr << ":myserver 441: User not in channel" << std::endl;
            return ;
        }

        client_kicker = this->server.GetClient(fd);
        if (!channel->GetClientByNick(client_kicker->GetNickname()) && !channel->GetAdminByNick(client_kicker->GetNickname()))
        {
            std::cerr << ":myserver 442: You're not on that channel" << std::endl;
            return ;
        }

        if (!channel->GetAdminByNick(client_kicker->GetNickname()))
        {
            std::cerr << ":myserver 482: You're not a channel operator" << std::endl;
            return ;
        }

        if (tokens.size() == 3)
        {
            this->server.sendResponse(RPL_KICKMSG(client_kicker->GetNickname(), tokens[1], *it), fd);
            channel->SendToAll(RPL_KICKMSG(client_kicker->GetNickname(), tokens[1], *it), fd, this->server);
        }
        else
        {
            this->server.sendResponse(RPL_KICKMSG(client_kicker->GetNickname(), tokens[1], *it), fd);
            channel->SendToAll(RPL_KICKMSG(client_kicker->GetNickname(), tokens[1], *it), fd, this->server);
        }

        channel->RemoveClientNick(client->GetNickname());

    }
}