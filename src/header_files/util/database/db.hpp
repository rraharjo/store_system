#include <string>
#include <vector>
#include <iostream>
#include "libpq-fe.h"
#include "private.hpp"

#ifndef DBCONNECTION_HPP
#define DBCONNECTION_HPP
namespace util
{
    //namespace database
    class DB
    {
    private:
        static DB *instance;
        PGconn *connection;
        DB();

    public:
        static DB *getInstance();
        std::vector<std::vector<std::string>> executeQuery(std::string query);
    };
};
#endif