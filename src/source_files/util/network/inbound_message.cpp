#include "util/network/inbound_message.hpp"
namespace util
{
    namespace network
    {
        InboundMessage::InboundMessage(char *msg, size_t size)
            : Message(nullptr, 0)
        {
            MessageHeader *head = (MessageHeader *)msg;
            if (!msg || size < sizeof(MessageHeader) || head->header_header != HHEADER)
            {
                throw std::invalid_argument("Inbound message does not have a proper header");
            }
            if (head->flags & FLAG_MSG_START)
            {
                this->has_start = true;
            }
            else
            {
                this->has_start = false;
            }
            if (has_end & FLAG_MSG_END)
            {
                this->has_end = true;
            }
            else
            {
                this->has_end = false;
            }
            size_t to_allocate = 1;
            char *msg_start = msg + sizeof(MessageHeader);
            size_t payload_size = size - sizeof(MessageHeader);
            while (to_allocate < payload_size)
            {
                to_allocate *= 2;
            }
            this->payload = std::make_unique<char>(to_allocate);
            this->total_payload_len = payload_size;
            this->allocated_memory = to_allocate;
            std::memcpy(this->payload.get(), msg_start, payload_size);
        }

        InboundMessage::InboundMessage() : Message(nullptr, 0)
        {
            this->has_end = false;
            this->has_start = false;
        }

        InboundMessage::~InboundMessage()
        {
        }

        void InboundMessage::add_payload(char *msg, size_t msg_len)
        {
            MessageHeader *this_head = (MessageHeader *)msg;
            if (!msg || msg_len < sizeof(MessageHeader) || this_head->header_header != HHEADER)
            {
                throw std::invalid_argument("Inbound message does not have a proper header");
            }
            if (this->has_end)
            {
                throw std::invalid_argument("Adding a payload to a complete message");
            }
            if (this->has_start && (this_head->flags & FLAG_MSG_START))
            {
                throw std::invalid_argument("Adding a start payload to an existing payload");
            }
            size_t to_copy = msg_len - sizeof(MessageHeader);

            if (this->allocated_memory < this->total_payload_len + to_copy)
            {
                size_t new_allocate_size = this->allocated_memory;
                while (new_allocate_size < this->total_payload_len + to_copy)
                {
                    new_allocate_size *= 2;
                }
                std::unique_ptr<char> new_allocated_memory = std::make_unique<char>(new_allocate_size);
                std::memcpy(new_allocated_memory.get(), this->payload.get(), this->total_payload_len);
                this->payload = std::move(new_allocated_memory);
                this->allocated_memory = new_allocate_size;
            }

            char *to_add = this->payload.get() + this->total_payload_len;
            char *msg_start = msg + sizeof(MessageHeader);
            std::memcpy(to_add, msg_start, to_copy);
        }
    }
}