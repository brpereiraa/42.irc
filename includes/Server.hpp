#ifndef SERVER_HPP
#define SERVER_HPP

#include "Libraries.hpp"

using std::cout;
using std::endl;

class Client;

class Server {
    static bool signal;
    int port;
    int server_socket;
    std::vector<Client> clients;
    std::vector<struct pollfd> fds;

    public:
        Server();
        Server(int port);
        void CloseFds();
        void ServerInit();
        void AcceptNewClient();
        void ReceiveNewData(int fd);
        void ClearClients(int fd);
        void ParseCmd(std::string &cmd, int fd);
        
        static void SignalHandler(int signum);

};

#endif // !SERVER_HPP