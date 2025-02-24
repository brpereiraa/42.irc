#include "ACommands.hpp"

Kick::Kick(Server &server) : ACommands(server) 
{
    this->server = server;
}

void Kick::execute(int fd, const std::string &line)
{
    cout << fd << endl;
    cout << line << endl;
    cout << "entrou kick" << endl;
}

// void Kick::execute(Client* client, std::vector<std::string> args)
// {
//     if(args.size() < 2)
//     {
//         std::cerr << "Error: " << client->GetNickname() << " missing paramters.\n";
//         return;
//     }

//     if(args.size() >= 3 && (args[2][0] != ':' || args[2].size() > 1))
//     {
//         std::stringstream reason_stream;

//         for (size_t i = 2; i < args.size(); ++i)
//         {
//             reason_stream << args[i] << " ";
//         }

//         std::string reason = reason_stream.str();
//     }
// }