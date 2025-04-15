/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrichard <mrichard@student.42porto.pt>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 19:40:11 by mrichard          #+#    #+#             */
/*   Updated: 2025/04/15 20:29:27 by mrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdlib.h>

#include "Libraries.hpp"

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
	// Channel *ch = new Channel("name");

	// std::map<std::string, Client>::iterator it = ch->clients.begin();
	// while(it  != ch->clients.end()){
	// 	std::cout << it->first << " dw" << it->second.GetUsername() << std::endl;
	// 	it++;
	// }

    if (argc != 3)
    {
        cout << "Invalid number of params!" << endl;
        return 1;
    }

    Server ser(atoi(argv[1]), argv[2]);
	std::cout << "---- SERVER ----" << std::endl;
	try{
		signal(SIGINT, Server::SignalHandler); //-> catch the signal (ctrl + c)
		signal(SIGQUIT, Server::SignalHandler); //-> catch the signal (ctrl + \)
		signal(SIGPIPE, Server::SignalHandler);
		ser.ServerInit(); //-> initialize the server
	}
	catch(const std::exception& e){
		ser.CloseFds(); //-> close the file descriptors
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}
