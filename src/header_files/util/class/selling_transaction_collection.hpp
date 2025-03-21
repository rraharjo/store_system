#ifndef SELLINGTRANSACTIONCOLLECTION_HPP
#define SELLINGTRANSACTIONCOLLECTION_HPP
#include <memory>
#include "collection.hpp"
#include "selling_entries_collection.hpp"
#include "store/transaction/transaction.hpp"
namespace util
{
    namespace baseclass
    {
        class SellingTransactionCollection : public Collection
        {
        private:
            std::unique_ptr<SellingEntriesCollection> selling_entries;

        public:
            SellingTransactionCollection();

            ~SellingTransactionCollection();

            void insert_new_item(HasTable *new_item) override;

            HasTable *get_from_database(std::string db_code) override;

            void update_existing_item(HasTable *existing_item) override;
        };
    }
}
#endif