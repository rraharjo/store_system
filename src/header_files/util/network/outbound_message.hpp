#ifndef OUTBOUND_MSG_HPP
#define OUTBOUND_MSG_HPP

#include "message.hpp"
namespace util
{
    namespace network
    {
        class OutboundMessage : public Message
        {
        private:
            char *current_payload;
            size_t current_payload_len;

        public:
            OutboundMessage(char *payload_source, size_t payload_len);

            ~OutboundMessage();

            char *get_current_payload();

            size_t get_current_payload_len();

            /* copy at most (buff_size - sizeof(MessageHeader)) bytes pointed by current_payload to dest
            returns the number of bytes copied */
            size_t dump(char *dest, size_t buff_size);
        };
    }
}

#endif