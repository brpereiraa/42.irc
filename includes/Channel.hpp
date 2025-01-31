#ifndef CHANNEL_H
# define CHANNEL_H

#include "Libraries.hpp"

class Channel {
	private:
		std::string topic;
		std::string password;
		std::map<std::string, Client> admins;
		std::map<std::string, Client> clients;

	public:
		Channel();
		Channel(const std::string topic);
		~Channel();

		const std::string GetTopic() const;
		const std::string GetPassword() const;
		const std::map<std::string, Client> GetClients() const;
		const std::map<std::string, Client> GetAdmins() const;

		void SetTopic(std::string topic);
		void SetPassword(std::string password);

		void AddClient(Client &client);
		void RemoveClient(Client &client);


};

#endif