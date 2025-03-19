#include "ACommands.hpp"

Privmsg::Privmsg(Server &server) : ACommands(server) 
{
    this->server = server;
}

void Privmsg::execute(int fd, const std::string &line)
{
    std::vector<std::string> tokens;
    std::istringstream ss(line);
    std::string token;

    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
    }

    //TODO: criar func de parser dos comandos e check de erros possiveis
    if (tokens.size() < 3) {  // Needs at least: "PRIVMSG <target> <message>"
        std::cerr << "Error: Invalid PRIVMSG format" << std::endl;
        return;
    }

    std::string target = tokens[1];  // Target (channel or user)
    std::string message = line.substr(line.find(" :", tokens[0].length()) + 2);

    Client* sender = this->server.GetClient(fd);
    if (!sender) {
        std::cerr << "Error: Sender not found" << std::endl;
        return;
    }

    std::string response = ":" + sender->GetNickname() + " PRIVMSG " + target + " :" + message + "\r\n";

    // Check if the message is for a channel
    if (target[0] == '#') {
        Channel* channel = this->server.GetChannel(target);
        if (channel) {
            channel->SendToAll(response, fd, this->server);
        } else {
            this->server.sendResponse("Error: Channel not found\r\n", fd);
        }
    }
    // Otherwise, it's a private message to another user
    else {
        Client* recipient = this->server.GetClientByNickname(target);
        if (recipient) {
            this->server.sendResponse(response, recipient->GetFd());
        } else {
            this->server.sendResponse("Error: User not found\r\n", fd);
        }
    }
}