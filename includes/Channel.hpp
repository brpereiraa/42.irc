#ifndef CHANNEL_H
# define CHANNEL_H

#include "Libraries.hpp"

class Server;

class Channel {
	private:
		int usr_limit;
		bool inv_only;
		std::string name;
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
		const std::string GetName() const;
		bool GetInvite() const;
		int GetLimit() const;

		void SetTopic(const std::string topic);
		void SetPassword(const std::string password);
		void SetInvite(const bool value);
		void SetLimit(const int limit );

		void AddClient(Client &client);
		void RemoveClient(int fd);
		void SendToAll(const std::string &reply, int fd, Server &server);
		void ClearClients();
		bool GetClientInChannel(std::string &nickname);
		std::string ClientChannelList();

};

#endif