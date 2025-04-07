#include "ACommands.hpp"


Kick::Kick(Server &server) : ACommands(server) 
{
    this->server = server;
}

//verifica se o cliente e admin
bool Kick::isAdmin(const Client &client, const Channel &channel) 
{
    const std::map<int, Client>& admins = channel.GetAdmins();
    for (std::map<int, Client>::const_iterator it = admins.begin(); it != admins.end(); ++it) 
    {
        if (it->second.GetFd() == client.GetFd() &&
            it->second.GetNickname() == client.GetNickname() &&
            it->second.GetUsername() == client.GetUsername()) 
            {
            return true;
        }
    }
    return false;
}


void Kick::execute(int fd, const std::string& line)
{
    cout << fd << endl;
    cout << line << endl;
    cout << "entrou kick" << endl;
    // Split the line into arguments
    std::stringstream line_stream(line);
    std::string temp;
    std::vector<std::string> args;
    

    while (line_stream >> temp) {
        args.push_back(temp);
    }

    if (args.size() < 2) {
        std::cerr << "Error: Missing parameters\n";
        return;
    }

    Client client;

    if (args.size() >= 3 && (args[2][0] != ':' || args[2].size() > 1)) 
    {
        std::stringstream reason_stream;

        for (size_t i = 2; i < args.size(); ++i) 
        {
            reason_stream << args[i] << " ";
        }

        std::string reason = reason_stream.str();
        std::cout << "Reason: " << reason << std::endl;
    }

    std::cout << "Executed kick command for client: " << client.GetNickname() << std::endl;
}