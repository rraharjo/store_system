#include "util/database/db.hpp"
#include "util/database/tables.hpp"
#include "util/misc.hpp"
#include "accounting/accounting_transaction/transaction.hpp"
#include "inventory/inventory_system.hpp"

void printVec(std::vector<std::string> &);
void printTable(std::vector<std::vector<std::string>> &);

int main(int argc, char **argv)
{
    util::AssetsTable *myTable = util::AssetsTable::getInstance();
    std::vector<util::TableCondition> conditions;
    std::string val = "01-01-2024";
    util::TableCondition newCondition, cond2;
    newCondition.col = myTable->getColumn("transaction_date");
    newCondition.value = val;
    newCondition.comparator = util::TableComparator::LESSTHAN;
    conditions.push_back(newCondition);
    std::vector<std::vector<std::string>> res = myTable->getRecords();
    printTable(res);
    std::vector<inventory::Equipment *> equipments = inventory::Equipment::generateFromDatabase();
    for (inventory::Equipment *equipment : equipments)
    {
        std::cout << equipment->toString() << "\n";
    }
    
    return 0;
}

void printVec(std::vector<std::string> &v)
{
    for (std::string &s : v)
    {
        std::cout << s << " ";
    }
}

void printTable(std::vector<std::vector<std::string>> &t)
{
    for (std::vector<std::string> &v : t)
    {
        printVec(v);
        std::cout << '\n';
    }
}