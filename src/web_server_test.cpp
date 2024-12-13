#include "winserver/win_server.hpp"

int main(){
    winnetwork::WinTCPServer my_server = winnetwork::WinTCPServer(LOOPBACK_ADDRESS, DEFAULT_PORT);
    my_server.start_server();
}