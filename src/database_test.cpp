#include "util/database/db.hpp"
#include "util/database/tables.hpp"
#include "util/misc.hpp"

int main(int argc, char** argv){
    //util::DB *instance = util::DB::getInstance();
    //std::vector<std::vector<std::string>> result = instance->executeQuery("insert into sellable (itemcode, itemname, sellingprice) values('10', 'testing', 100.00) returning *");
    //util::printTable(result);
    util::AccountingEntryTable* myTable = util::AccountingEntryTable::getInstance();
    std::cout << myTable << std::endl;
    //std::cout << util::AccountingEntryTable::instance << std::endl;
    delete myTable;
    std::cout << myTable << std::endl;
    //std::cout << util::AccountingEntryTable::instance << std::endl;
    int *myInt = new int(5);
    myTable = util::AccountingEntryTable::getInstance();
    std::cout << myTable << std::endl;
    //std::cout << util::AccountingEntryTable::instance << std::endl;
    //std::vector<std::string> myStr;
    //myTable->insertRow(myStr);

    return 0;
}