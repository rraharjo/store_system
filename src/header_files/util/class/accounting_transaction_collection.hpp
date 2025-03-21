#ifndef ACCOUNTINGTRANSACTIONCOLLECTION_HPP
#define ACCOUNTINGTRANSACTIONCOLLECTION_HPP
#include <memory>
#include "collection.hpp"
#include "accounting/accounting_transaction/transaction.hpp"
#include "accounting_entries_collection.hpp"

namespace util
{
    namespace baseclass
    {
        class AccountingTransactionCollection : public Collection
        {
        private:
            std::unique_ptr<AccountingEntryCollection> entries_collection;

        public:
            AccountingTransactionCollection();

            ~AccountingTransactionCollection();

            void insert_new_item(HasTable *new_item) override;

            void update_existing_item(HasTable *existing_item) override;

            HasTable *get_from_database(std::string db_code) override;
        };
    }
}

#endif