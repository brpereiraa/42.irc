#include "ACommands.hpp"
#include <string>

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

    bool value;

    std::string::iterator str_it = modes.begin();
    std::vector<std::string>::iterator arg_it = args.begin();

    Client *client = this->server.GetClient(fd);
    value = true;

    //Check if it's to remove or add
    if (*str_it == '-'){
        value = false; 
        str_it++;
    }

    if (*str_it == '+')
        str_it++;

    //handle modes
    while (str_it != modes.end()){
        //Handle set invite positive/negative
        if (*str_it == 'i'){
            if (!value)
                this->server.GetChannel(target)->SetInvite(false);
            else
                this->server.GetChannel(target)->SetInvite(true);
        }

        //
        else if (*str_it == 't')
            ;        

        //Handle channel password
        else if (*str_it == 'k'){
            //Remove password from channel
            if (!value)
                this->server.GetChannel(target)->SetPassword("");

            //Set password on channel
            else {
                //Check if there is argument for password
                if (arg_it == args.end()){
                    this->server.sendResponse(":myserver 461 " + client->GetNickname() + " MODE :Not enough parameters\r\n", fd);
                    str_it++;
                    continue ;
                }
                
                this->server.GetChannel(target)->SetPassword(*arg_it);
                arg_it++;
            }

        }
        else if (*str_it == 'o')
            ;
        else if (*str_it == 'l')
            ;
        else 
            ;
        str_it++;
    }
}

// void Mode::user(int fd, const std::string &targer, std::string &modes, std::vector<std::string> args)
// {

// }