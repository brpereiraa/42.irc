#include "Server.hpp"

bool Server::signal = false;

Server::Server() {}

Server::Server(int port, std::string password) 
{
    this->server_socket = -1;
    this->port = port;
    this->password =  password;
}

void Server::sendResponse(std::string response, int fd) {
    if (send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Response send() failed" << std::endl;
}

std::string Server::getTime() const {
    std::stringstream ss;

    ss << this->time->tm_mon + 1 << "/" << this->time->tm_mday << " " << this->time->tm_hour << ":" << this->time->tm_min << ":" << this->time->tm_sec;

    return ss.str();
}

void Server::setTime() {
    time_t now = std::time(0);
    this->time = localtime(&now);
}

bool Server::addChannel(Channel &channel) {
	if (this->channels.count(channel.GetTopic())) {
		std::cout << "Channel with topic  " << channel.GetTopic() << " already exists" << std::endl;
        return (false);
    }
	this->channels[channel.GetTopic()] = channel;
    return (true);
}

bool Server::addClient(Client &client) {
	if (this->clients.count(client.GetFd())) {
		std::cout << "Client with topic  " << client.GetFd() << " already exists" << std::endl;
        return (false);
    }
	this->clients[client.GetFd()] = client;
    return (true);
}

bool Server::removeChannel(std::string name) {
    if (this->channels.count(name)){
        this->channels.erase(name);
        return (true);
    }
    std::cout << "Channel with name " << name << " doesn't exist" << std::endl;
    return (false);
}

bool Server::removeClient(int fd) {
    if (this->clients.count(fd)){
        this->clients.erase(fd);
        return (true);
    }
    std::cout << "Channel with name " << fd << " doesn't exist" << std::endl;
    return (false);
}

std::map<std::string, Channel> Server::getChannels() const {
    return this->channels;
}

void Server::SignalHandler(int signum)
{
    (void)signum;
    cout << endl << "Signal received!" << endl;
    Server::signal = true;
}

void Server::ClearClients(int fd)
{
    std::map<int, Client>::iterator it = this->clients.begin();
	
    for (size_t i = 0; i < fds.size(); i++) //remove da poll
    {
        if (fds[i].fd == fd)
        {
            fds.erase(fds.begin() + i);
            break;
        }
    }

    while (it != this->clients.end()) {
        if (it->second.GetFd() == fd){
            this->removeClient(it->first);
            break;
        }
		it++;
	}
}

//acho que isso pode ir para utils
void Server::CloseFds()
{
    std::map<int, Client>::iterator it = this->clients.begin();

    while (it != this->clients.end()){
        close(it->second.GetFd());
		it++;
	}
    if (server_socket != -1)
    {
        cout << "Server <" << server_socket << "> disconnected" << endl;
        close(server_socket);
    }
}

Client *Server::GetClient(int fd)
{
	for (size_t i = 0; i < this->clients.size(); i++){
		if (this->clients[i].GetFd() == fd)
			return &this->clients[i];
	}
	return NULL;
}

Channel *Server::GetChannel(std::string topic)
{
    std::map<std::string, Channel>::iterator it = this->channels.find(topic);
    if (it != this->channels.end()) {
        return &it->second;
    }
	return NULL;
}

void Server::AcceptNewClient()
{
    Client cli("", "");
    struct sockaddr_in cli_add;
    struct pollfd new_poll;
    socklen_t len = sizeof(cli_add);

    int incofd = accept(server_socket, (sockaddr *)&(cli_add), &len); //aceitar o novo cliente
    if (incofd == -1)
        {cout << "accept() failed" << endl;return;}

    if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1) //non blocking 
        {cout << "fcntl() failed" << endl; return;}

    new_poll.fd = incofd; //adicionar o socket do client ao pollfd
    new_poll.events = POLLIN;
    new_poll.revents = 0;

    cli.SetFd(incofd); //settar o fd do cliente
    cli.SetIpAdd(inet_ntoa((cli_add.sin_addr))); //converte o endereco de ip para string e setta
    this->addClient(cli); //adiciona novo cliente a lista !!!
    fds.push_back(new_poll); //adiciona o socket do cliente ao pollfd

    cout << "Client <" << incofd << "> connected" << endl;
}

