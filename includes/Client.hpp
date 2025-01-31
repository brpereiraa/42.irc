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
    //bool isOperator;

    public:
        Client();
        Client(std::string nickname, std::string username);
        int GetFd();
        const std::string GetIpAdd() const;
        const std::string GetNickname() const;
        const std::string GetUsername() const;
        const std::string GetBuffer() const;

        void SetFd(int fd);
        void SetNickname(std::string nick);
        void SetUsername(std::string user);
        void SetBuffer(char *buff);
        void SetIpAdd(std::string ip_add);

		bool operator==(const Client &client);	
};
#endif // !CLIENT_HPP