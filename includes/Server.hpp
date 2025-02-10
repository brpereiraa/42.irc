#ifndef SERVER_HPP
#define SERVER_HPP

#include "Libraries.hpp"

using std::cout;
using std::endl;

class Client;
class Channel;

class Server {
    static bool signal;
    int port;
    int server_socket;
    std::vector<struct pollfd> fds;
    std::map<int,  Client> clients;
    std::map<std::string, Channel> channels;
    tm *time;

    public:
        Server();
        Server(int port);

        void CloseFds();
        void ServerInit();
        void AcceptNewClient();
        void ReceiveNewData(int fd);
        void ClearClients(int fd);
        void ParseCmd(std::string &cmd, int fd);
        //Client *GetClient(int fd);
        std::vector<std::string> SplitBuffer(std::string str);

        std::map<int, Client> getClients() const;
        std::map<std::string, Channel> getChannels() const;
        std::string getTime() const;

        void    setTime();

        bool    addClient(Client &client);
        bool    addChannel(Channel &channel);
        bool    removeClient(int fd);
        bool    removeChannel(std::string name);
        
        static void SignalHandler(int signum);

};

#endif // !SERVER_HPP