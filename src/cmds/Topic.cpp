#include "ACommands.hpp"

Topic::Topic(Server &server) : ACommands(server)
{
    this->server = server;
}

void Topic::execute(int fd, const std::string &line)
{
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;
    std::string cmd = "TOPIC";

    while (std::getline(iss, token, ' ')) {
        tokens.push_back(token);
    }

    // Ensure correct command format
    if (tokens.size() < 2) {
        this->server.sendResponse(ERR_NEEDMOREPARAMS(cmd), fd);
        return;
    }

    std::string channelName = tokens[1];

    // Find the channel
    Channel *channel = this->server.GetChannel(channelName);
    Client *client = this->server.GetClient(fd);
    if (!channel) {
        this->server.sendResponse(ERR_NOSUCHCHANNEL(client->GetNickname(), channel->GetName()), fd);
        return;
    }

    // Check if topic is restricted and user is an operator
    /* if (channel->GetTopicRestricted() && !channel->IsAdmin(client)) {
        this->server.sendResponse(ERR_CHANOPRIVSNEEDED(client->GetNickname(), channelName), fd);
        return;
    } */

    // If user is setting a new topic
    if (tokens.size() > 2) {
        size_t pos = line.find(":");
        if (pos == std::string::npos) {
            this->server.sendResponse(ERR_NEEDMOREPARAMS(cmd), fd);
            return;
        }

        std::string newTopic = line.substr(pos + 1); // Extract everything after ':'
        
        channel->SetTopic(newTopic);

        // Broadcast to all users in the channel
        std::string response = RPL_TOPICMSG(client->GetNickname(), channelName, newTopic);
        channel->SendToAll(response, fd, this->server);
        this->server.sendResponse(response, fd);
    } 
    else {
        // If no topic is provided, return the current topic
        if (channel->GetTopic().empty()) {
            this->server.sendResponse(RPL_NOTOPIC(client->GetNickname(), channel->GetName()), fd);
        } else {
            this->server.sendResponse(RPL_TOPICIS(client->GetNickname(), channel->GetName(), channel->GetTopic()), fd);
        }
    }
}

