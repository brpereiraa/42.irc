#include "ACommands.hpp"

Join::Join(Server &server) : ACommands(server)
{
    this->server = server;
}

bool Join::initialChecksJoin(int fd, Client* client, Channel* channel, const std::string& key)
{
    if (!client || !channel)
        return true;

    if (this->server.GetClientChannelCount(client) >= 10) {
        this->server.sendResponse(ERR_TOOMANYCHANNELS(client->GetNickname(), channel->GetName()), fd);
        return true;
    }

    if (!channel->GetPassword().empty() && channel->GetPassword() != key) {
        this->server.sendResponse(ERR_BADCHANNELKEY(client->GetNickname(), channel->GetName()), fd);
        return true;
    }

    if (channel->GetInvite() && !channel->GetInvitedByNick(client->GetNickname())) {
        this->server.sendResponse(ERR_INVITEONLYCHAN(client->GetNickname(), channel->GetName()), fd);
        return true;
    }

    // cout << "channel limit: " << channel->GetLimit() << endl;
    // cout << "clients number: " << channel->GetClients().size() << endl;
    // cout << "admins number: " << channel->GetAdmins().size() << endl;
    
    size_t totalUsers = channel->GetClients().size() + channel->GetAdmins().size();
    //cout << "total users: " << totalUsers << endl;
    if (channel->GetLimit() > 0 && totalUsers >= static_cast<size_t>(channel->GetLimit())) {
        this->server.sendResponse(ERR_CHANNELISFULL(client->GetNickname(), channel->GetName()), fd);
        return true;
    }

    return false;
}

void Join::execute(int fd, const std::string &line)
{
    Client* sender = this->server.GetClient(fd);
    if (!sender)
        return;

    std::string cmd = "JOIN";
    std::vector<std::string> tokens;
    std::istringstream ss(line);
    std::string token;

    while (std::getline(ss, token, ' '))
        tokens.push_back(token);

    if (tokens.size() < 2) {
        this->server.sendResponse(ERR_NEEDMOREPARAMS(cmd), fd);
        return;
    }

    std::vector<std::string> channelNames;
    std::vector<std::string> keys;

    // Split channel names
    std::istringstream chStream(tokens[1]);
    while (std::getline(chStream, token, ','))
        channelNames.push_back(token);

    // Split keys (if any)
    if (tokens.size() > 2) {
        std::istringstream keyStream(tokens[2]);
        while (std::getline(keyStream, token, ','))
            keys.push_back(token);
    }

    if (channelNames.size() > 10) {
        this->server.sendResponse(ERR_TOOMANYTARGETS(sender->GetNickname(), "*"), fd);
        return;
    }

    // Call join/create passing the correct key (if available)
    for (size_t i = 0; i < channelNames.size(); i++) {
        std::string key = (i < keys.size()) ? keys[i] : "";
        if (this->server.GetChannelByName(channelNames[i]))
            joinChannel(fd, channelNames[i], key);
        else
            createAndJoinChannel(fd, channelNames[i], key);
    }
}


void Join::joinChannel(int fd, const std::string& channelName, const std::string& key)
{
    Client* client = this->server.GetClient(fd);
    Channel* channel = this->server.GetChannel(channelName);

    if (!channel || !client) {
        if (client)
            this->server.sendResponse(ERR_NOSUCHCHANNEL(client->GetNickname(), channelName), fd);
        return;
    }

    if (initialChecksJoin(fd, client, channel, key))
        return;

    channel->AddClient(client);

    std::string joinMsg = RPL_JOINMSG(client->GetNickname(), client->GetUsername(), channelName);
    std::string nameReply = RPL_NAMREPLY(client->GetNickname(), channelName, channel->ClientChannelList());
    std::string endNames = RPL_ENDOFNAMES(client->GetNickname(), channelName);

    this->server.sendResponse(joinMsg + nameReply + endNames, fd);
    channel->SendToAll(joinMsg, fd, this->server);
}

void Join::createAndJoinChannel(int fd, const std::string& channelName, const std::string& key)
{
    Client* client = this->server.GetClient(fd);
    if (!client)
        return;

    if (channelName[0] != '#') {
        this->server.sendResponse(ERR_NOSUCHCHANNEL(client->GetNickname(), channelName), fd);
        return;
    }

    Channel *newChannel = new Channel(channelName);
    if (!key.empty())
        newChannel->SetPassword(key);

    newChannel->AddAdmin(client);
    this->server.addChannel(newChannel);

    Channel* createdChannel = this->server.GetChannel(channelName);
    if (!createdChannel)
        return;

    std::string joinMsg = RPL_JOINMSG(client->GetNickname(), client->GetUsername(), channelName);
    std::string nameReply = RPL_NAMREPLY(client->GetNickname(), channelName, createdChannel->ClientChannelList());
    std::string endNames = RPL_ENDOFNAMES(client->GetNickname(), channelName);

    this->server.sendResponse(joinMsg + nameReply + endNames, fd);
    createdChannel->SendToAll(joinMsg, fd, this->server);
}
