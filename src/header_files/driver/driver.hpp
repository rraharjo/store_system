#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <exception>
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

namespace storedriver
{
    class Driver
    {
    private:
        store::StoreSystem *s_system = NULL;

    public:
        Driver();

        virtual void start() = 0;

        bool execute_command(std::string);
    };

    class StdIODriver : public Driver
    {
    public:
        StdIODriver();

        void start() override;
    };

    class PipeIODriver : public Driver
    {
    private:
        int input_pipe[2];
        int output_pipe[2];

        std::string read_input();

        void write_output(std::string);
    public:
        PipeIODriver();

        void start() override;

        void write_input(std::string);

        std::string read_output();
    };
}
#endif