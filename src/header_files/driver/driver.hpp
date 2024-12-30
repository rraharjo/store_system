#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <exception>
#include "store/store_system.hpp"
#include "util/misc.hpp"
#include "driver/executor.hpp"
#ifndef STORE_DRIVER
#define STORE_DRIVER
#define STREAM_SIZE 512


namespace storedriver
{
    class Driver
    {
    private:
        store::StoreSystem *s_system = NULL;

    protected:
        bool json_input;

    public:
        Driver(bool);

        virtual void start() = 0;

        int execute_command(std::string);

        virtual int execute_commands(std::string);
    };

    class StdIODriver : public Driver
    {
    public:
        StdIODriver(bool);

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
        PipeIODriver(bool);

        void start() override;

        int execute_commands(std::string) override;

        void write_input(std::string);

        std::string read_output();
    };
}
#endif