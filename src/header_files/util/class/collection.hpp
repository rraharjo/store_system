#ifndef ITEMCOLLECTION_HPP
#define ITEMCOLLECTION_HPP
#include <memory>
#include "util/database/tables.hpp"
#include "base_class.hpp"
#include "util/date.hpp"
namespace util
{
    namespace baseclass
    {
        class Collection
        {
        protected:
            util::enums::PrimaryKeyPrefix primary_key_prefix;
            util::Table *table;
            Collection(util::enums::PrimaryKeyPrefix primary_key_prefix, util::Table *table);

            void validate_insert(HasTable *new_item);

            void validate_update(HasTable *new_item);

            // This is here because set_db_code is Collection class is friend to HasTable
            void set_db_code(HasTable *new_item, const std::string db_code);

            virtual ~Collection();

        public:
            virtual void insert_new_item(HasTable *new_item) = 0;

            virtual void update_existing_item(HasTable *existing_item) = 0;

            virtual std::unique_ptr<HasTable> get_from_database(std::string db_code) = 0;

            virtual std::vector<std::unique_ptr<HasTable>> get_from_database(std::vector<util::TableCondition> &conditions);
        };
    }
}
#endif