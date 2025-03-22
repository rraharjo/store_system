#include "util/database/db.hpp"
#include "util/database/tables.hpp"
#include "util/misc.hpp"
#include "accounting/accounting_transaction/transaction.hpp"
#include "inventory/inventory_system.hpp"

void print_vec(std::vector<std::string> &);
void print_table(std::vector<std::vector<std::string>> &);

int main(int argc, char **argv)
{
    util::AssetsTable *myTable = util::AssetsTable::get_instance();
    std::vector<util::TableCondition> conditions;
    std::string val = "01-01-2024";
    util::TableCondition newCondition, cond2;
    newCondition.col = myTable->get_column("transaction_date");
    newCondition.value = val;
    newCondition.comparator = util::TableComparator::LESSTHAN;
    conditions.push_back(newCondition);
    std::vector<std::vector<std::string>> res = myTable->get_records();
    print_table(res);
    std::vector<inventory::Asset *> equipments = inventory::Equipment::generate_from_database();
    for (inventory::Asset *equipment : equipments)
    {
        std::cout << equipment->to_string() << "\n";
    }
    
    return 0;
}

void print_vec(std::vector<std::string> &v)
{
    for (std::string &s : v)
    {
        std::cout << s << " ";
    }
}

void print_table(std::vector<std::vector<std::string>> &t)
{
    for (std::vector<std::string> &v : t)
    {
        print_vec(v);
        std::cout << '\n';
    }
}