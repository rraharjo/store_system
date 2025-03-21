#ifndef TRANSACTIONCOLLECTION_HPP
#define TRANSACTIONCOLLECTION_HPP
#include <memory>
#include "collection.hpp"
#include "purchase_entries_collection.hpp"
#include "store/transaction/transaction.hpp"
namespace util
{
    namespace baseclass
    {
        class PurchaseTransactionCollection : public Collection
        {
        private:
            std::unique_ptr<PurchaseEntriesCollection> purchase_entries;

        public:
            PurchaseTransactionCollection();

            ~PurchaseTransactionCollection();

            void insert_new_item(HasTable *new_item) override;

            HasTable *get_from_database(std::string db_code) override;

            void update_existing_item(HasTable *existing_item) override;

            std::vector<HasTable *> get_from_database(std::vector<util::TableCondition> &conditions) override;
        };
    }
}
#endif