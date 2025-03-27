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
            size_t current_expected_payload_len;

            // add (len - sizeof(MessageHeader)) bytes of data from msg
            // msg must start with a proper MessageHeader
            void add_new_msg(char *msg, size_t msg_len);

            // add payload_len bytes of payload from payload, payload does not have a header
            // the state of this object must be expecting more or equal than payload_len bytes
            void add_existing_msg(char *payload, size_t payload_len);

        public:
            InboundMessage();

            ~InboundMessage();

            void add_msg(char *msg, size_t msg_len);

            bool ended();

            size_t get_current_expected_payload_size();
        };
    }
}

#endif