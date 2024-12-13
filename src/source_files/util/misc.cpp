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
