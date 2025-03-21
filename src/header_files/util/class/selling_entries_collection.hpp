#ifndef SELLINGENTRIESCOLLECTION_HPP
#define SELLINGENTRIESCOLLECTION_HPP
#include "collection.hpp"
#include "inventory/transaction/entry.hpp"

namespace util
{
    namespace baseclass
    {
        class SellingEntriesCollection : public Collection
        {
        public:
            SellingEntriesCollection();

            ~SellingEntriesCollection();

            void insert_new_item(HasTable *new_item) override;

            HasTable *get_from_database(std::string db_code) override;

            void update_existing_item(HasTable *existing_item) override;

            void set_item_db_code(inventory::SellingEntry *new_entry, std::string db_code);

            std::vector<HasTable *> get_from_database(std::vector<util::TableCondition> &conditions) override;
        };
    }
}

#endif