#include "util/database/db.hpp"
#include "util/misc.hpp"

int main(int argc, char** argv){
    util::DB *instance = util::DB::get_instance();
    std::vector<std::vector<std::string>> result = instance->execute_query("insert into sellable (itemcode, itemname, sellingprice) values('10', 'testing', 100.00) returning *");
    util::printTable(result);
    return 0;
}