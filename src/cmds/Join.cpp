#include "ACommands.hpp"

Join::Join(Server &server) : ACommands(server)
{
    this->server = server;
}

bool Join::initialChecksJoin(int fd, size_t i, std::vector<std::string> tokens, Client *newClient, Channel *channel)
{
    cout << "count channels by client: " << this->server.GetClientChannelCount(newClient) << endl;

    if (!newClient) {
        return true;
    }

    if (!channel) {
        this->server.sendResponse(ERR_NOSUCHCHANNEL(newClient->GetNickname(), tokens[i]), fd);
        return true;
    }

    //TODO: not sending any message
    // Check if the client is already in 10 channels
    if (this->server.GetClientChannelCount(newClient) >= 10) {
        this->server.sendResponse(ERR_TOOMANYCHANNELS(newClient->GetNickname(), channel->GetTopic()), fd);
        //return true;
    }

    // Ensure the channel name has the '#' prefix
    // if (channelName[0] != '#') {
    //     channelName = "#" + channelName;
    // }

    // // Create and add the channel to the server
    // Channel newChannel(channelName);
    
    // // If the channel requires a password and the user didn't provide one
    // if (!newChannel.GetPassword().empty() && (tokens.size() <= i + 1 || newChannel.GetPassword() != tokens[i + 1])) {
    //     if (!this->server.IsInvited(newClient, channelName, 0)) {
    //         this->server.sendResponse(ERR_BADCHANNELKEY(newClient->GetNickname(), channelName), fd);
    //         return;
    //     }
    // }

    // // If the channel is invite-only and the user is not invited
    // if (newChannel.GetInvitOnly() && !this->server.IsInvited(newClient, channelName, 1)) {
    //     this->server.sendResponse(ERR_INVITEONLYCHAN(newClient->GetNickname(), channelName), fd);
    //     return;
    // }

    // // If the channel is full
    // if (newChannel.GetLimit() && newChannel.GetClientsNumber() >= newChannel.GetLimit()) {
    //     this->server.sendResponse(ERR_CHANNELISFULL(newClient->GetNickname(), channelName), fd);
    //     return;
    // }
    return false;
}

void Join::execute(int fd, const std::string &line)
{
    std::vector<std::string> tokens;
    std::istringstream ssjoin(line);
    std::string token;

    while (std::getline(ssjoin, token, ' ')) {
        tokens.push_back(token);
    }

    if ((tokens.size() - 1) > 10)
        ThrowException("ERR_TOOMANYTARGETS (407)");

    for (size_t i = 1; i <= (tokens.size() - 1); i++) {
        std::map<std::string, Channel> channels = this->server.getChannels();
        if (channels.find(tokens[i]) != channels.end()) {
            joinChannel(fd, i, tokens);
            cout << "Channel " << tokens[i] << " already exists." << endl;
        }
		else {
			createAndJoinChannel(fd, i, tokens);
            cout << "Channel " << tokens[i] << " does not exist." << endl;
        }
    }
}

void Join::joinChannel(int fd, size_t i, std::vector<std::string> tokens)
{
    Client *newClient = this->server.GetClient(fd);
    Channel *channel = this->server.GetChannel(tokens[i]);

    if (initialChecksJoin(fd, i, tokens, newClient, channel))
        return;

    channel->AddClient(*newClient);

    std::string joinMsg = RPL_JOINMSG(newClient->GetNickname(), newClient->GetUsername(), tokens[i]);
    std::string nameReply = RPL_NAMREPLY(newClient->GetNickname(), tokens[i], channel->ClientChannelList());
    std::string endNames = RPL_ENDOFNAMES(newClient->GetNickname(), tokens[i]);

    if (!channel->GetTopic().empty()) {
        this->server.sendResponse(joinMsg + RPL_TOPICIS(newClient->GetNickname(), tokens[i], channel->GetTopic()) + nameReply + endNames, fd);
    } else {
        this->server.sendResponse(joinMsg + nameReply + endNames, fd);
    }

    channel->SendToAll(joinMsg, fd, this->server);
}

void Join::createAndJoinChannel(int fd, size_t i, std::vector<std::string> tokens) 
{
    Client *newClient = this->server.GetClient(fd);
    std::string channelName = tokens[i];

    // Ensure the channel name has the '#' prefix (if needed)
    if (channelName[0] != '#') {
        channelName = "#" + channelName;
    }

    Channel newChannel(channelName);

    if (initialChecksJoin(fd, i, tokens, newClient, &newChannel))
        return;

    newChannel.AddClient(*newClient);
    this->server.addChannel(newChannel);

    Channel *createdChannel = this->server.GetChannel(channelName);
    if (!createdChannel) {
        this->server.sendResponse(ERR_NOSUCHCHANNEL(newClient->GetNickname(), channelName), fd);
        return;
    }

    std::string joinMsg = RPL_JOINMSG(newClient->GetNickname(), newClient->GetUsername(), channelName);
    std::string nameReply = RPL_NAMREPLY(newClient->GetNickname(), channelName, createdChannel->ClientChannelList());
    std::string endNames = RPL_ENDOFNAMES(newClient->GetNickname(), channelName);

    this->server.sendResponse(joinMsg + nameReply + endNames, fd);
}