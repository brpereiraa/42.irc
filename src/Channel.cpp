#include "Libraries.hpp"

Channel::Channel() {}
Channel::Channel(const std::string topic){ this->topic = topic; }

Channel::~Channel() {}

const std::string Channel::GetPassword() const { return this->password; }
const std::string Channel::GetTopic() const { return this->topic; }

void Channel::SetPassword(std::string password) { this->password = password; }
void Channel::SetTopic(std::string topic) { this->topic = topic; }

void Channel::AddClient(Client &client){
	if (this->clients.count(client.GetUsername()))
		std::cout << "User with nickname " << client.GetUsername() << " already exists" << std::endl;
	else
		this->clients[client.GetUsername()] = client;
}

void Channel::RemoveClient(Client &client){
	this->clients.erase(client.GetUsername());
}

const std::map<std::string, Client> Channel::GetClients() const{
	return this->clients;
}

const std::map<std::string, Client> Channel::GetAdmins() const{
	return this->admins;
}