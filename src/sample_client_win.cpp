#ifdef _WS32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <thread>
#include "util/network/outbound_message.hpp"
#include "util/network/inbound_message.hpp"
#define MY_PORT "8000"
#define LOCAL_IP "127.0.0.1"
#define BUFF_SIZE 512

auto handle_rcv = [](SOCKET *connect_sock)
{
    char recv_buff[BUFF_SIZE];
    int i_result = 0;
    util::network::InboundMessage in_msg;
    while ((i_result = recv(*connect_sock, recv_buff, BUFF_SIZE, 0)) > 0)
    {
        #ifdef DEBUG
        printf("Raw> ");
        if (in_msg.get_total_payload_len() == 0)
        {
            util::network::Message::print_buffer(recv_buff, i_result, true);
        }
        else
        {
            std::cout.write(recv_buff, i_result);
        }
        std::cout << std::endl;
        #endif
        in_msg.add_msg(recv_buff, i_result);
        if (in_msg.ended())
        {
            std::cout << "Server> ";
            std::cout.write(in_msg.get_payload(), in_msg.get_total_payload_len());
            std::cout << std::endl;
            in_msg.clear_payload();
        }
    }
    if (i_result < 0)
    {
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

    char snd_buff[BUFF_SIZE];
    std::string input_buff;
    char *buff;
    std::thread rcv_thread = std::thread(handle_rcv, &connect_socket);
    rcv_thread.detach();
    while (1)
    {
        input_buff.erase();
        std::getline(std::cin, input_buff);

        if (input_buff.length() == 0)
        {
            break;
        }
        util::network::OutboundMessage to_send((char *)input_buff.c_str(), input_buff.length());
        while (to_send.get_current_payload_len() > 0)
        {
            size_t this_send_size = to_send.dump(snd_buff, BUFF_SIZE);
            // util::network::Message::print_buffer(snd_buff, this_send_size, true);
            iResult = send(connect_socket, snd_buff, this_send_size, 0);
            if (iResult == SOCKET_ERROR)
            {
                printf("send failed error %d\n", WSAGetLastError());
                closesocket(connect_socket);
                WSACleanup();
                return 1;
            }
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
#endif