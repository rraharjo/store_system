#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <string>
#include <iostream>
#include <stdexcept>

#ifndef WIN_SERVER
#define WIN_SERVER

#define DEFAULT_PORT "20000"
#define LOOPBACK_ADDRESS "127.0.0.1"
#define MAX_CLIENT 5
#define BUFF_SIZE 512
namespace winnetwork
{

    class WinTCPServer
    {
    private:
        const int max_client = MAX_CLIENT;

        std::string port_number;
        std::string ip_address;
        int network_fam;
        int socket_type;
        int protocol;

        WSAData wsa_data;
        SOCKET listen_socket;

        std::mutex my_mtx;

        // shared resources
        int num_of_client = 0;

    private:
        void init_socket();

    public:
        WinTCPServer(std::string ip_address, std::string port_number);

        void start_server();
    };
}
#endif