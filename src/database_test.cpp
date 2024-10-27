#include "util/database/db.hpp"
#include "util/database/tables.hpp"
#include "util/misc.hpp"

void printVec(std::vector<std::string>&);
void printTable(std::vector<std::vector<std::string>>&);

int main(int argc, char** argv){
    //util::DB *instance = util::DB::getInstance();
    //std::vector<std::vector<std::string>> result = instance->executeQuery("insert into sellable (itemcode, itemname, sellingprice) values('10', 'testing', 100.00) returning *");
    //util::printTable(result);
    util::AccountingTransactionTable* myTable = util::AccountingTransactionTable::getInstance();
    std::vector<util::TableCondition> conditions;
    std::string val = "01-01-2024";
    util::TableCondition newCondition, cond2;
    newCondition.col = myTable->getColumn("transaction_date");
    newCondition.value = val;
    newCondition.comparator = util::TableComparator::LESSTHAN;
    conditions.push_back(newCondition);
    std::vector<std::vector<std::string>> res = myTable->getRecords(conditions);
    printTable(res);
    return 0;
}

void printVec(std::vector<std::string> &v){
    for (std::string &s : v){
        std::cout << s << " ";
    }
} 

void printTable(std::vector<std::vector<std::string>> &t){
    for (std::vector<std::string> &v : t){
        printVec(v);
        std::cout << '\n';
    }
}