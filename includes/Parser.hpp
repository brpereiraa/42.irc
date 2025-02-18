#ifndef PARSER_HPP
#define PARSER_HPP

#include "Libraries.hpp"

class Server;

void Handler(int fd, std::string line, Server &server);

#endif // !PARSER_HPP