#include "winserver/win_server.hpp"

//TODO: Declare a thread handler here

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

void winnetwork::WinTCPServer::start_server(){
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
    //TODO: Handle thread
}