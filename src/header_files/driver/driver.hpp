#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <exception>
#include "store/store_system.hpp"
#include "util/misc.hpp"
#include "driver/executor.hpp"
#ifndef STORE_DRIVER
#define STORE_DRIVER
#define STREAM_SIZE 1024 * 4


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

        virtual ~Driver();

        virtual void start() = 0;

        nlohmann::json execute_command(std::string);
    };

    class StdIODriver : public Driver
    {
    public:
        StdIODriver(bool);

        ~StdIODriver();

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

        ~PipeIODriver();

        void start() override;

        void write_input(std::string);

        std::string read_output();
    };
}
#endif