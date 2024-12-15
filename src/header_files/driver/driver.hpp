#include <iostream>
#include "store/store_system.hpp"
#include "util/misc.hpp"
#ifndef STORE_DRIVER
#define STORE_DRIVER
#define STREAM_SIZE 512
#define ADD_INV 1
#define PURC_INV 2
#define PURC_ASS 3
#define CAPT_ASS 4
#define SELL_INV 5
#define SELL_ASS 6
#define EO_YEAR 7
#define LIST_ITEM 8
namespace storedriver
{
    class Driver
    {
    private:
        store::StoreSystem *s_system = NULL;

    public:
        Driver();

        virtual void start();

        void execute_command(std::string);
    };

    class StdDriver : public Driver
    {
    public:
        void start() override;
    };

    class CustomDriver : public Driver
    {
    private:
        std::istringstream input_stream;
        std::ostringstream output_stream;
    public:
        void start() override;
    };
}
#endif