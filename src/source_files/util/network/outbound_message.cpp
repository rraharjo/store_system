#include "util/network/outbound_message.hpp"
namespace util
{
    namespace network
    {
        OutboundMessage::OutboundMessage() : OutboundMessage::OutboundMessage(NULL, 0)
        {
        }
        
        OutboundMessage::OutboundMessage(char *payload_source, size_t payload_len)
            : Message::Message(payload_source, payload_len)
        {
            this->current_payload_len = this->total_payload_len;
            // this->current_payload = this->payload.get();
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
            if (this->total_payload_len == this->current_payload_len)
            {
                cur_flag = cur_flag | FLAG_MSG_START;
            }
            if (buff_size >= sizeof(MessageHeader) + this->current_payload_len)
            {
                cur_flag = cur_flag | FLAG_MSG_END;
            }
            size_t this_payload_len = std::min(this->current_payload_len, buff_size - sizeof(MessageHeader));
            MessageHeader this_header = {sizeof(MessageHeader), this_payload_len, cur_flag};
            memcpy(dest, &this_header, sizeof(MessageHeader));
            char *payload_start = dest + sizeof(MessageHeader);
            std::memcpy(payload_start, this->get_current_payload(), this_payload_len);
            this->current_payload += this_payload_len;
            this->current_payload_len -= this_payload_len;
            return this_payload_len + sizeof(MessageHeader);
        }

        void OutboundMessage::reset_message()
        {
            // this->current_payload = this->payload.get();
            this->current_payload = this->payload;
            this->current_payload_len = this->total_payload_len;
        }
    }
}