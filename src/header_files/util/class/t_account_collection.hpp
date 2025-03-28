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

            ~TAccountCollection();

            void insert_new_item(HasTable *new_item) override;

            std::unique_ptr<HasTable> get_from_database(std::string db_code) override;

            std::unique_ptr<HasTable> get_from_database(util::enums::TAccounts title);

            void update_existing_item(HasTable *existing_item) override;

            std::vector<std::unique_ptr<HasTable>>get_temporary_accounts();
        };
    }
}
#endif