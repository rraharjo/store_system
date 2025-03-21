#include "driver/driver.hpp"
#include "winserver/win_server.hpp"
// Debug heap
// #define _CRTDBG_MAP_ALLOC
// #include <cstdlib>
// #include <crtdbg.h>

int main(int argc, char **argv)
{
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    if (argc < 2)
    {
        std::cout << "unknown command" << std::endl;
        return 1;
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