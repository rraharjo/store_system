#ifndef ITEMCOLLECTION_HPP
#define ITEMCOLLECTION_HPP
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
            std::string primary_key;
            util::Table *table;
            Collection(std::string primary_key, util::Table *table);
        
            void validate_insert(HasTable *new_item);

            void validate_update(HasTable *new_item);

            //This is here because set_db_code is Collection class is friend to HasTable
            void set_db_code(HasTable *new_item, const std::string db_code);

        public:
            //Make this pure virtual
            virtual void insert_new_item(HasTable *new_item) = 0;

            virtual void update_existing_item(HasTable *existing_item) = 0;

            virtual HasTable *get_from_database(std::string db_code) = 0;

            virtual std::vector<HasTable*> get_from_database(std::vector<util::TableCondition>& conditions);
        };
    }
}
#endif