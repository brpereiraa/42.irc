#include "Client.hpp"

Client::Client() {}

int Client::GetFd()
{
    return this->fd;
}

void Client::SetFd(int fd)
{
    this->fd = fd;
}

void Client::SetIpAdd(std::string ip_add)
{
    this->ip_add = ip_add;
}