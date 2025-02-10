#ifndef ACOMMANDS_HPP
#define ACOMMANDS_HPP

#include "Libraries.hpp"

class ACommands {
	public:
		virtual ~ACommands() {};
		virtual void execute(int fd, const std::string& line) = 0;
};

class Join : public ACommands {
    public:
        void execute(int fd, const std::string& line);
};

class Kick : public ACommands {
    public:
        void execute(int fd, const std::string& line);
};

class Invite : public ACommands {
    public:
        void execute(int fd, const std::string& line);
};

class Topic : public ACommands {
    public:
        void execute(int fd, const std::string& line);
};

class Mode : public ACommands {
    public:
        void execute(int fd, const std::string& line);
};

#endif // !ACOMMANDS_HPP