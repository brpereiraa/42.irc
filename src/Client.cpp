#include "Client.hpp"

Client::Client() {
	this->log = false;
}

Client::Client(std::string nickname, std::string username){
	this->log = false;
	this->nickname = nickname;
	this->username = username;
}

int Client::GetFd() { return this->fd; }
const std::string Client::GetIpAdd() const { return this->ip_add; }
const std::string Client::GetNickname() const { return this->nickname; }
const std::string Client::GetUsername() const { return this->username; }
const std::string Client::GetBuffer() const { return this->buff; }
const std::string Client::GetPassword() const { return this->password; }
bool Client::GetLoggedIn() const { return this->log; }

void Client::SetFd(int fd) { this->fd = fd; }
void Client::SetIpAdd(std::string ip_add) { this->ip_add = ip_add; }
void Client::SetBuffer(char *buff) { this->buff += buff; }
void Client::SetNickname(std::string nick) { this->nickname = nick; }
void Client::SetUsername(std::string user) { this->username = user; }
void Client::SetPassword(std::string pass) { this->password = pass; }
void Client::SetLogged(bool log) { this->log = log; }