#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <memory>
#include "postgresql/libpq-fe.h"
#include ".env/private.hpp"

#ifndef DBCONNECTION_HPP
#define DBCONNECTION_HPP
namespace util
{
    class DB
    {
    private:
        static std::unique_ptr<DB> instance;
        PGconn *connection;
        DB();

    public:
        static DB *get_instance();
        std::vector<std::vector<std::string>> execute_query(std::string query);

        std::vector<std::vector<std::string>> execute_query_parameterized(std::string query, std::vector<std::string> &values);
    };
};
#endif