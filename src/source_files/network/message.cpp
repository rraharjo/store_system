#include "network/message.hpp"

namespace util
{
    namespace network
    {
        void Message::print_buffer(char *source, size_t len, bool header)
        {
            std::cout << "Message summary: " << std::endl;
            if (header)
            {
                MessageHeader *head = (MessageHeader *)source;
                std::cout << "Expected header_header: " << (int)HHEADER << std::endl;
                std::cout << "This header_header: " << (int)head->header_header << std::endl;
                std::cout << "Header length: " << (int)head->header_len << std::endl;
                std::cout << "Payload length: " << (int)head->payload_len << std::endl;
                std::cout << "Flags (unsigned): " << (unsigned int)head->flags << std::endl;
                source += sizeof(MessageHeader);
                len -= sizeof(MessageHeader);
            }
            std::cout << "Payload: ";
            std::cout.write(source, len);
            std::cout << std::endl;
        }

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

        Message::Message(const Message &other)
            : allocated_memory(other.allocated_memory), total_payload_len(other.total_payload_len)
        {
            this->payload = new char[this->allocated_memory];
            std::memcpy(this->payload, other.payload, this->total_payload_len);
        }

        Message::Message(char *payload_source, size_t payload_len)
        {
            size_t to_allocate = 1;
            while (to_allocate < payload_len)
            {
                to_allocate = to_allocate * 2;
            }
            this->payload = new char[to_allocate];
            this->total_payload_len = payload_len;
            this->allocated_memory = to_allocate;
            std::memcpy(this->payload, payload_source, payload_len);
        }

        Message::~Message()
        {
            delete[] this->payload;
        }

        size_t Message::get_total_payload_len()
        {
            return this->total_payload_len;
        }

        char *Message::get_payload()
        {
            return this->payload;
        }

        void Message::clear_payload()
        {
            this->total_payload_len = 0;
        }

    }
}