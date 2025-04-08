#ifndef CHANNEL_H
# define CHANNEL_H

#include "Libraries.hpp"

class Server;

class Channel {
	private:
		int usr_limit;
		bool inv_only;
		bool topic_restricted;
		std::string name;
		std::string topic;
		std::string password;
		std::map<int, Client> admins;
		std::map<int, Client> clients;
		std::map<int, Client> invited;

	public:
		Channel();
		Channel(const std::string topic);
		~Channel();

		const std::string GetTopic() const;
		const std::string GetName() const;
		const std::string GetPassword() const;
		const std::map<int, Client> GetClients() const;
		const std::map<int, Client> GetAdmins() const;
		const std::map<int, Client> GetInvited() const;
		bool GetInvite() const;
		bool GetTopicRestricted() const;
		int GetLimit() const;
		
		Client	*GetAdminByNick(std::string nickname);
		Client	*GetClientByNick(std::string nickname);
		Client	*GetInvitedByNick(std::string nickname);

		void SetTopic(const std::string topic);
		void SetPassword(const std::string password);
		void SetInvite(const bool value);\
		void SetTopicRestricted(const bool value);
		void SetLimit(const int limit );

		void AddClient(Client &client);
		void AddAdmin(Client &client);
		void AddInvited(Client &client);

		void RemoveClient(int fd);
		void RemoveClientNick(std::string nick);

		void RemoveAdmin(std::string nick);

		void RemoveInvited(std::string nick);

		void SendToAll(const std::string &reply, int fd, Server &server);
		void ClearClients();
		bool GetClientInChannel(const std::string &nickname);
		bool IsAdmin(int fd);
		std::string ClientChannelList();

};

#endif