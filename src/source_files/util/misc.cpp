#include "util/misc.hpp"

using namespace util;

void util::print_vec(std::vector<int> &v)
{
    std::cout << "|";
    for (int i : v)
    {
        std::cout << i << "|";
    }
    std::cout << std::endl;
}
void util::print_vec(std::vector<std::string> &v)
{
    std::cout << "|";
    for (std::string &s : v)
    {
        std::cout << s << "|";
    }
    std::cout << std::endl;
}

void util::print_table(std::vector<std::vector<std::string>> &t, bool truncated)
{
    int rows = t.size();
    int cols = t[0].size();
    if (truncated)
    {
        for (std::vector<std::string> &row : t)
        {
            std::cout << "|";
            for (std::string &col : row)
            {
                std::cout << std::setw(10) << col << "|";
            }
            std::cout << std::endl;
        }
    }
    else
    {
        std::vector<int> max_widths;
        for (int i = 0; i < cols; i++)
        {
            int max_width = 0;
            for (int j = 0; j < rows; j++)
            {
                max_width = std::max((int)t[j][i].length(), max_width);
            }
            max_widths.push_back(max_width);
        }

        for (int i = 0; i < rows; i++)
        {
            std::cout << "|";
            for (int j = 0; j < cols; j++)
            {
                std::cout << std::setw(std::max(max_widths[j], 10)) << t[j][i] << "|";
            }
            std::cout << std::endl;
        }
    }
}
void util::print_table(std::vector<std::vector<std::string>> &t)
{
    util::print_table(t, true);
}

void util::print_table_int(std::vector<std::vector<int>> &t)
{
    for (std::vector<int> &row : t)
    {
        std::cout << "|";
        for (int col : row)
        {
            std::cout << std::setw(10) << col << "|";
        }
        std::cout << std::endl;
    }
}

std::vector<std::string> util::tokenize(const std::string &command)
{
    std::vector<std::string> to_ret;
    std::string current_string;
    bool inside_quote = false;
    int size = command.size();
    int cur_idx = 0;
    while (cur_idx < size)
    {
        if (inside_quote)
        {
            if (command[cur_idx] == '\"')
            {
                inside_quote = false;
                to_ret.push_back(current_string);
                current_string.clear();
            }
            else
            {
                current_string.push_back(command[cur_idx]);
            }
        }
        else
        {
            if (command[cur_idx] == ' ')
            {
                if (current_string.length() > 0)
                {
                    to_ret.push_back(current_string);
                    current_string.clear();
                }
            }
            else if (command[cur_idx] == '\"')
            {
                if (current_string.length() > 0)
                {
                    throw std::invalid_argument(command + " has a misplaced \"\n");
                }
                inside_quote = true;
            }
            else
            {
                current_string.push_back(command[cur_idx]);
            }
        }
        cur_idx++;
    }
    if (inside_quote)
    {
        throw std::invalid_argument(command + " has an unclosed \"\n");
    }
    if (current_string.length() > 0)
    {
        to_ret.push_back(current_string);
    }
    return to_ret;
}
