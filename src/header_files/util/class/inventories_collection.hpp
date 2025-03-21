#ifndef INVENTORIESCOLLECTION_HPP
#define INVENTORIESCOLLECTION_HPP
#include "collection.hpp"
#include "inventory/assets/inventory.hpp"
#include "purchase_entries_collection.hpp"
#include "selling_entries_collection.hpp"

namespace util
{
    namespace baseclass
    {
        class InventoryCollection : public Collection
        {
        private:
            std::unique_ptr<PurchaseEntriesCollection> purchase_history_collection;
            std::unique_ptr<SellingEntriesCollection> selling_history_collection;

        public:
            InventoryCollection();

            ~InventoryCollection();

            void insert_new_item(HasTable *new_item) override;

            void update_existing_item(HasTable *existing_item) override;

            HasTable *get_from_database(std::string db_code) override;

            std::vector<HasTable *> get_from_database(std::vector<util::TableCondition>& conditions) override;
        };
    }
}

#endif