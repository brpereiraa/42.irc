#include "ACommands.hpp"

Privmsg::Privmsg(Server &server) : ACommands(server) {
    this->server = server;
}

void Privmsg::execute(int fd, const std::string &line) {
    std::vector<std::string> tokens;
    std::istringstream ss(line);
    std::string token;

    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
    }

    Client* sender = this->server.GetClient(fd);
    if (!sender) return;

    std::string cmd = "PRIVMSG";

    if (tokens.size() < 2) {
        this->server.sendResponse(ERR_NORECIPIENT(sender->GetNickname(), cmd), fd);
        return;
    }

    if (!sender->GetLoggedIn()) {
        this->server.sendResponse(ERR_NOTREGISTERED(cmd), fd);
        return;
    }

    // Extract targets
    std::vector<std::string> targets;
    std::istringstream targetsStream(tokens[1]);
    while (std::getline(targetsStream, token, ',')) {
        targets.push_back(token);
    }

    if (targets.size() > 10) {
        this->server.sendResponse(ERR_TOOMANYTARGETS(sender->GetNickname(), tokens[1]), fd);
        return;
    }

    // Extract message
    std::string message;
    size_t colonPos = line.find(" :");
    if (colonPos != std::string::npos) {
        message = line.substr(colonPos + 2);  // Everything after ' :'
    } else if (tokens.size() >= 3) {
        message = tokens[2];  // Just the third token as message
    } else {
        this->server.sendResponse(ERR_NOTEXTTOSEND(sender->GetNickname()), fd);
        return;
    }

    // Send message to each target
    for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); ++it) {
        std::string target = *it;
        std::string response = ":" + sender->GetNickname() + " PRIVMSG " + target + " :" + message + "\r\n";

        if (target[0] == '#') {
            Channel* channel = this->server.GetChannelByName(target);
            if (!channel) {
                this->server.sendResponse(ERR_NOSUCHCHANNEL(sender->GetNickname(), target), fd);
                continue;
            }

            if (!channel->GetClientByNick(sender->GetNickname()) && !channel->GetAdminByNick(sender->GetNickname())) {
                this->server.sendResponse(ERR_CANNOTSENDTOCHAN(sender->GetNickname(), target), fd);
                continue;
            }

            channel->SendToAll(response, fd, this->server);
        } else {
            Client* recipient = this->server.GetClientByNickname(target);
            if (!recipient) {
                this->server.sendResponse(ERR_NOSUCHNICK(sender->GetNickname(), target), fd);
                continue;
            }

            this->server.sendResponse(response, recipient->GetFd());
        }
    }
}
