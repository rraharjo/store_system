#include "network/outbound_message.hpp"
namespace util
{
    namespace network
    {
        OutboundMessage::OutboundMessage() : OutboundMessage::OutboundMessage(NULL, 0)
        {
        }

        OutboundMessage::OutboundMessage(const OutboundMessage &other) : Message(other)
        {
            this->current_payload_len = this->total_payload_len;
            this->current_payload = this->payload;
        }

        OutboundMessage::OutboundMessage(char *payload_source, size_t payload_len)
            : Message::Message(payload_source, payload_len)
        {
            this->current_payload_len = this->total_payload_len;
            this->current_payload = this->payload;
        }

        OutboundMessage::~OutboundMessage()
        {
        }

        char *OutboundMessage::get_current_payload()
        {
            return this->current_payload;
        }

        size_t OutboundMessage::get_current_payload_len()
        {
            return this->current_payload_len;
        }

        size_t OutboundMessage::dump(char *dest, size_t buff_size)
        {
            uint8_t cur_flag = 0;
            if (this->current_payload_len == 0)
            {
                return 0;
            }
            size_t this_payload_len = 0, to_ret = 0;
            char *payload_start = NULL;
            if (this->get_current_payload_len() == this->get_total_payload_len())
            {
                this_payload_len = std::min(this->current_payload_len, buff_size - sizeof(MessageHeader));
                MessageHeader this_header = {sizeof(MessageHeader), this->total_payload_len, cur_flag};
                memcpy(dest, &this_header, sizeof(MessageHeader));
                payload_start = dest + sizeof(MessageHeader);
                to_ret += sizeof(MessageHeader);
            }
            else
            {
                this_payload_len = std::min(this->current_payload_len, buff_size);
                payload_start = dest;
            }
            std::memcpy(payload_start, this->get_current_payload(), this_payload_len);
            this->current_payload += this_payload_len;
            this->current_payload_len -= this_payload_len;
            to_ret += this_payload_len;
            return to_ret;
        }

        void OutboundMessage::reset_message()
        {
            this->current_payload = this->payload;
            this->current_payload_len = this->total_payload_len;
        }
    }
}