#include "util/database/db.hpp"

using namespace util;

const std::string connection_info = "dbname=" + dotenv::local_postgres.db_name +
                                    " user=" + dotenv::local_postgres.username +
                                    " password=" + dotenv::local_postgres.password +
                                    " port=" + dotenv::local_postgres.port;

std::unique_ptr<DB> DB::instance = NULL;

DB::DB()
{
    this->connection = PQconnectdb(connection_info.c_str());
    if (PQstatus(this->connection) != CONNECTION_OK)
    {
        std::cout << PQerrorMessage(this->connection) << std::endl;
    }
}

DB *DB::get_instance()
{
    if (DB::instance == NULL)
    {
        DB::instance.reset(new DB());
    }
    return DB::instance.get();
}

std::vector<std::vector<std::string>> DB::execute_query(std::string query)
{
    PGresult *res = PQexec(this->connection, query.c_str());
    std::vector<std::vector<std::string>> to_ret;
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        throw std::runtime_error(PQerrorMessage(this->connection));
    }

    int rows = PQntuples(res);
    int field = PQnfields(res);
    for (int i = 0; i < rows; i++)
    {
        to_ret.push_back(std::vector<std::string>());
        for (int j = 0; j < field; j++)
        {
            to_ret[to_ret.size() - 1].push_back((PQgetvalue(res, i, j)));
        }
    }
    return to_ret;
}

std::vector<std::vector<std::string>> DB::execute_query_parameterized(std::string query, std::vector<std::string> &values)
{
    const int num_of_params = values.size();
    char **param_values;
    param_values = new char *[num_of_params];
    for (int i = 0; i < num_of_params; i++)
    {
        param_values[i] = new char[values[i].length() + 1];
        memcpy(param_values[i], values[i].c_str(), values[i].length() + 1);
        param_values[i][values[i].length()] = '\0';
    }
    PGresult *res = PQexecParams(this->connection,
                                 query.c_str(),
                                 num_of_params,
                                 NULL,
                                 param_values,
                                 NULL,
                                 NULL,
                                 0);
    for (int i = 0; i < num_of_params; i++)
    {
        delete[] param_values[i];
    }
    delete[] param_values;
    std::vector<std::vector<std::string>> to_ret; // error
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        throw std::runtime_error(PQerrorMessage(this->connection));
    }

    int rows = PQntuples(res);
    int field = PQnfields(res);
    for (int i = 0; i < rows; i++)
    {
        to_ret.push_back(std::vector<std::string>());
        for (int j = 0; j < field; j++)
        {
            to_ret[to_ret.size() - 1].push_back((PQgetvalue(res, i, j)));
        }
    }
    return to_ret;
}