#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <thread>
#define MY_PORT "8000"
#define LOCAL_IP "127.0.0.1"
#define BUFF_SIZE 512

auto handle_rcv = [](SOCKET *connect_sock)
{
    char recv_buff[BUFF_SIZE];
    int i_result = 0;

    while ((i_result = recv(*connect_sock, recv_buff, BUFF_SIZE, 0)) > 0)
    {
        recv_buff[i_result] = '\0';
        printf("Server>%s\n", recv_buff);
    }
    if (i_result < 0){
        printf("recv() error with error code %d\n", WSAGetLastError());
    }
    closesocket(*connect_sock);
};

int main()
{
    WSADATA wsadata;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (iResult != 0)
    {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }
    struct addrinfo *result = NULL, *ptr = NULL, hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_protocol = IPPROTO_TCP;
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    iResult = getaddrinfo(LOCAL_IP, MY_PORT, &hint, &result);
    if (iResult != 0)
    {
        printf("getaddrinfo failed %d\n", iResult);
        WSACleanup();
        return 1;
    }

    SOCKET connect_socket = INVALID_SOCKET;
    connect_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connect_socket == INVALID_SOCKET)
    {
        printf("failed on socket(), %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    iResult = connect(connect_socket, result->ai_addr, (int)result->ai_addrlen);

    if (iResult == SOCKET_ERROR)
    {
        closesocket(connect_socket);
        connect_socket = INVALID_SOCKET;
    }
    freeaddrinfo(result);

    if (connect_socket == INVALID_SOCKET)
    {
        printf("connect failed %d\n", WSAGetLastError());
        WSACleanup();
    }

    char rcv_buff[BUFF_SIZE];
    std::string send_buff;
    char *buff;
    std::thread rcv_thread = std::thread(handle_rcv, &connect_socket);
    rcv_thread.detach();
    while (1)
    {
        send_buff.erase();
        std::getline(std::cin, send_buff);
        if (send_buff.length() == 0)
        {
            break;
        }
        send_buff.push_back('\0');
        std::cout << send_buff << std::endl;
        iResult = send(connect_socket, send_buff.c_str(), (int)strlen(send_buff.c_str()), 0);
        if (iResult == SOCKET_ERROR)
        {
            printf("send failed error %d\n", WSAGetLastError());
            closesocket(connect_socket);
            WSACleanup();
            return 1;
        }
    }
    iResult = shutdown(connect_socket, SD_SEND); // No more sending
    if (iResult == SOCKET_ERROR)
    {
        printf("error shutdown socket %d\n", WSAGetLastError());
        closesocket(connect_socket);
        WSACleanup();
        return 1;
    }
    closesocket(connect_socket);
    WSACleanup();

    return 0;
}