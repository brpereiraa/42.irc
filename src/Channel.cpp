#include "Libraries.hpp"

Channel::Channel() {}
Channel::Channel(const std::string topic){ this->topic = topic; }

Channel::~Channel() {}

const std::string Channel::GetPassword() const { return this->password; }
const std::string Channel::GetTopic() const { return this->topic; }

void Channel::SetPassword(std::string password) { this->password = password; }
void Channel::SetTopic(std::string topic) { this->topic = topic; }

