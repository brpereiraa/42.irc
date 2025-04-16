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

bool Server::addChannel(Channel *channel) {
	if (this->channels.count(channel->GetName())) {
		std::cout << "Channel with topic  " << channel->GetName() << " already exists" << std::endl;
        return (false);
    }
	this->channels[channel->GetName()] = channel;
    return (true);
}

bool Server::addClient(Client *client) {
	if (this->clients.count(client->GetFd())) {
		std::cout << "Client with name  " << client->GetFd() << " already exists" << std::endl;
        return (false);
    }
	this->clients[client->GetFd()] = client;
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
    std::cout << "Client with name " << fd << " doesn't exist" << std::endl;
    return (false);
}


//-----------Setters/Getters----------------

std::map<int, Client *>   *Server::getClients() { return &this->clients; }
Client                  *Server::GetClient(int fd) {return (this->clients.at(fd)); }

std::map<std::string, Channel *>  *Server::getChannels() { return (&this->channels); }
Channel                         *Server::GetChannel(std::string name) { return (this->channels.at(name)); }

Channel *Server::GetChannelByName(std::string name) {
    std::map<std::string, Channel *>::iterator it = this->channels.begin();

    while (it != this->channels.end()){
        if (toLowerString(it->second->GetName()) == toLowerString(name))
            return (it->second);
        it++;
    }

    return (NULL);
}

std::string Server::getPassword() const { return this->password; }

std::string Server::getTime() const {
    std::stringstream ss;

    ss << this->time->tm_mon + 1 << "/" << this->time->tm_mday << " " << this->time->tm_hour << ":" << this->time->tm_min << ":" << this->time->tm_sec;

    return ss.str();
}

void Server::setTime() {
    time_t now = std::time(0);
    this->time = localtime(&now);
}

Client *Server::GetClientByNickname(std::string &nick) {
    for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (toLowerString(it->second->GetNickname()) == nick) {
            return it->second;
        }
    }
    return NULL;
}

int Server::GetClientChannelCount(Client *client) {
    int count = 1;
    for (std::map<std::string, Channel *>::iterator it = this->channels.begin(); it != this->channels.end(); ++it) {
        if (it->second->GetClientInChannel(client->GetNickname())) {
            count++;
        }
    }
    return count;
}
