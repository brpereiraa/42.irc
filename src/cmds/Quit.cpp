#include "ACommands.hpp"

Quit::Quit(Server &server) : ACommands(server) {
    this->server = server;
}

void Quit::execute(int fd, const std::string& line) {
    Client* client = this->server.GetClient(fd);
    if (!client)
        return;

    std::string reason = "Client Quit";
    size_t pos = line.find(':');
    if (pos != std::string::npos) {
        reason = line.substr(pos + 1);
    }

    std::string quitMsg = ":" + client->GetNickname() + "!" + client->GetUsername() + "@localhost QUIT :" + reason + "\r\n";

    std::map<std::string, Channel*>* channels = this->server.getChannels();

    for (std::map<std::string, Channel*>::iterator it = channels->begin(); it != channels->end(); ) {
        Channel* chan = it->second;
        const std::string& channelName = it->first;

        // Increment iterator before possibly erasing the channel
        ++it;

        if (!chan)
            continue;

        bool wasInChannel = false;

        if (chan->GetClientByNick(client->GetNickname())) {
            chan->RemoveClient(fd);
            wasInChannel = true;
        }

        if (chan->GetAdminByNick(client->GetNickname())) {
            chan->RemoveAdmin(client->GetNickname());
            wasInChannel = true;
        }

        if (wasInChannel) {
            chan->SendToAll(quitMsg, fd, this->server);
        }

        // Remove channel if empty
        if (chan->GetClients().empty() && chan->GetAdmins().empty()) {
            this->server.removeChannel(channelName);
        }
    }

    this->server.sendResponse(quitMsg, fd);
    this->server.removeClient(fd);
}
