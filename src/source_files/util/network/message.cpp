#include "util/network/message.hpp"

namespace util
{
    namespace network
    {
        MessageHeader::MessageHeader(size_t header_len,
                                     size_t payload_len,
                                     uint8_t flags)
        {
            this->header_header = HHEADER;
            this->header_len = header_len;
            this->payload_len = payload_len;
            this->flags = flags;
        }

        MessageHeader::MessageHeader()
        {
        }

        Message::Message(char *payload_source, size_t payload_len)
        {
            size_t to_allocate = 1;
            while (to_allocate < payload_len){
                to_allocate = to_allocate * 2;
            }
            this->payload = std::make_unique<char>(to_allocate);
            this->total_payload_len = payload_len;
            this->allocated_memory = to_allocate;
            std::memcpy(this->payload.get(), payload_source, payload_len);
        }

        Message::~Message()
        {
        }

        size_t Message::get_total_payload_len()
        {
            return this->total_payload_len;
        }

        char *Message::get_payload()
        {
            return this->payload.get();
        }
    }
}