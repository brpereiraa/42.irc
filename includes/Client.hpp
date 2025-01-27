#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Libraries.hpp"

using std::cout;
using std::endl;

class Client {
    int fd;
    std::string buff;
    std::string ip_add;
    std::string nickname;
    std::string username;

    public:
        Client();
        int GetFd();
        std::string GetIpAdd();
        std::string GetNickname();
        std::string GetUsername();
        std::string GetBuffer();

        void SetFd(int fd);
        void SetNickname(std::string nick);
        void SetUsername(std::string user);
        void SetBuffer(char *buff);
        void SetIpAdd(std::string ip_add);
};
#endif // !CLIENT_HPP