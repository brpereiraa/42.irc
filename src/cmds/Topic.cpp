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
    Client *client = this->server.GetClient(fd);
    Channel *channel = this->server.GetChannelByName(channelName);

    if (!client || !client->GetLoggedIn()) {
        this->server.sendResponse(ERR_NOTREGISTERED(cmd), fd);
        return;
    }
    
    if (!channel) {
        this->server.sendResponse(ERR_NOSUCHCHANNEL(client->GetNickname(), channelName), fd);
        return;
    }

    // Check topic restriction
    if (channel->GetTopicRestricted() && !channel->IsAdmin(client->GetFd())) {
        this->server.sendResponse(ERR_CHANOPRIVSNEEDED(client->GetNickname(), channelName), fd);
        return;
    }

    // If user is setting a topic
    if (tokens.size() > 2) {
        std::string newTopic;
        size_t colonPos = line.find(':');

        if (colonPos != std::string::npos) {
            newTopic = line.substr(colonPos + 1);
        } else {
            newTopic = tokens[2]; // Just the word after the channel
        }

        channel->SetTopic(newTopic);
        std::string response = RPL_TOPICMSG(client->GetNickname(), channelName, newTopic);
        channel->SendToAll(response, fd, this->server);
        this->server.sendResponse(response, fd);
    }
    else {
        // Return current topic
        if (channel->GetTopic().empty()) {
            this->server.sendResponse(RPL_NOTOPIC(client->GetNickname(), channelName), fd);
        } else {
            this->server.sendResponse(RPL_TOPICIS(client->GetNickname(), channelName, channel->GetTopic()), fd);
        }
    }
}
