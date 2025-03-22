#ifndef ACCOUNTINGENTRIESCOLLECTION_HPP
#define ACCOUNTINGENTRIESCOLLECTION_HPP
#include "collection.hpp"
#include "accounting/accounting_transaction/entry.hpp"

namespace util
{
    namespace baseclass
    {
        class AccountingEntryCollection : public Collection
        {
        public:
            AccountingEntryCollection();

            ~AccountingEntryCollection();

            void insert_new_item(HasTable *new_item) override;

            void update_existing_item(HasTable *existing_item) override;

            std::unique_ptr<HasTable> get_from_database(std::string db_code) override;

            std::vector<std::unique_ptr<HasTable>> get_from_database(std::vector<util::TableCondition> &conditions) override;
        };
    }
}

#endif