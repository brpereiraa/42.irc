#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Libraries.hpp"

using std::cout;
using std::endl;

class Client {
    int fd;
    bool log;
    std::string buff;
    std::vector<std::string> vect;
    std::string ip_add;
    std::string nickname;
    std::string username;
    std::string password;
    std::string temp;
    //bool isOperator;

    public:
        Client();
        Client(std::string nickname, std::string username);
        int GetFd() const;
        const std::string GetIpAdd() const;
        const std::string GetNickname() const;
        const std::string GetUsername() const;
        const std::string GetBuffer() const;
        const std::string GetTemp() const;
        const std::vector<std::string> GetVect();
        const std::string GetPassword() const;
        bool GetLoggedIn () const;

        void SetFd(int fd);
        void SetNickname(std::string nick);
        void SetUsername(std::string user);
        void SetPassword(std::string pass);
        void SetBuffer(std::string buff);
        void SetTemp(std::string temp);
        void SetIpAdd(std::string ip_add);
        void SetLogged(bool log);
        
        void VectAdd(std::string line);
        void VectReset();

		//bool operator==(const Client &client);	
};
#endif // !CLIENT_HPP