void Server::ReceiveNewData(int fd)
{
    char buff[1024]; //para os dados recebidos
    memset(buff, 0, sizeof(buff)); //limpar o buffer
    Client *cli = new Client();
    std::vector<std::string> cmd;

    ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0); //recebe os dados

    if (bytes <= 0) //cliente desconectou
    {
        cout << "Client <" << fd << "> disconnected" << endl;
        ClearClients(fd);
        close(fd);
    }
    else //imprime os dados recebidos
    {
        //associar o buffer ao cliente aqui
        buff[bytes] = '\0';
        //fazer parse/split do buffer, e para cada posicao do vetor retornado fazer parse do cmd
        //here you can add your code to process the received data: parse, check, authenticate, handle the command, etc...
        cli->SetBuffer(buff);
        cmd = SplitBuffer(cli->GetBuffer());

        std::vector<std::string>::iterator it = cmd.begin();

        while(it != cmd.end()) {
            if (it->substr(0, 4) == "NICK") {
                std::string nickname = it->substr(5);
                this->clients[fd].SetNickname(nickname);
                cout << "Client set nickname: " << nickname << endl;
            }
            else if (it->substr(0, 4) == "USER") {
                std::string realName = it->substr(5); // USER <username> <hostname> <servername> :<real name>
                size_t colonPos = realName.find(":");
                if (colonPos != std::string::npos) {
                    realName = realName.substr(colonPos + 1); // Remove the "real name" part after the colon
                }
                this->clients[fd].SetUsername(realName);  // Set the real name
                cout << "Client set real name: " << realName << endl;
            } else if (it->substr(0, 4) == "PASS") {
                std::string password = it->substr(5);
                this->clients[fd].SetPassword(password);
            }
            it++;
        }
        
        if (this->password != "" && this->clients[fd].GetPassword() != this->password) {
            std::string msg = ":myserver 464 " + this->clients[fd].GetNickname() + " :Password incorrect\r\n";
            send(fd, msg.c_str(), msg.size(), 0);
            return ;
        } else if (this->clients[fd].GetLoggedIn() == false) {
            SendMessages(fd);
            this->clients[fd].SetLogged(true);
        }

        // if (registered(fd))
        //     SendMessages(fd);

        for (unsigned long i = 0; i < cmd.size(); i++)
            Handler(fd, cli->GetBuffer(), *this);
    }
}

void Server::ServerInit()
{
    struct sockaddr_in server_address;
	struct pollfd pollfd;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(this->port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
        ThrowException("failed to create socket");

    int en = 1;
    //reutilizar endereco para quando reiniciar o programa
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
        ThrowException("failed to set option (SO_REUSEADDR) on socket");
    //settar non blocking (execucao nao trava se uma acao nao finalizar)
    if (fcntl(server_socket, F_SETFL, O_NONBLOCK) == 1)
        ThrowException("failed to set option (O_NONBLOCK) on socket");
    //associar o socket ao endereco
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
        ThrowException("failed to bind the socket");
    //receber conexoes, setted para ser socket passivo (apenas esperar para ouvir)
    if (listen(server_socket, SOMAXCONN) == -1)
        ThrowException("listen() failed");

    this->setTime();

    pollfd.fd = server_socket; //socket para ser monitorado
    pollfd.events = POLLIN; //prepara para ler dados
    pollfd.revents = 0; //eventos que aconteceram, sera preenchido pelo poll
    fds.push_back(pollfd); //adiciono o socket criado ao FDS, essa sera a lista monitorada pelo poll

    while (Server::signal == false)
    {
        if ((poll(&fds[0], fds.size(), -1)) == -1 && Server::signal == false) //monitora a lista de sockets, espera por um evento
            ThrowException("poll() failed");

        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & POLLIN) //se tiver um evento neste socket
            {
                if (fds[i].fd == server_socket)
                    AcceptNewClient();
                else
                    ReceiveNewData(fds[i].fd);
            }
        }
    }
    CloseFds();
}

