#include "ACommands.hpp"

Join::Join(Server &server) : ACommands(server)
{
    this->server = server;
}

bool Join::initialChecksJoin(int fd, size_t i, std::vector<std::string> tokens, Client *newClient, Channel *channel)
{
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
        cout << channel->GetName() << endl;
        this->server.sendResponse(ERR_TOOMANYCHANNELS(newClient->GetNickname(), channel->GetName()), fd);
        return true;
    }

    // Ensure the channel name has the '#' prefix
    // if (channelName[0] != '#') {
    //     channelName = "#" + channelName;
    // }

    // If the channel requires a password and the user didn't provide one
    if (!channel->GetPassword().empty() && (tokens.size() <= i + 1 || channel->GetPassword() != tokens[i + 1])) {
        this->server.sendResponse(ERR_BADCHANNELKEY(newClient->GetNickname(), channel->GetName()), fd);
        return true;
    }

    // If the channel is invite-only and the user is not invited
    if (channel->GetInvite() && !channel->GetInvitedByNick(newClient->GetNickname())) {
        std::cout << "Invite only channel" << std::endl;
        this->server.sendResponse(ERR_INVITEONLYCHAN(newClient->GetNickname(), channel->GetName()), fd);
        return true;
    }

    // If the channel is full
    if (channel->GetLimit() > 0 && channel->GetClients().size() >= static_cast<size_t>(channel->GetLimit())) {
        this->server.sendResponse(ERR_CHANNELISFULL(newClient->GetNickname(), channel->GetName()), fd);
        return true;
    }
    return false;
}

void Join::execute(int fd, const std::string &line)
{
    std::vector<std::string> tokens;
    std::vector<std::string> channelName;
    std::istringstream ssjoin(line);
    std::string token;

    while (std::getline(ssjoin, token, ' ')) {
        cout << token << endl;
        tokens.push_back(token);
    }

    std::istringstream iss(token);
    std::string cmds;
    while (std::getline(iss, cmds, ',')) {
        channelName.push_back(cmds);
    }

    if ((tokens.size() - 1) > 10)
        ThrowException("ERR_TOOMANYTARGETS (407)");

    for (size_t i = 0; i <= (channelName.size() - 1); i++) {
        if (this->server.GetChannelByName(channelName[i])) {
            joinChannel(fd, i, channelName);
        }
		else {
			createAndJoinChannel(fd, i, channelName);
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

    if (!channel->GetName().empty()) {
        this->server.sendResponse(joinMsg + RPL_TOPICIS(newClient->GetNickname(), tokens[i], channel->GetName()) + nameReply + endNames, fd);
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
        server.sendResponse(ERR_NOSUCHCHANNEL(newClient->GetNickname(), channelName), fd);
        return ;
    }

    Channel newChannel(channelName);

    if (initialChecksJoin(fd, i, tokens, newClient, &newChannel))
        return;

    newChannel.AddAdmin(*newClient);
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