#include "util/network/inbound_message.hpp"

namespace util
{
    namespace network
    {
        InboundMessage::InboundMessage() : Message(nullptr, 0)
        {
            this->current_expected_payload_len = 0;
        }

        InboundMessage::InboundMessage(const InboundMessage &other) : Message(other)
        {
            this->current_expected_payload_len = 0;
        }

        InboundMessage::~InboundMessage()
        {
        }

        void InboundMessage::add_new_msg(char *msg, size_t msg_len)
        {
            if (this->current_expected_payload_len > 0)
            {
                throw std::invalid_argument("Previous message was not finished, but received a new header");
            }
            MessageHeader *this_head = (MessageHeader *)msg;
            if (!msg || msg_len < sizeof(MessageHeader) || this_head->header_header != HHEADER)
            {
                throw std::invalid_argument("Inbound message does not have a proper header");
            }
            this->current_expected_payload_len = this_head->payload_len;
            size_t to_copy = msg_len - sizeof(MessageHeader);

            if (this->allocated_memory < this->total_payload_len + to_copy)
            {
                size_t new_allocate_size = this->allocated_memory;
                while (new_allocate_size < this->total_payload_len + to_copy)
                {
                    new_allocate_size *= 2;
                }
                char *new_allocated_memory = new char[new_allocate_size];
                std::memcpy(new_allocated_memory, this->payload, this->total_payload_len);
                delete[] this->payload;
                this->payload = new_allocated_memory;
                this->allocated_memory = new_allocate_size;
            }
            char *to_add = this->payload + this->total_payload_len;
            char *msg_start = msg + sizeof(MessageHeader);
            std::memcpy(to_add, msg_start, to_copy);
            this->total_payload_len += to_copy;
            this->current_expected_payload_len -= to_copy;
        }

        void InboundMessage::add_existing_msg(char *payload, size_t payload_len)
        {
            if (this->current_expected_payload_len < payload_len)
            {
                std::string err_msg = "This message is expecting " + std::to_string(this->current_expected_payload_len) +
                                      " but the payload length is " + std::to_string(payload_len);
                throw std::invalid_argument(err_msg);
            }
            if (this->allocated_memory < this->total_payload_len + payload_len)
            {
                size_t new_allocate_size = this->allocated_memory;
                while (new_allocate_size < this->total_payload_len + payload_len)
                {
                    new_allocate_size *= 2;
                }
                char *new_allocated_memory = new char[new_allocate_size];
                std::memcpy(new_allocated_memory, this->payload, this->total_payload_len);
                delete[] this->payload;
                this->payload = new_allocated_memory;
                this->allocated_memory = new_allocate_size;
            }
            char *to_add = this->payload + this->total_payload_len;
            std::memcpy(to_add, payload, payload_len);
            this->total_payload_len += payload_len;
            this->current_expected_payload_len -= payload_len;
        }

        void InboundMessage::add_msg(char *msg, size_t msg_len)
        {
            if (this->current_expected_payload_len == 0)
            {
                this->add_new_msg(msg, msg_len);
            }
            else
            {
                this->add_existing_msg(msg, msg_len);
            }
        }

        bool InboundMessage::ended()
        {
            return this->current_expected_payload_len == 0;
        }

        size_t InboundMessage::get_current_expected_payload_size()
        {
            return this->current_expected_payload_len;
        }
    }
}