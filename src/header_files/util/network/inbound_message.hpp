#ifndef INBOUND_MSG_HPP
#define INBOUND_MSG_HPP
#include <thread>
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

            void finish_timer();
        public:
            // Takes a message with formatted MessageHeader
            InboundMessage(char *msg, size_t size);

            InboundMessage();

            ~InboundMessage();

            void clear_payload() override;

            // add (len - sizeof(MessageHeader)) bytes of data from msg
            // msg must start with a proper MessageHeader
            void add_msg(char *msg, size_t msg_len);

            bool started();

            bool ended();
        };
    }
}

#endif