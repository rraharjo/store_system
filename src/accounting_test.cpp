#include <iostream>
#include "header_files/accounting/accounting_system.hpp"
#include "header_files/util/date.hpp"

int main()
{
    accounting::AccountingSystem *aSystem = accounting::AccountingSystem::getInstance();
    accounting::Transaction *transaction1 = new accounting::Transaction("Transaction 1");
    accounting::Entry *entry1 = new accounting::Entry(true, 100, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::CASH);
    accounting::Entry *entry2 = new accounting::Entry(false, 100, util::enums::AccountTitles::LIABILITIES, util::enums::TAccounts::UNEARNEDREV);
    transaction1->addEntry(entry1);
    transaction1->addEntry(entry2);
    aSystem->addTransaction(transaction1);
    //std::cout << entry1->getTransactionTitle() << std::endl;
    std::cout << aSystem->to_string() << std::endl;
    return 0;
}

