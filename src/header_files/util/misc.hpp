#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>

#ifndef MISCUTIL
#define MISCUTIL

namespace util{
    void printVec(std::vector<int>& v);
    void printVec(std::vector<std::string>& v);
    void printTable(std::vector<std::vector<std::string>>& t, bool truncated);
    void printTable(std::vector<std::vector<std::string>>& t);
    void printTableInt(std::vector<std::vector<int>>& t);
};

#endif