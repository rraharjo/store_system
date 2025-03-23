#ifndef INBOUND_MSG_HPP
#define INBOUND_MSG_HPP
#include "message.hpp"
namespace util
{
    namespace network
    {
        class InboundMessage : public Message
        {
        private:
            bool has_start;
            bool has_end;
        public:
            // Takes a message with formatted MessageHeader
            InboundMessage(char *msg, size_t size);

            InboundMessage();

            ~InboundMessage();

            // add (len - sizeof(MessageHeader)) bytes of data from src
            // src must start with a proper MessageHeader
            void add_payload(char *msg, size_t msg_len);
        };
    }
}

#endif