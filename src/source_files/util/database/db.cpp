#include "util/database/db.hpp"

using namespace util;

const std::string connectionInfo = "dbname=store_system user=postgres password=Sek@rsuli32! port=5432";

DB* DB::instance = NULL;

DB::DB(){
    this->connection = PQconnectdb(connectionInfo.c_str());
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
    std::vector<std::vector<std::string>> toRet;
    if (PQresultStatus(res) != PGRES_TUPLES_OK){
        std::cout << PQerrorMessage(this->connection) << std::endl;
        PQclear(res);
        return toRet;
    }

    int rows = PQntuples(res);
    int field = PQnfields(res);
    for (int i = 0 ; i < rows ; i++){
        toRet.push_back(std::vector<std::string>());
        for (int j = 0 ; j < field ; j++){
            toRet[toRet.size() - 1].push_back((PQgetvalue(res, i, j)));
        }
    }
    return toRet;
}