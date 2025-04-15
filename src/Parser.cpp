#include "Parser.hpp"

void Handler(int fd, std::string line, Server &server)
{
    ACommands *command = NULL;
    Client *client;
    
    size_t pos = line.find('\r');
    if (pos != std::string::npos)
    line.erase(pos);
    
    std::istringstream iss(line);
    std::string cmds;
    std::getline(iss, cmds, ' ');

    client = server.GetClient(fd);
    
    std::cout << 
        (!client->GetNickname().empty() ? client->GetNickname() : "*") << 
        ": " << RED << line << RESET << std::endl;

    if (cmds.empty())
        return;
    else if (cmds == "USER")
        command = new User(server);
    else if (cmds == "NICK")
        command = new Nick(server);
    else if (cmds == "PASS")
        command = new Pass(server);
    else if (cmds == "JOIN")
        command = new Join(server);
    else if (cmds == "KICK")
        command = new Kick(server);
    else if (cmds == "INVITE")
        command = new Invite(server);
    else if (cmds == "TOPIC")
        command = new Topic(server);
    else if (cmds == "MODE")
        command = new Mode(server);
    else if (cmds == "PING")
        command = new Pong(server);
    else if (cmds == "PRIVMSG")
        command = new Privmsg(server);
    else if (cmds == "PART")
        command = new Part(server);
    else if (cmds == "QUIT")
        command = new Part(server);
        
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

std::string toLowerString(const std::string& str) {
    std::string result = str;
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = std::tolower(result[i]);
    }
    return result;
}

bool ends_with(const std::string& str, const std::string& suffix) {
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}