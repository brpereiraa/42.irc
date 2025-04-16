#include "ACommands.hpp"

Quit::Quit(Server &server) : ACommands(server) {
    this->server = server;
}

void Quit::execute(int fd, const std::string& line) {
    Client* client = this->server.GetClient(fd);
    if (!client || !client->GetLoggedIn())
        return;

    // Parse quit reason
    std::string reason = "Client Quit";
    size_t pos = line.find(':');
    if (pos != std::string::npos && pos + 1 < line.length()) {
        reason = line.substr(pos + 1);
    }

    // QUIT message
    std::string quitMsg = ":" + client->GetNickname() + "!" + client->GetUsername() + "@localhost QUIT :" + reason + "\r\n";

    // Percorre todos os canais
    std::map<std::string, Channel*>* channels = this->server.getChannels();
    std::vector<std::string> toRemove;

    for (std::map<std::string, Channel*>::iterator it = channels->begin(); it != channels->end(); ++it) {
        Channel* chan = it->second;
        if (!chan) continue;

        const std::string& chanName = it->first;

        if (chan->GetClientByNick(client->GetNickname()) || chan->GetAdminByNick(client->GetNickname())) {
            chan->SendToAll(quitMsg, fd, this->server);
        }

        if (chan->GetClientByNick(client->GetNickname()))
            chan->RemoveClient(fd);

        if (chan->GetAdminByNick(client->GetNickname()))
            chan->RemoveAdmin(client->GetNickname());

        if (chan->GetClients().empty() && chan->GetAdmins().empty())
            toRemove.push_back(chanName);
    }

    // Remove canais vazios
    for (size_t i = 0; i < toRemove.size(); ++i) {
        this->server.removeChannel(toRemove[i]);
    }

    // Envia a mensagem de QUIT para o próprio cliente (opcional)
    send(fd, quitMsg.c_str(), quitMsg.length(), 0);
}
