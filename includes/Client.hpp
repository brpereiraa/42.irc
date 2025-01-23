#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Libraries.hpp"

using std::cout;
using std::endl;

class Client {
    int fd;
    std::string ip_add;

    public:
        Client();
        int GetFd();
        void SetFd(int fd);
        void SetIpAdd(std::string ip_add);
};

#endif // !CLIENT_HPP