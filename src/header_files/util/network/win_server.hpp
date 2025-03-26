#include <winsock2.h>
#include <ws2tcpip.h>
#include <memory>
#include <queue>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <string>
#include <iostream>
#include <stdexcept>
#include "inbound_message.hpp"
#include "outbound_message.hpp"
#include "driver/driver.hpp"
#include "nlohmann/json.hpp"

#ifndef WIN_SERVER
#define WIN_SERVER

#define DEFAULT_PORT "8000"
#define LOOPBACK_ADDRESS "127.0.0.1"
#define MAX_CLIENT 3
#define RECV_BUFF 2048
#define SEND_BUFF 2048
namespace winnetwork
{

    class WinTCPServer
    {
    private:
        // Server component
        WSAData wsa_data;
        SOCKET listen_socket;
        std::string port_number;
        std::string ip_address;
        const int max_client = MAX_CLIENT;
        int network_fam;
        int socket_type;
        int protocol;

        // shared resources
        std::mutex client_mtx, driver_mtx;
        int num_of_client = 0;
        std::unique_ptr<storedriver::PipeIODriver> driver;
        std::queue<SOCKET *> client_commands;

    private:
        void init_socket();

    public:
        WinTCPServer(std::string ip_address, std::string port_number);

        ~WinTCPServer();

        void start_server_with_driver();

        void start_server_debug();
    };
}
#endif