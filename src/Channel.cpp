#include "Libraries.hpp"

Channel::Channel() {}
Channel::Channel(const std::string name){ 
    this->name = name; 
    this->inv_only = false;
}

Channel::~Channel() {}

const   std::string Channel::GetPassword() const { return this->password; }
const   std::string Channel::GetTopic() const { return this->topic; }
const   std::string Channel::GetName() const { return this->name; }
const   std::map<int, Client> Channel::GetClients() const { return this->clients; }
const   std::map<int, Client> Channel::GetAdmins() const { return this->admins; }
const   std::map<int, Client> Channel::GetInvited() const { return this->admins; }
int     Channel::GetLimit() const { return this->usr_limit; }
bool    Channel::GetInvite() const { return inv_only; }


void    Channel::SetPassword(const std::string password) { this->password = password; }
void    Channel::SetTopic(const std::string topic) { this->topic = topic; }
void    Channel::SetInvite(const bool value) { this->inv_only = value; }
void    Channel::SetLimit(const int limit) { this->usr_limit = limit; }

Client *Channel::GetInvitedByNick(std::string nickname){
    std::map<int, Client>::iterator it = this->invited.begin();

    while (it != this->invited.end()){
        if (it->second.GetNickname() == nickname)
            return (&it->second);
        it++;
    }
    
    return (NULL);
}

Client *Channel::GetClientByNick(std::string nickname) {
    std::map<int, Client>::iterator it = this->clients.begin();

    while (it != this->clients.end()){
        if (it->second.GetNickname() == nickname)
            return (&it->second);
        it++;
    }
    
    return (NULL);
}

Client *Channel::GetAdminByNick(std::string nickname) {
    std::map<int, Client>::iterator it = this->admins.begin();

    while (it != this->clients.end()){
        if (it->second.GetNickname() == nickname)
            return (&it->second);
        it++;
    }
    
    return (NULL);
}


//----------Map Functions-------------

void Channel::AddClient(Client &client) {
	if (this->clients.count(client.GetFd()))
		std::cout << "User with nickname " << client.GetNickname() << " already exists" << std::endl;
	else
		this->clients[client.GetFd()] = client;
}

void Channel::AddAdmin(Client &client) {
	if (this->admins.count(client.GetFd()))
		std::cout << "User with nickname " << client.GetNickname() << " already exists" << std::endl;
	else
		this->admins[client.GetFd()] = client;
}

void Channel::AddInvited(Client &client) {
	if (this->invited.count(client.GetFd()))
        ;
	else
		this->clients[client.GetFd()] = client;
}

void Channel::RemoveClient(int fd){ this->clients.erase(fd); }

void Channel::RemoveClientNick(std::string nick){
    std::map<int, Client>::iterator it = this->clients.begin();

    while (it != this->clients.end()){
        if (it->second.GetNickname() == nick) {
            clients.erase(it->second.GetFd());
            return ;
        }
        it++;
    }
}

void Channel::RemoveAdmin(std::string nick){
    std::map<int, Client>::iterator it = this->admins.begin();

    while (it != this->clients.end()){
        if (it->second.GetNickname() == nick){
            clients.erase(it->second.GetFd());
            return ;
        }
        it++;
    }
}

void Channel::RemoveInvited(std::string nick){
    std::map<int, Client>::iterator it = this->invited.begin();

    while (it != this->clients.end()){
        if (it->second.GetNickname() == nick) {
            clients.erase(it->second.GetFd());
            return ;
        }
        it++;
    }
}

//----------Helper Functions--------------

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

bool Channel::GetClientInChannel(const std::string &nickname) 
{
    for (std::map<int, Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it) {
        if (it->second.GetNickname() == nickname) {
            return true;
        }
    }

    for (std::map<int, Client>::iterator it = this->admins.begin(); it != this->admins.end(); ++it) {
        if (it->second.GetNickname() == nickname) {
            return true;
        }
    }

    return false;
}

bool Channel::IsAdmin(int fd) {
    std::map<int, Client>::iterator it = this->admins.begin();


    while (it != this->admins.end()){
        if (it->second.GetFd() == fd)
            return (true);
        it++;
    }

    return (false);
}