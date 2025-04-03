#ifndef LIBRARIES_HPP
#define LIBRARIES_HPP

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
#include <cstring>
#include <sstream>
#include <bits/stdc++.h> 
#include <map>
#include <ctime>
#include <memory>

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Parser.hpp"
#include "ACommands.hpp"

#define CRLF "\r\n"

#define RPL_JOINMSG(nickname, username, channelname) \
    (":" + nickname + "!" + username + " JOIN " + channelname + CRLF)

#define RPL_NAMREPLY(nickname, channelname, clientslist) \
    (":myserver 353 " + nickname + " = " + channelname + " :" + clientslist + CRLF)

#define RPL_ENDOFNAMES(nickname, channelname) \
    (":myserver 366 " + nickname + " " + channelname + " :End of /NAMES list" + CRLF)

#define RPL_TOPICIS(nickname, channelname, topic) \
    (":myserver 332 " + nickname + " " + channelname + " :" + topic + CRLF)

#define ERR_NOSUCHCHANNEL(nickname, channel) \
    (":myserver 403 " + nickname + " " + channel + " :No such channel" + CRLF)
    
#define ERR_INVITEONLYCHAN(nickname, channel) \
    (":myserver 473 " + nickname + " " + channel + " :Cannot join channel (+i)" + CRLF)

#define ERR_TOOMANYCHANNELS(nickname, channel) \
    (":myserver 405 " + nickname + " " + channel + " :You have joined too many channels" + CRLF)

#define ERR_BADCHANNELKEY(nickname, channel) \
    (":myserver 475 " + nickname + " " + channel + " :Cannot join channel (+k) - bad key" + CRLF)

#define ERR_CHANNELISFULL(nickname, channel) \
    (":myserver 471 " + nickname + " " + channel + " :Cannot join channel (+l)" + CRLF)

#define ERR_NEEDMOREPARAMS(cmd) \
    (":myserver 461 " + cmd + " :Not enough parameters" + CRLF)

#define ERR_CHANOPRIVSNEEDED(nickname, channel) \
    (":myserver 482 " + nickname + " " + channel + " :You're not a channel operator" + CRLF)

#define RPL_TOPIC(nickname, channel, topic) \
    (":myserver 332 " + nickname + " " + channel + " :" + topic + CRLF)

#define RPL_NOTOPIC(nickname, channel) \
    (":myserver 331 " + nickname + " " + channel + " :No topic is set" + CRLF)

#define RPL_TOPICMSG(nickname, channel, topic) \
    (":" + nickname + " TOPIC " + channel + " :" + topic + CRLF)

#define RED "\033[0;31m"
#define RESET "\033[0m"

// typedef std::pair<t_args, std::string> t_input

inline void ThrowException(std::string msg)
{
    throw(std::runtime_error(msg));
}

std::string toLowerString(const std::string& str);

#endif // !LIBRARIES_HPP