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
        void joinChannel(int fd, size_t i, std::vector<std::string> tokens);
        void createAndJoinChannel(int fd, size_t i, std::vector<std::string> tokens);
};

class Kick : public ACommands {
    public:
        Kick(Server &server);
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
};

#endif // !ACOMMANDS_HPP