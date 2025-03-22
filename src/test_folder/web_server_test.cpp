#include "winserver/win_server.hpp"

int main(int argc, char *argv[])
{
    if (argc >= 2)
    {
        std::string addr = argv[0], port = argv[1];
        winnetwork::WinTCPServer my_server = winnetwork::WinTCPServer(addr, port);
        my_server.start_server();
        return 0;
    }
    winnetwork::WinTCPServer my_server = winnetwork::WinTCPServer(LOOPBACK_ADDRESS, DEFAULT_PORT);
    my_server.start_server();
    return 0;
}