#include "Libraries.hpp"

Channel::Channel() {}
Channel::Channel(const std::string topic){ this->topic = topic; }

Channel::~Channel() {}

const std::string Channel::GetPassword() const { return this->password; }
const std::string Channel::GetTopic() const { return this->topic; }

void Channel::SetPassword(const std::string password) { this->password = password; }
void Channel::SetTopic(const std::string topic) { this->topic = topic; }

void Channel::AddClient(Client &client){
	if (this->clients.count(client.GetFd()))
		std::cout << "User with nickname " << client.GetNickname() << " already exists" << std::endl;
	else
		this->clients[client.GetFd()] = client;
}

void Channel::RemoveClient(int fd){
	this->clients.erase(fd);
}

const std::map<int, Client> Channel::GetClients() const{
	return this->clients;
}

const std::map<int, Client> Channel::GetAdmins() const{
	return this->admins;
}

std::string Channel::ClientChannelList() {
    std::string list;

    // Iterar sobre os admins e adicionar "@nickname"
    for (std::map<int, Client>::iterator it = admins.begin(); it != admins.end(); ++it) {
        if (!list.empty()) list += " "; // Adiciona espaço se não for o primeiro item
        list += "@" + it->second.GetNickname();
    }

    // Iterar sobre os clients normais
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (!list.empty()) list += " "; // Adiciona espaço entre admins e clients
        list += it->second.GetNickname();
    }

    return list;
}

void Channel::SendToAll(std::string reply, int fd) {
    // Iterar sobre os admins
    for (std::map<int, Client>::iterator it = admins.begin(); it != admins.end(); ++it) {
        if (it->second.GetFd() != fd) {
            if (send(it->second.GetFd(), reply.c_str(), reply.size(), 0) == -1) {
                std::cerr << "send() failed" << std::endl;
            }
        }
    }

    // Iterar sobre os clients
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->second.GetFd() != fd) {
            if (send(it->second.GetFd(), reply.c_str(), reply.size(), 0) == -1) {
                std::cerr << "send() failed" << std::endl;
            }
        }
    }
} 