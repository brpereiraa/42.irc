#include "ACommands.hpp"

Privmsg::Privmsg(Server &server) : ACommands(server) 
{
    this->server = server;
}

void Privmsg::execute(int fd, const std::string &line)
{
    std::cout << fd << std::endl;
    std::cout << line << std::endl;
    std::cout << "entrou no Privmsg" << std::endl;

    std::vector<std::string> tokens;
    std::istringstream ss(line);
    std::string token;

    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
    }

    // Verifica se há ao menos 2 tokens (destinatário e mensagem)
    if (tokens.size() < 2) {
        std::cerr << "Erro: Formato inválido para PRIVMSG" << std::endl;
        return;
    }

    std::string target = tokens[1];  // Destinatário (usuário ou canal)
    std::string message = line.substr(line.find(" ", tokens[0].length()) + 1);  // Mensagem após o primeiro espaço

    // Se o destinatário for um canal, enviamos a mensagem para todos os membros do canal
    if (target[0] == '#') {
        Channel* channel = this->server.GetChannel(target);
        if (channel) {
            std::string response = ":" + this->server.GetClient(fd)->GetNickname() + " PRIVMSG " + target + " :" + message + "\r\n";
            channel->SendToAll(response, fd, this->server);  // Envia para todos no canal
        } else {
            std::cerr << "Erro: Canal não encontrado!" << std::endl;
        }
    }
    // Se o destinatário for um usuário, enviamos a mensagem diretamente para o usuário
    else {
        Client* client = this->server.GetClient(fd);
        if (client) {
            std::string response = ":" + this->server.GetClient(fd)->GetNickname() + " PRIVMSG " + target + " :" + message + "\r\n";
            this->server.sendResponse(response, client->GetFd());  // Envia para o usuário
        } else {
            std::cerr << "Erro: Usuário não encontrado!" << std::endl;
        }
    }
}