#ifndef SERVER_HPP
#define SERVER_HPP

#include "Libraries.hpp"

using std::cout;
using std::endl;

class Client;
class Channel;

class Server {
    tm *time;
    int port;
    int server_socket;
    static bool signal;
    std::string password;
    std::map<int, Client> clients;
    std::vector<struct pollfd> fds;
    std::map<std::string, Channel> channels;

    public:
        Server();
        Server(int port, std::string password);

        void CloseFds();
        void ServerInit();
        void AcceptNewClient();
        void ReceiveNewData(int fd);
        void ClearClients(int fd);
        void ParseCmd(std::string &cmd, int fd);
        void SendMessages(int fd);
        Client *GetClient(int fd);
        Channel *GetChannel(std::string topic);
        std::vector<std::string> SplitBuffer(std::string str);

        std::map<int, Client> getClients() const;
        std::map<std::string, Channel> getChannels() const;
        std::string getTime() const;

        void setTime();

        bool addClient(Client &client);
        bool addChannel(Channel &channel);
        bool removeClient(int fd);
        bool removeChannel(std::string name);
        bool registered(int fd);
        
        void sendResponse(std::string response, int fd);
        static void SignalHandler(int signum);
        void    HandleRegistration(Client &client, std::string buffer);


};

#endif // !SERVER_HPP