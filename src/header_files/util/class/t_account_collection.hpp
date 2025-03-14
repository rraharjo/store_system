#ifndef TACCOUNTCOLLECTION_HPP
#define TACCOUNTCOLLECTION_HPP
#include "collection.hpp"
#include "purchase_entries_collection.hpp"
#include "accounting/accounts/t_account.hpp"
namespace util
{
    namespace baseclass
    {
        class TAccountCollection : public Collection
        {
        public:
            TAccountCollection();

            void insert_new_item(HasTable *new_item) override;

            HasTable *get_from_database(std::string db_code) override;

            HasTable *get_from_database(util::enums::TAccounts title);

            void update_existing_item(HasTable *existing_item) override;
        };
    }
}
#endif