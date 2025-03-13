#include "Server.hpp"

//-----------Constructor------------

Server::Server() {}

Server::Server(int port, std::string password) 
{
    this->server_socket = -1;
    this->port = port;
    this->password =  password;
}


//-----------Map_Handler-------------------

bool Server::addChannel(Channel &channel) {
	if (this->channels.count(channel.GetTopic())) {
		std::cout << "Channel with topic  " << channel.GetTopic() << " already exists" << std::endl;
        return (false);
    }
	this->channels[channel.GetTopic()] = channel;
    return (true);
}

bool Server::addClient(Client &client) {
	if (this->clients.count(client.GetFd())) {
		std::cout << "Client with topic  " << client.GetFd() << " already exists" << std::endl;
        return (false);
    }
	this->clients[client.GetFd()] = client;
    return (true);
}

bool Server::removeChannel(std::string name) {
    if (this->channels.count(name)){
        this->channels.erase(name);
        return (true);
    }
    std::cout << "Channel with name " << name << " doesn't exist" << std::endl;
    return (false);
}

bool Server::removeClient(int fd) {
    if (this->clients.count(fd)){
        this->clients.erase(fd);
        return (true);
    }
    std::cout << "Channel with name " << fd << " doesn't exist" << std::endl;
    return (false);
}


//-----------Setters/Getters----------------

std::string Server::getTime() const {
    std::stringstream ss;

    ss << this->time->tm_mon + 1 << "/" << this->time->tm_mday << " " << this->time->tm_hour << ":" << this->time->tm_min << ":" << this->time->tm_sec;

    return ss.str();
}

void Server::setTime() {
    time_t now = std::time(0);
    this->time = localtime(&now);
}

Client *Server::GetClient(int fd) {
	for (size_t i = 0; i < this->clients.size(); i++){
		if (this->clients[i].GetFd() == fd)
			return &this->clients[i];
	}
	return NULL;
}

Channel *Server::GetChannel(std::string topic) {
    std::map<std::string, Channel>::iterator it = this->channels.find(topic);
    if (it != this->channels.end()) {
        return &it->second;
    }
	return NULL;
}

std::map<std::string, Channel> Server::getChannels() const {
    return this->channels;
}

std::map<int, Client> &Server::getClients() {
    return this->clients;
}

std::string Server::getPassword() const {
    return this->password;
}
