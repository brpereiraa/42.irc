#include "./includes/Server/Server.hpp"

Server::Server() 
{
    this->server_socket = -1;
}

//acho que isso pode ir para utils
void Server::CloseFds()
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        cout << "Client <" << clients[i].GetFd() << "> disconnected" << endl;
        close(clients[i].GetFd());
    }
    if (server_socket != -1)
    {
        cout << "Server <" << server_socket << "> disconnected" << endl;
        close(server_socket);
    }
}

void Server::AcceptNewClient()
{
    Client cli;
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
    clients.push_back(cli); //adiciona novo cliente a lista
    fds.push_back(new_poll); //adiciona o socket do cliente ao pollfd

    cout << "Client <" << incofd << "> connected" << endl;
}

void Server::ReceiveNewData(int fd)
{
    
}

bool Server::ServerInit(int port, std::string address)
{
    struct sockaddr_in server_address;
	struct pollfd pollfd;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
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

    pollfd.fd = server_socket; //socket para ser monitorado
    pollfd.events = POLLIN; //prepara para ler dados
    pollfd.revents = 0; //eventos que aconteceram, sera preenchido pelo poll
    fds.push_back(pollfd); //adiciono o socket criado ao FDS, essa sera a lista monitorada pelo poll

    while (1)
    {
        if ((poll(&fds[0], fds.size(), -1)) == -1) //monitora a lista de sockets, espera por um evento
            ThrowException("poll() failed");

        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & POLLIN) //se tiver um evento neste socket
                AcceptNewClient();
            else
                ReceiveNewData(fds[i].fd);
        }
    }

    CloseFds();
}