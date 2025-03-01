#include "driver/driver.hpp"
#include <cstring>

/************************************************************ */

storedriver::Driver::Driver(bool json_input) : json_input(json_input)
{
    this->s_system = store::StoreSystem::get_instance();
};

nlohmann::json storedriver::Driver::execute_command(std::string command)
{
    nlohmann::json response;
    try
    {
        nlohmann::json body = storedriver::Executor::execute(this->s_system, command, this->json_input);
        response["status"] = true;
        response["body"] = body;
    }
    catch (std::exception& e)
    {
        nlohmann::json body;
        body["message"] = e.what();
        response["status"] = false;
        response["body"] = body;
    }
    return response;
}

storedriver::StdIODriver::StdIODriver(bool json_input) : storedriver::Driver(json_input)
{
}

void storedriver::StdIODriver::start()
{
    char buff[BUFSIZ];
    int buff_size;
    std::string buff_str;
    bool ongoing = true;
    while (ongoing)
    {
        std::cout << '>';
        std::cin.getline(buff, BUFSIZ);
        buff_size = strlen(buff);
        buff_str = std::string(buff);
        if (!buff_size)
        {
            break;
        }
        std::cout << this->execute_command(buff).dump() << std::endl;;
    }
}

storedriver::PipeIODriver::PipeIODriver(bool json_input) : storedriver::Driver(json_input)
{
    if (_pipe(this->input_pipe, 2 * STREAM_SIZE, O_TEXT) == -1)
    {
        throw std::runtime_error("error on input _pipe()");
    }
    if (_pipe(this->output_pipe, 2 * STREAM_SIZE, O_TEXT) == -1)
    {
        throw std::runtime_error("error on output _pipe()");
    }
}

std::string storedriver::PipeIODriver::read_input()
{
    char read_buff[STREAM_SIZE];
    int bytes_read = 0;
    if (bytes_read = _read(this->input_pipe[0], read_buff, STREAM_SIZE) == -1)
    {
        throw std::runtime_error("error on read_input _read()");
    }
    std::string to_ret(read_buff);
    return to_ret;
}

void storedriver::PipeIODriver::write_input(std::string input)
{
    int input_length = input.length();
    if (input_length > STREAM_SIZE - 1)
    {
        throw std::invalid_argument("error on write_input - input is too long");
    }
    char write_buff[STREAM_SIZE];
    strcpy(write_buff, input.c_str());
    if (_write(this->input_pipe[1], write_buff, input_length + 1) == -1)
    {
        throw std::runtime_error("error on write_input _write()");
    }
}

std::string storedriver::PipeIODriver::read_output()
{
    char read_buff[STREAM_SIZE];
    int bytes_read = 0;
    std::string to_ret;
    if (bytes_read = _read(this->output_pipe[0], read_buff, STREAM_SIZE) == -1)
    {
        throw std::runtime_error("error on read_output _read()");
    }
    to_ret = std::string(read_buff);
    return to_ret;
}

void storedriver::PipeIODriver::write_output(std::string input)
{
    int input_length = input.length();
    if (input_length > STREAM_SIZE - 1)
    {
        throw std::invalid_argument("error on write_output - input is too long");
    }
    char write_buff[STREAM_SIZE];
    strcpy(write_buff, input.c_str());
    if (_write(this->output_pipe[1], write_buff, input_length + 1) == -1)
    {
        throw std::runtime_error("error on write_output _write()");
    }
}

void storedriver::PipeIODriver::start()
{
    int read_bytes = 0;
    std::string input = "";
    while (true)
    {
        input.clear();
        input = this->read_input();
        nlohmann::json output = this->execute_command(input);
        this->write_output(output.dump());
    }
}
