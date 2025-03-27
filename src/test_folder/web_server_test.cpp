#include "util/network/server.hpp"

int main(int argc, char *argv[])
{
    if (argc >= 2)
    {
        std::string addr = argv[0], port = argv[1];
        util::network::TCPServer my_server = util::network::TCPServer(addr, port);
        my_server.start_server_with_driver();
        return 0;
    }
    util::network::TCPServer my_server = util::network::TCPServer(LOOPBACK_ADDRESS_STR, DEFAULT_PORT_STR);
    my_server.start_server_with_driver();
    return 0;
}