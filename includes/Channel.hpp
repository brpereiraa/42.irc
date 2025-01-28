#ifndef CHANNEL_H
# define CHANNEL_H

#include "Libraries.hpp"

class Channel {
	private:
		std::string topic;
		std::string password;
		std::vector<Client> clients;
		std::vector<Client> admin;

	public:
		Channel();
		Channel(const std::string topic);
		~Channel();

		const std::string GetTopic() const;
		const std::string GetPassword() const;
		const std::vector<Client> GetClients() const;
		const std::vector<Client> GetAdmins() const;

		void SetTopic(std::string topic);
		void SetPassword(std::string password);
};

#endif