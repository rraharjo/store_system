#ifndef UTIL_MESSAGE_HPP
#define UTIL_MESSAGE_HPP
#include <cstdint>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>
#define HHEADER 0b0101010101010101
#define MAX_PAYLOAD_LEN 1024
#define FLAG_MSG_START 1
#define FLAG_MSG_END 2
namespace util
{
    namespace network
    {
#ifdef _WIN32
#pragma pack(push, 1)
        struct MessageHeader
        {
            uint16_t header_header;
            uint64_t header_len;
            uint64_t payload_len;
            uint8_t flags; // Not sure if this field is needed

            MessageHeader(uint64_t header_len,
                          uint64_t payload_len,
                          uint8_t flags);

            MessageHeader();
        };
#pragma pop()
#endif

#ifdef __linux__
        struct __attribute__((packed)) MessageHeader
        {
            uint16_t header_header;
            uint64_t header_len;
            uint64_t payload_len;
            uint8_t flags; // Not sure if this field is needed

            MessageHeader(uint64_t header_len,
                          uint64_t payload_len,
                          uint8_t flags);

            MessageHeader();
        };
#endif
    }
}
namespace util
{
    namespace network
    {

        class Message
        {
        protected:
            char *payload = NULL;
            size_t total_payload_len;
            size_t allocated_memory = 0;

        public:
            static void print_buffer(char *source, size_t source_len, bool header);

            Message(const Message &other);

            // takes a payload without MessageHeader
            Message(char *payload_source, size_t payload_len);

            virtual ~Message();

            size_t get_total_payload_len();

            char *get_payload();

            virtual void clear_payload();
        };
    }
}
#endif