std::vector<std::string> Server::SplitBuffer(std::string str)
{
	std::vector<std::string> vec;
	std::istringstream stm(str);
	std::string line;
	while(std::getline(stm, line))
	{
		size_t pos = line.find_first_of("\r\n");
		if(pos != std::string::npos)
			line = line.substr(0, pos);
		vec.push_back(line);
	}
	return vec;
}

void Server::SendMessages(int fd)
{
    this->clients[fd].SetLogged(true);

    std::string welcomeMsg = ":myserver 001 " + this->clients[fd].GetIpAdd() + " :Welcome to the IRC server " + this->clients[fd].GetNickname() + "\r\n";
    send(this->clients[fd].GetFd(), welcomeMsg.c_str(), welcomeMsg.size(), 0);

    welcomeMsg = ":myserver 002 " + this->clients[fd].GetIpAdd() + " :Your host is running IRC Server, running version 1.0 \r\n";
    send(this->clients[fd].GetFd(), welcomeMsg.c_str(), welcomeMsg.size(), 0);

    welcomeMsg = ":myserver 003 " + this->clients[fd].GetIpAdd() + " :This server was created  " + this->getTime() +  "\r\n";
    send(this->clients[fd].GetFd(), welcomeMsg.c_str(), welcomeMsg.size(), 0);

    // 004 - Informações do servidor e modos suportados
    welcomeMsg = ":myserver 004 " + this->clients[fd].GetNickname() + " IRC 1.0 oiw btkl case\r\n";
    send(this->clients[fd].GetFd(), welcomeMsg.c_str(), welcomeMsg.size(), 0);

    // 005 - Recursos do servidor suportados
    // TODO: rever esta mensagem
    welcomeMsg = ":myserver 005 " + this->clients[fd].GetNickname() + " CHANTYPES=# PREFIX=(o,v)@+ MAXNICKLEN=30 :are supported by this server\r\n";
    send(this->clients[fd].GetFd(), welcomeMsg.c_str(), welcomeMsg.size(), 0);

    // 375 - Início do MOTD
    welcomeMsg = ":myserver 375 " + this->clients[fd].GetNickname() + " :- IRC Message of the day - AMELO\r\n";
    send(this->clients[fd].GetFd(), welcomeMsg.c_str(), welcomeMsg.size(), 0);

    // 372 - Linhas do MOTD
    welcomeMsg = ":myserver 372 " + this->clients[fd].GetNickname() + " :- Welcome to our IRC server!\r\n";
    send(this->clients[fd].GetFd(), welcomeMsg.c_str(), welcomeMsg.size(), 0);

    welcomeMsg = ":myserver 372 " + this->clients[fd].GetNickname() + " :- Please be respectful and follow the rules.\r\n";
    send(this->clients[fd].GetFd(), welcomeMsg.c_str(), welcomeMsg.size(), 0);

    welcomeMsg = ":myserver 372 " + this->clients[fd].GetNickname() + " :- Have fun chatting!\r\n";
    send(this->clients[fd].GetFd(), welcomeMsg.c_str(), welcomeMsg.size(), 0);

    // 376 - Fim do MOTD
    welcomeMsg = ":myserver 376 " + this->clients[fd].GetNickname() + " :End of MOTD command.\r\n";
    send(this->clients[fd].GetFd(), welcomeMsg.c_str(), welcomeMsg.size(), 0);
}

bool Server::registered(int fd) 
{
    if (GetClient(fd) && !GetClient(fd)->GetNickname().empty() && !GetClient(fd)->GetUsername().empty() && GetClient(fd)->GetNickname() != "*" && GetClient(fd)->GetLoggedIn())
		return false;
	return true;
}