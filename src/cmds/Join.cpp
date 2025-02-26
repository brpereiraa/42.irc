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
            //JoinChannel(fd, i, tokens);
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
    //check if client is in more than 10 channels (405)
    Client *newClient = this->server.GetClient(fd);
    Channel channel = *this->server.GetChannel(tokens[i]);

    channel.AddClient(*newClient);

    //enviar mensagem RPL_JOINMSG client e channel como 1 param
    //topic RPL_TOPIC, if no topic, empty 
    //list of users RPL_NAMREPLY + RPL_ENDOFNAMES
}

void Join::createAndJoinChannel(int fd, size_t i, std::vector<std::string> tokens) 
{
    //check if client is in more than 10 channels (405)
    Channel newChannel(tokens[i]);
    Client *newClient = this->server.GetClient(fd);

    newChannel.AddClient(*newClient);
    this->server.addChannel(newChannel);

    if (newChannel.GetTopic().empty()) {
        this->server.sendResponse(
            RPL_JOINMSG(this->server.GetClient(fd)->GetUsername(), this->server.GetClient(fd)->GetIpAdd(), newChannel.GetTopic()) + \
            RPL_NAMREPLY(this->server.GetClient(fd)->GetNickname(), newChannel.GetTopic(), newChannel.ClientChannel_list()) + \
            RPL_ENDOFNAMES(this->server.GetClient(fd)->GetNickname(), newChannel.GetTopic()), fd);
    }
    else {
        this->server.sendResponse(
            RPL_JOINMSG(this->server.GetClient(fd)->GetUsername(), this->server.GetClient(fd)->GetIpAdd(), newChannel.GetTopic()) + \
            RPL_TOPICIS(this->server.GetClient(fd)->GetNickname(), newChannel.GetTopic(), newChannel.GetTopic()) + \
            RPL_NAMREPLY(this->server.GetClient(fd)->GetNickname(), newChannel.GetTopic(), newChannel.ClientChannel_list()) + \
            RPL_ENDOFNAMES(this->server.GetClient(fd)->GetNickname(), newChannel.GetTopic()), fd);
    }
    //send to all
}
