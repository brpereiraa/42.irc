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
		std::cout << "User with nickname " << client.GetUsername() << " already exists" << std::endl;
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