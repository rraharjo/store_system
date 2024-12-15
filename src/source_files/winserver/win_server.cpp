#include "winserver/win_server.hpp"

auto handle_send_thread = [](SOCKET *client_sock, std::mutex *mtx, std::string command)
{
    int i_result;
    char send_buff[SEND_BUFF] = "Done operation: ";
    size_t cur_send_len = strlen(send_buff);
    const size_t command_len = strlen(command.c_str());
    memcpy(send_buff + cur_send_len, command.c_str(), strlen(command.c_str()));
    send_buff[cur_send_len + command_len] = '\0';
    std::unique_lock<std::mutex> process_lock(*mtx);
    std::cout << command << " process successful!" << std::endl;
    process_lock.unlock();
    i_result = send(*client_sock, send_buff, (int)strlen(send_buff), 0);
    if (i_result == SOCKET_ERROR)
    {
        int error_code = WSAGetLastError();
        throw std::runtime_error("Error on send() with error code " + std::to_string(error_code) + "\n");
    }
};

auto handle_recv_thread = [](SOCKET *client_sock, std::mutex *mtx, int *num_of_client)
{
    int i_result = 0;
    char recv_buff[RECV_BUFF];
    do
    {
        memset(&recv_buff, '\0', i_result);
        i_result = recv(*client_sock, recv_buff, RECV_BUFF, 0);
        if (i_result > 0)
        {
            std::string command = recv_buff;
            //Send receive buff to driver
            std::thread send_thread = std::thread(handle_send_thread, client_sock, mtx, command);
            send_thread.detach();
        }
        else if (i_result == 0)
        {
            break;
        }
        else
        {
            int error_code = WSAGetLastError();
            throw std::runtime_error("Error on recv() with error code " + std::to_string(error_code) + "\n");
        }
    } while (i_result > 0);
    closesocket(*client_sock);
    delete client_sock;
    std::unique_lock<std::mutex> my_lock(*mtx);
    *num_of_client -= 1;
    my_lock.unlock();
    std::cout << "a client disconnected..." << std::endl;
};



winnetwork::WinTCPServer::WinTCPServer(std::string ip_address, std::string port_number) : ip_address(ip_address), port_number(port_number)
{
    this->network_fam = AF_INET;
    this->socket_type = SOCK_STREAM;
    this->protocol = IPPROTO_TCP;
}

void winnetwork::WinTCPServer::init_socket()
{
    int i_result;
    i_result = WSAStartup(MAKEWORD(2, 2), &this->wsa_data);
    if (i_result != 0)
    {
        throw std::runtime_error("Error on WSAStartup() with error code " + std::to_string(i_result) + "\n");
    }

    struct addrinfo hint, *addrinfo_result;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = this->network_fam;
    hint.ai_protocol = this->protocol;
    hint.ai_socktype = this->socket_type;
    i_result = getaddrinfo(this->ip_address.c_str(), this->port_number.c_str(), &hint, &addrinfo_result);
    if (i_result != 0)
    {
        WSACleanup();
        throw std::runtime_error("Error on getaddrinfo() with error code " + std::to_string(i_result) + "\n");
    }

    this->listen_socket = socket(addrinfo_result->ai_family, addrinfo_result->ai_socktype, addrinfo_result->ai_protocol);
    if (this->listen_socket == INVALID_SOCKET)
    {
        int error_code = WSAGetLastError();
        WSACleanup();
        throw std::runtime_error("Error on socket() with error code " + std::to_string(error_code) + "\n");
    }

    i_result = bind(this->listen_socket, addrinfo_result->ai_addr, sizeof(*addrinfo_result));
    if (i_result == SOCKET_ERROR)
    {
        int error_code = WSAGetLastError();
        WSACleanup();
        freeaddrinfo(addrinfo_result);
        throw std::runtime_error("Error on bind() with error code " + std::to_string(error_code) + "\n");
    }
    freeaddrinfo(addrinfo_result);
}

void winnetwork::WinTCPServer::start_server()
{
    int i_result;

    this->init_socket();

    i_result = listen(this->listen_socket, SOMAXCONN);
    if (i_result == SOCKET_ERROR)
    {
        int error_code = WSAGetLastError();
        WSACleanup();
        throw std::runtime_error("Error on listen() with error code " + std::to_string(error_code) + "\n");
    }
    std::cout << "listening at port " << this->port_number << std::endl;
    // TODO: Handle thread
    while (true)
    {
        SOCKET *client_sock = new SOCKET();
        struct sockaddr_in client_addr;
        int client_size = sizeof(client_addr);
        *client_sock = INVALID_SOCKET;
        *client_sock = accept(this->listen_socket, (sockaddr *)&client_addr, &client_size);
        if (*client_sock == SOCKET_ERROR)
        {
            printf("Accept failed %d\n", WSAGetLastError());
            closesocket(this->listen_socket);
            WSACleanup();
            break;
        }

        std::unique_lock<std::mutex> my_lock(this->my_mtx);
        if (this->num_of_client >= this->max_client)
        {
            char max_client_msg[32] = "max client reached...";
            i_result = send(*client_sock, max_client_msg, strlen(max_client_msg), 0);
            my_lock.unlock();
            closesocket(*client_sock);
            delete client_sock;
            continue;
        }

        this->num_of_client++;
        my_lock.unlock();
        std::thread new_thread = std::thread(handle_recv_thread, client_sock, &this->my_mtx, &this->num_of_client);
        new_thread.detach();
        std::cout << "A new client is connected" << std::endl;
    }
}