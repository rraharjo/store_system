#include <string>
#include <vector>
#include <iostream>
#include "libpq-fe.h"
#include ".env/private.hpp"

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
        static DB *get_instance();
        std::vector<std::vector<std::string>> execute_query(std::string query);
    };
};
#endif