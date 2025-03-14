#include "ACommands.hpp"

Join::Join(Server &server) : ACommands(server)
{
    this->server = server;
}

void Join::execute(int fd, const std::string &line)
{
    cout << fd << endl;
    cout << line << endl;
    cout << "entrou join" << endl;

    std::vector<std::string> tokens;
    std::istringstream ssjoin(line);
    std::string token;

    while (std::getline(ssjoin, token, ' ')) {
        tokens.push_back(token);
    }

    if ((tokens.size() - 1) > 10)
        ThrowException("ERR_TOOMANYTARGETS (407)");

    cout << "tokens size: " << tokens.size() << endl;

    for (size_t i = 1; i <= (tokens.size() - 1); i++) {
        std::map<std::string, Channel> channels = this->server.getChannels();
        if (channels.find(tokens[i]) != channels.end()) {
            joinChannel(fd, i, tokens);
            cout << "Channel " << tokens[i] << " already exists." << endl;
        }
		else {
			createAndJoinChannel(fd, i, tokens);
            //criar novo channel com topic tokens[i] -> add channel server -> add client no channel
            cout << "Channel " << tokens[i] << " does not exist." << endl;
        }
    }
}

void Join::joinChannel(int fd, size_t i, std::vector<std::string> tokens)
{
    Client *newClient = this->server.GetClient(fd);
    Channel *channel = this->server.GetChannel(tokens[i]);

    if (!channel) return; // Ensure channel exists

    channel->AddClient(*newClient); // Modify the actual channel in server

    std::string joinMsg = RPL_JOINMSG(newClient->GetNickname(), newClient->GetUsername(), tokens[i]);
    std::string nameReply = RPL_NAMREPLY(newClient->GetNickname(), tokens[i], channel->ClientChannelList());
    std::string endNames = RPL_ENDOFNAMES(newClient->GetNickname(), tokens[i]);

    if (!channel->GetTopic().empty()) {
        this->server.sendResponse(joinMsg + RPL_TOPICIS(newClient->GetNickname(), tokens[i], channel->GetTopic()) + nameReply + endNames, fd);
    } else {
        this->server.sendResponse(joinMsg + nameReply + endNames, fd);
    }

    channel->SendToAll(joinMsg, fd, this->server); // Send JOIN message to all users
}

void Join::createAndJoinChannel(int fd, size_t i, std::vector<std::string> tokens) 
{
    Client *newClient = this->server.GetClient(fd);
    std::string channelName = tokens[i];

    // Create and add the channel to the server
    Channel newChannel(channelName);
    newChannel.AddClient(*newClient);
    this->server.addChannel(newChannel);

    Channel *createdChannel = this->server.GetChannel(channelName);
    if (!createdChannel) return;

    std::string joinMsg = RPL_JOINMSG(newClient->GetNickname(), newClient->GetUsername(), channelName);
    std::string nameReply = RPL_NAMREPLY(newClient->GetNickname(), channelName, createdChannel->ClientChannelList());
    std::string endNames = RPL_ENDOFNAMES(newClient->GetNickname(), channelName);

    this->server.sendResponse(joinMsg + nameReply + endNames, fd);
}

// void Join::joinChannel(int fd, size_t i, std::vector<std::string> tokens)
// {
//     //check if client is in more than 10 channels (405)
//     Client *newClient = this->server.GetClient(fd);
//     Channel channel = *this->server.GetChannel(tokens[i]);

//     channel.AddClient(*newClient);

//     if (channel.GetTopic().empty()) {
//         this->server.sendResponse(
//             RPL_JOINMSG(newClient->GetUsername(), this->server.GetClient(fd)->GetIpAdd(), channel.GetTopic()) + \
//             RPL_NAMREPLY(newClient->GetNickname(), channel.GetTopic(), channel.ClientChannelList()) + \
//             RPL_ENDOFNAMES(newClient->GetNickname(), channel.GetTopic()), fd);
//     }
//     else {
//         this->server.sendResponse(
//             RPL_JOINMSG(newClient->GetUsername(), newClient->GetIpAdd(), channel.GetTopic()) + \
//             RPL_TOPICIS(newClient->GetNickname(), channel.GetTopic(), channel.GetTopic()) + \
//             RPL_NAMREPLY(newClient->GetNickname(), channel.GetTopic(), channel.ClientChannelList()) + \
//             RPL_ENDOFNAMES(newClient->GetNickname(), channel.GetTopic()), fd);
//     }

//     channel.SendToAll(RPL_JOINMSG(newClient->GetUsername(), newClient->GetIpAdd(), tokens[i]), fd);
// }

// void Join::createAndJoinChannel(int fd, size_t i, std::vector<std::string> tokens) 
// {
//     //check if client is in more than 10 channels (405)
//     Channel newChannel(tokens[i]);
//     Client *newClient = this->server.GetClient(fd);

//     cout << "name: " << newClient->GetNickname() << endl;   
//     newChannel.AddClient(*newClient);
//     this->server.addChannel(newChannel);    

//     this->server.sendResponse(RPL_JOINMSG(newClient->GetUsername(),newClient->GetIpAdd(),newChannel.GetTopic()) + \
//         RPL_NAMREPLY(newClient->GetNickname(), newChannel.GetTopic(), newChannel.ClientChannelList()) + \
//         RPL_ENDOFNAMES(newClient->GetNickname(), newChannel.GetTopic()), fd);
// }
