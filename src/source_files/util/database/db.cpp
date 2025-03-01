#include "util/database/db.hpp"

using namespace util;

const std::string connection_info = "dbname=" + dotenv::local_postgres.db_name + 
" user=" + dotenv::local_postgres.username + 
" password=" + dotenv::local_postgres.password + 
" port=" + dotenv::local_postgres.port;


DB* DB::instance = NULL;

DB::DB(){
    this->connection = PQconnectdb(connection_info.c_str());
    if (PQstatus(this->connection) != CONNECTION_OK){
        std::cout << PQerrorMessage(this->connection) << std::endl;
    }
}

DB* DB::get_instance(){
    if (DB::instance == NULL){
        DB::instance = new DB();
    }
    return DB::instance;
}

std::vector<std::vector<std::string>> DB::execute_query(std::string query){
    PGresult* res = PQexec(this->connection, query.c_str());
    std::vector<std::vector<std::string>> to_ret;
    if (PQresultStatus(res) != PGRES_TUPLES_OK){
        throw std::runtime_error(PQerrorMessage(this->connection));
    }

    int rows = PQntuples(res);
    int field = PQnfields(res);
    for (int i = 0 ; i < rows ; i++){
        to_ret.push_back(std::vector<std::string>());
        for (int j = 0 ; j < field ; j++){
            to_ret[to_ret.size() - 1].push_back((PQgetvalue(res, i, j)));
        }
    }
    return to_ret;
}