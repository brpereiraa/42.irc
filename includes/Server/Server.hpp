#include <iostream>
#include <vector> //-> for vector
#include <sys/socket.h> //-> for socket()
#include <sys/types.h> //-> for socket()
#include <netinet/in.h> //-> for sockaddr_in
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h> //-> for poll()
#include <csignal> //-> for signal()

#include "./includes/Client/Client.hpp"
#include "./src/Utils/Utils.cpp"

using std::cout;
using std::endl;

class Client;

class Server {
    int port;
    int server_socket;
    std::vector<Client> clients;
    std::vector<struct pollfd> fds;

    public:
        Server();
        void CloseFds();
        bool ServerInit(int port, std::string address);
        void AcceptNewClient();
        void ReceiveNewData(int fd);
};