#ifndef ACOMMANDS_HPP
#define ACOMMANDS_HPP

#include "Libraries.hpp"

class Server;

class ACommands {
    protected:
        Server &server;

	public:
        ACommands(Server &server);
		virtual ~ACommands() {};
		virtual void execute(int fd, const std::string& line) = 0;
};

class Join : public ACommands {
    public:
        Join(Server &server);
        void execute(int fd, const std::string& line);
        bool initialChecksJoin(int fd, Client* client, Channel* channel, const std::string& key);
        void joinChannel(int fd, const std::string &channelName, const std::string &key);
        void createAndJoinChannel(int fd, const std::string &channelName, const std::string &key);
};

class Privmsg : public ACommands {
    public:
        Privmsg(Server &server);
        void execute(int fd, const std::string& line);
};

class Kick : public ACommands {
    public:
        Kick(Server &server);
        bool isAdmin(const Client &client, const Channel &channel);
        void execute(int fd, const std::string& line);
};

class Invite : public ACommands {
    public:
        Invite(Server &server);
        void execute(int fd, const std::string& line);
};

class Topic : public ACommands {
    public:
        Topic(Server &server);
        void execute(int fd, const std::string& line);
};

class Mode : public ACommands {
    public:
        Mode(Server &server);
        void execute(int fd, const std::string& line);
        void channel(int fd, const std::string &target, std::string &modes, std::vector<std::string> args);
        void user(int fd, const std::string &targer, std::string &modes, std::vector<std::string> args);
};

class Pong : public ACommands {
    public:
        Pong(Server &server);
        void execute (int fd, const std::string &line);
};

class Nick : public ACommands {
    public:
        Nick(Server &server);
        void execute (int fd, const std::string &line);
        void SendSharedChannels(Client *client, std::string nickname, int fd);

};

class User : public ACommands {
    public:
        User(Server &server);
        void execute (int fd, const std::string &line);

};

class Pass : public ACommands {
    public:
        Pass(Server &server);
        void execute (int fd, const std::string &line);
        
};

class Part : public ACommands {
    public:
        Part(Server &server);
        void execute (int fd, const std::string &line);
        
};

class Quit : public ACommands {
    public:
        Quit(Server &server);
        void execute (int fd, const std::string &line);
        
};

#endif // !ACOMMANDS_HPP