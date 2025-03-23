#include "driver/driver.hpp"
#include "util/network/win_server.hpp"

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
            winnetwork::WinTCPServer my_server = winnetwork::WinTCPServer(LOOPBACK_ADDRESS, DEFAULT_PORT);
            my_server.start_server();
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