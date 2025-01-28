#include "Libraries.hpp"

bool handler(int fd, std::string line)
{
/*  ∗ KICK - Eject a client from the channel
    ∗ INVITE - Invite a client to a channel
    ∗ TOPIC - Change or view the channel topic
    ∗ MODE - Change the channel’s mode:
        · i: Set/remove Invite-only channel
        · t: Set/remove the restrictions of the TOPIC command to channel operators
        · k: Set/remove the channel key (password)
        · o: Give/take channel operator privilege
           l: Set/remove the user limit to channel */

    // remove o /r
   size_t pos = line.find('\r');
   if (pos != std::string::npos)
   {
        line.erase(pos);
   }

    // da parse no input para comandos e argumentos
    // e verifica se o input first nao esta vazio ex KICK ou INVITE etc
   t_input input = getInput(line);
   if (input.first.empty())
   {
    return false;
   }
    if (input.first[0] == "KICK")
        return (KICK(fd, input, input.first[0]));
    if (input.first[0] == "INVITE")
        return (INVITE(fd, input, input.first[0]));
    if (input.first[0] == "TOPIC")
        return (INVITE(fd, input, input.first[0]));
    if (input.first[0] == "MODE")
        return (MODE(fd, input, input.first[0]));

}