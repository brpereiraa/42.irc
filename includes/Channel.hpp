#ifndef CHANNEL_H
# define CHANNEL_H

#include "Libraries.hpp"

class Channel {
	private:
		std::string topic;
		std::string password;
		std::map<int, Client> admins;
		std::map<int, Client> clients;

	public:
		Channel();
		Channel(const std::string topic);
		~Channel();

		const std::string GetTopic() const;
		const std::string GetPassword() const;
		const std::map<int, Client> GetClients() const;
		const std::map<int, Client> GetAdmins() const;

		void SetTopic(const std::string topic);
		void SetPassword(const std::string password);

		void AddClient(Client &client);
		void RemoveClient(int fd);
};

#endif