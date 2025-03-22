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

void Channel::SendToAll(const std::string &reply, int senderFd, Server &server) {
    // Send message to all admins
    for (std::map<int, Client>::iterator it = this->admins.begin(); it != this->admins.end(); ++it) {
        if (it->second.GetFd() != senderFd) {
            server.sendResponse(reply, it->second.GetFd());
        }
    }

    // Send message to all regular clients
    for (std::map<int, Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it) {
        if (it->second.GetFd() != senderFd) {
            server.sendResponse(reply, it->second.GetFd());
        }
    }
}

void Channel::ClearClients() 
{
    // Iterate through the clients map and close each connection
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        int clientFd = it->second.GetFd();
        
        // Send a final message to the client before removing them
        std::string message = "You are being disconnected from the channel.";
        send(clientFd, message.c_str(), message.size(), 0);
        
        // Close the client connection (if needed)
        close(clientFd);
    }

    // Clear the clients map
    clients.clear();

    // Optionally clear the admins if needed
    for (std::map<int, Client>::iterator it = admins.begin(); it != admins.end(); ++it) {
        int adminFd = it->second.GetFd();
        close(adminFd);  // Close the admin connection if necessary
    }
    admins.clear(); // Clear the admin map
}

bool Channel::GetClientInChannel(std::string &nickname) 
{
    for (std::map<int, Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it) {
        if (it->second.GetNickname() == nickname) {
            return true;
        }
    }
    return false;
}
