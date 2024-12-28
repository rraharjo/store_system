#include <string>

#ifndef ENVIRONMENT_INFORMATION_HPP
#define ENVIRONMENT_INFORMATION_HPP
namespace dotenv{
    typedef struct postgresStruct{
        std::string db_name;
        std::string username;
        std::string password;
        std::string port;
    } PostgresStruct;
    extern const PostgresStruct local_postgres;
}
#endif