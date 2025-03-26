#include "ACommands.hpp"

Mode::Mode(Server &server) : ACommands(server) 
{
    this->server = server;
}

void Mode::execute(int fd, const std::string &line)
{
    std::istringstream stream(line);
    std::string word;
    std::string target, modes;
    std::vector<std::string> args;
    int i;

    i = 0;
    while (stream >> word){
        i++;
        if (i == 2)
            target = word;
        else if (i == 3)
            modes = word;
        else if (i > 3)
            args.push_back(word);
    };

    if (target.empty()){
        std::cout << "Missing target" << std::endl;
        return ;
    }

    if (modes.empty()){
        std::cout << "Missing modes" << std::endl;
        return ;
    }
    
    if (target[0] != '#'){
        std::cout << "Only channel modes are available" << std::endl;
        return ;
    }
    
    this->channel(fd, target, modes, args);
    
}

void Mode::channel(int fd, const std::string &target, std::string &modes, std::vector<std::string> args)
{
    (void) args;
    (void) target;
    (void) fd;

    std::string::iterator str_it = modes.begin();
    // std::vector<std::string>::iterator arg_it = args.begin();

    while (str_it != modes.end()){
        if (*str_it == 'i')
            ;
        if (*str_it == 't')
            ;        
        if (*str_it == 'k')
            ;
        if (*str_it == 'o')
            ;
        if (*str_it == 'l')
            ;
        str_it++;
    }
}