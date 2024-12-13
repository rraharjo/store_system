#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>

#ifndef MISCUTIL
#define MISCUTIL

namespace util{
    void print_vec(std::vector<int>& v);
    void print_vec(std::vector<std::string>& v);
    void print_table(std::vector<std::vector<std::string>>& t, bool truncated);
    void print_table(std::vector<std::vector<std::string>>& t);
    void print_table_int(std::vector<std::vector<int>>& t);
};

#endif