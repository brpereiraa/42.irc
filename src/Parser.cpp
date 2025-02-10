#include "Parser.hpp"

//poderia ser retorno void, mas o que faz no cmds.empty()?
bool Handler(int fd, std::string line)
{
    ACommands *command = NULL;

    (void)fd;
    size_t pos = line.find('\r');
    if (pos != std::string::npos)
            line.erase(pos);    

    std::istringstream iss(line);
    std::string cmds;
    
    std::getline(iss, cmds, ' ');
    // da parse no input para comandos e argumentos
    // e verifica se o input first nao esta vazio ex KICK ou INVITE etc
    // t_input input = getInput(line);
    if (cmds.empty())
        return false;
    if (cmds == "JOIN")
        command = new Join();
    if (cmds == "KICK")
        command = new Kick();
    if (cmds == "INVITE")
        command = new Invite();
    if (cmds == "TOPIC")
        command = new Topic();
    if (cmds == "MODE")
        command = new Mode();
        
    if (command) {
        command->execute(fd, line); // Chama o método polimórfico
        delete command; // Libera a memória manualmente
    }
    return true;
}