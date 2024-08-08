#include "util/misc.hpp"

using namespace util;

void util::printVec(std::vector<int> &v)
{
    std::cout << "|";
    for (int i : v)
    {
        std::cout << i << "|";
    }
    std::cout << std::endl;
}
void util::printVec(std::vector<std::string> &v)
{
    std::cout << "|";
    for (std::string &s : v)
    {
        std::cout << s << "|";
    }
    std::cout << std::endl;
}

void util::printTable(std::vector<std::vector<std::string>> &t, bool truncated)
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
        std::vector<int> maxWidths;
        for (int i = 0; i < cols; i++)
        {
            int maxWidth = 0;
            for (int j = 0; j < rows; j++)
            {
                maxWidth = std::max((int)t[j][i].length(), maxWidth);
            }
            maxWidths.push_back(maxWidth);
        }

        for (int i = 0; i < rows; i++)
        {
            std::cout << "|";
            for (int j = 0; j < cols; j++)
            {
                std::cout << std::setw(std::max(maxWidths[j], 10)) << t[j][i] << "|";
            }
            std::cout << std::endl;
        }
    }
}
void util::printTable(std::vector<std::vector<std::string>> &t)
{
    util::printTable(t, true);
}

void util::printTableInt(std::vector<std::vector<int>> &t)
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
