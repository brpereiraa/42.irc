#include "Parser.hpp"

void Handler(int fd, std::string line, Server &server)
{
    ACommands *command = NULL;
    
    size_t pos = line.find('\r');
    if (pos != std::string::npos)
    line.erase(pos);
    
    std::istringstream iss(line);
    std::string cmds;
    std::getline(iss, cmds, ' ');

    if (cmds.empty())
        return;
    if (cmds == "USER")
        command = new User(server);
    if (cmds == "NICK")
        command = new Nick(server);
    if (cmds == "PASS")
        command = new Pass(server);
    if (cmds == "JOIN")
        command = new Join(server);
    if (cmds == "KICK")
        command = new Kick(server);
    if (cmds == "INVITE")
        command = new Invite(server);
    if (cmds == "TOPIC")
        command = new Topic(server);
    if (cmds == "MODE")
        command = new Mode(server);
    if (cmds == "PING")
        command = new Pong(server);
    if (cmds == "PRIVMSG")
        command = new Privmsg(server);
        
    if (command) { 
        command->execute(fd, line);
        delete command;
    }
}

bool checkname(std::string name)
{
    size_t i = 0;

    if (name.empty())
        return (false);
    if (name[0] == '#')
        return (false);
    for(i = 0; i < name.size(); i++)
        if(std::isspace(name[i]))
            return(false);
    return(true);
}