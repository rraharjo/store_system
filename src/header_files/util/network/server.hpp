#ifndef WIN_IX_SERVER
#define WIN_IX_SERVER

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#ifdef __linux__
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#define SOCKET int
#endif

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

#define DEFAULT_PORT_STR "8000"
#define LOOPBACK_ADDRESS_STR "127.0.0.1"
#define MAX_CLIENT 3
#define RECV_BUFF 2048
#define SEND_BUFF 2048
namespace util
{
    namespace network
    {
        class TCPServer
        {
        private:
            // Server component
            #ifdef _WIN32
                        WSAData wsa_data;
            #endif

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
            TCPServer(std::string ip_address, std::string port_number);

            ~TCPServer();

            void start_server_with_driver();

            void start_server_debug();
        };
    }
}
#endif