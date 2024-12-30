#include "driver/driver.hpp"
#include <cstring>

/************************************************************ */

storedriver::Driver::Driver(bool json_input) : json_input(json_input)
{
    this->s_system = store::StoreSystem::get_instance();
};

int storedriver::Driver::execute_command(std::string command)
{
    try
    {
        storedriver::Executor::execute(this->s_system, command, this->json_input);
        return 1;
    }
    catch (nlohmann::json_abi_v3_11_3::detail::out_of_range& e){
        std::cerr << e.what() << std::endl;
        return 0;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 0;
    }
}


//Don't use this
int storedriver::Driver::execute_commands(std::string commands)
{
    std::vector<std::string> multiple_commands;
    std::string delimiter = ENDCMD;
    std::string temp;
    int commands_length = commands.length(), delimiter_length = delimiter.length();
    int j = 0;
    for (int i = 0; i < commands_length; i++)
    {
        if (commands[i] == delimiter[j])
        {
            j++;
        }
        else
        {
            j = 0;
        }
        temp.push_back(commands[i]);
        if (j == delimiter_length)
        {
            multiple_commands.push_back(temp);
            temp.clear();
        }
    }
    int to_ret = 0;
    for (std::string command : multiple_commands)
    {
        to_ret += this->execute_command(command);
    }
    return to_ret;
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
        this->execute_command(buff);
    }
}


//Don't use this
int storedriver::PipeIODriver::execute_commands(std::string commands)
{
    std::vector<std::string> multiple_commands;
    std::string delimiter = ENDCMD;
    std::string temp;
    int commands_length = commands.length(), delimiter_length = delimiter.length();
    int j = 0;
    for (int i = 0; i < commands_length; i++)
    {
        if (commands[i] == delimiter[j])
        {
            j++;
        }
        else
        {
            j = 0;
        }
        temp.push_back(commands[i]);
        if (j == delimiter_length)
        {
            multiple_commands.push_back(temp);
            temp.clear();
        }
    }
    int to_ret = 0;
    for (std::string command : multiple_commands)
    {
        if (this->execute_command(command))
        {
            to_ret++;
            this->write_output("success");
        }
        else
        {
            this->write_output("failed");
        }
    }
    return to_ret;
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
        this->execute_command(input);
    }
}
