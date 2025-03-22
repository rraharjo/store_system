#include <iostream>
#include "header_files/accounting/accounting_system.hpp"
#include "header_files/util/date.hpp"

/*int main()
{
    accounting::AccountingSystem *a_system = accounting::AccountingSystem::get_instance();
    accounting::Transaction *transaction1 = new accounting::Transaction("Transaction 1");
    accounting::Entry *entry1 = new accounting::Entry(true, 100, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::CASH);
    accounting::Entry *entry2 = new accounting::Entry(false, 100, util::enums::AccountTitles::LIABILITIES, util::enums::TAccounts::UNEARNEDREV);
    transaction1->add_entry(entry1);
    transaction1->add_entry(entry2);
    a_system->add_transaction(transaction1);
    delete transaction1;
    std::cout << a_system->to_string() << std::endl;
    delete a_system;
    return 0;
}*/

