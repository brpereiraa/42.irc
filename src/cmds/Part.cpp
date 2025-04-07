#include "ACommands.hpp"

Part::Part(Server &server): ACommands(server){
	this->server = server;
}

void Part::execute(int fd, const std::string &line){
    std::vector<std::string> tokens;
    std::istringstream ss(line);
    std::string token;
    Channel *channel = NULL;

    std::cout << "Part command received" << std::endl;

    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 2) {
        std::cout << "Not enough parameters" << std::endl;
        // this->server.sendResponse(ERR_NEEDMOREPARAMS(token[0]), fd);
        return ;
    }

    channel = this->server.GetChannel(tokens[1]);

    for (size_t i = 1; i < tokens.size(); i++) {
        if (tokens[i][0] != '#') {
            this->server.sendResponse(ERR_NOSUCHCHANNEL(this->server.GetClient(fd)->GetNickname(), tokens[i]), fd);
            continue ;
        }
        std::cout << "Channel: " << tokens[i] << std::endl;
        this->server.GetChannel(tokens[i])->RemoveClient(fd);
        channel->SendToAll(RPL_PARTMSG(this->server.GetClient(fd)->GetNickname(), tokens[i]), fd, this->server);
        this->server.sendResponse(RPL_PARTMSG(this->server.GetClient(fd)->GetNickname(), tokens[i]), fd);
    }
}
