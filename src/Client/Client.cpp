#include "Client.hpp"

Client::Client() {}

int Client::GetFd() { return this->fd; }

std::string Client::GetIpAdd() { return this->ip_add; }

std::string Client::GetNickname() { return this->nickname; }

std::string Client::GetUsername() { return this->username; }

std::string Client::GetBuffer() { return this->buff; }

void Client::SetFd(int fd) { this->fd = fd; }

void Client::SetIpAdd(std::string ip_add) { this->ip_add = ip_add; }

void Client::SetBuffer(char *buff) { this->buff += buff; }

void Client::SetNickname(std::string nick) { this->nickname = nick; }

void Client::SetUsername(std::string user) { this->username = user; }
