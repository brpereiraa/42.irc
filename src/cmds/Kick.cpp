#include "ACommands.hpp"

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
    std::string cmd = "KICK";

    Client *client;
    Client *client_kicker;

    while (line_stream >> temp) {
        tokens.push_back(temp);
    }

    if (tokens.size() < 3) {
        this->server.sendResponse(ERR_NEEDMOREPARAMS(cmd), fd);
        return ;
    }

    std::stringstream token_stream(tokens[2]);
    while (std::getline(token_stream, temp, ',')) {
        users.push_back(temp);
    }

    // Get the channel by name
    channel = this->server.GetChannelByName(tokens[1]); 
    if (!channel) {
        Client* sender = this->server.GetClient(fd);
        if (sender) {
            this->server.sendResponse(ERR_NOSUCHCHANNEL(sender->GetNickname(), tokens[1]), fd);
        }
        return;
    }

    for (std::vector<std::string>::iterator it = users.begin(); it != users.end(); ++it) {
        std::cout << "User: " << *it << std::endl;

        // Check if the client exists on the server
        client = this->server.GetClientByNickname(*it);  // Get the client by nickname
        if (!client) {
            // The client doesn't exist on the server
            client_kicker = this->server.GetClient(fd); // Get the kicker (the one sending the command)
            if (client_kicker) {
                this->server.sendResponse(ERR_NOSUCHNICK(client_kicker->GetNickname(), *it), fd);  // Send error if no such nick
            }
            return ;
        }

        // Check if the user is in the channel
        if (!channel->GetClientByNick(*it) && !channel->GetAdminByNick(*it)) {
            // If the user is neither a client nor an admin of the channel, send ERR_USERNOTINCHANNEL
            client_kicker = this->server.GetClient(fd); // Get the kicker (the one sending the command)
            if (client_kicker) {
                this->server.sendResponse(ERR_USERNOTINCHANNEL(client_kicker->GetNickname(), *it, channel->GetName()), fd);  // Send error for user not in channel
            }
            return ;
        }

        client_kicker = this->server.GetClient(fd); // Get the kicker (the one sending the command)

        // If the kicker is not in the channel, send ERR_INVITERINCHANNEL
        if (!channel->GetClientByNick(client_kicker->GetNickname()) && !channel->GetAdminByNick(client_kicker->GetNickname())) {
            this->server.sendResponse(ERR_INVITERINCHANNEL(client_kicker->GetNickname(), client_kicker->GetNickname()), fd);
            return ;
        }

        // If the kicker is not an admin of the channel, send ERR_USERCANINVITE
        if (!channel->GetAdminByNick(client_kicker->GetNickname())) {
            this->server.sendResponse(ERR_USERCANINVITE(client_kicker->GetNickname(), client_kicker->GetNickname()), fd);
            return ;
        }

        this->server.sendResponse(RPL_KICKMSG(client_kicker->GetNickname(), tokens[1], *it), fd);
        channel->SendToAll(RPL_KICKMSG(client_kicker->GetNickname(), tokens[1], *it), fd, this->server);

        // Remove the client from the channel
        if (channel->GetAdminByNick(client->GetNickname())) {
            channel->RemoveAdmin(client->GetNickname());
        }
        else if (channel->GetClientByNick(client->GetNickname())) {
            channel->RemoveClientNick(client->GetNickname());
        }

        cout << channel->GetClients().empty() << endl;
        cout << channel->GetAdmins().empty() << endl;
        if (channel->GetClients().empty() && channel->GetAdmins().empty()) {
            this->server.removeChannel(channel->GetName());
        }
    }
}

