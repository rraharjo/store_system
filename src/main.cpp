#include "driver/driver.hpp"
#include "network/server.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "unknown command" << std::endl;
        return 0;
    }
    try
    {
        if (strcmp(argv[1], "local") == 0)
        {
            storedriver::StdIODriver my_driver = storedriver::StdIODriver(false);
            my_driver.start();
        }
        else if (strcmp(argv[1], "server") == 0)
        {
            util::network::TCPServer my_server = util::network::TCPServer(LOOPBACK_ADDRESS_STR, DEFAULT_PORT_STR);
            my_server.start_server_with_driver();
        }
        else if (strcmp(argv[1], "serverdebug") == 0){
            util::network::TCPServer my_server = util::network::TCPServer(LOOPBACK_ADDRESS_STR, DEFAULT_PORT_STR);
            my_server.start_server_debug();
        }
        else
        {
            std::cout << "unknown command" << std::endl;
            return 1;
        }
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
}