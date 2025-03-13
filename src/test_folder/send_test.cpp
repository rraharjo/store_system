#include <stdio.h>
#include <cstring>
#include <string>

#define PAYLOADLIMITER "PAYLOADSIZE"
int main(){
    std::string message = "aaaaaaaaaaaa";//12
    const char *msg_c = message.c_str();
    char header[256];
    int header_len = sprintf(header, "%s%lu", PAYLOADLIMITER, strlen(msg_c));
    printf("%s\n", header);
    printf("written: %d\n", header_len);
    return 0;
}