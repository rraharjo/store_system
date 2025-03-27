#ifdef __linux__
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <thread>
#include "util/network/outbound_message.hpp"
#include "util/network/inbound_message.hpp"
#define SOCKET int
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
        printf("recv() error with error code %d\n", errno);
    }
    close(*connect_sock);
};

int main()
{
    int iResult;

    SOCKET connect_socket = -1;
    connect_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connect_socket == -1)
    {
        printf("failed on socket(), %d\n", errno);
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);
    addr.sin_addr.s_addr = inet_addr(LOCAL_IP);

    iResult = connect(connect_socket, (sockaddr *)&addr, sizeof(addr));

    if (iResult == -1)
    {
        close(connect_socket);
        printf("connect() failed with error code %d\n", errno);
        return 1;
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
            if (iResult == -1)
            {
                printf("send failed error %d\n", errno);
                close(connect_socket);
                return 1;
            }
        }
    }
    iResult = shutdown(connect_socket, SHUT_RDWR); // No more sending
    if (iResult == -1)
    {
        printf("error shutdown socket %d\n", errno);
        close(connect_socket);
        return 1;
    }
    close(connect_socket);
    return 0;
}
#endif