#include "util/database/tables.hpp"
#include <vector>
#include <string>
#ifndef BASECLASS_HPP
#define BASECLASS_HPP
namespace util
{
    namespace baseclass
    {
        class Collection;

        class HasTable
        {
        private:
            util::enums::PrimaryKeyPrefix primary_key_prefix;
            std::string db_code;

        protected:
            HasTable(util::enums::PrimaryKeyPrefix primary_key_prefix);

            void set_db_code(std::string db_code);

            // void update_to_db_with_table(util::Table *table);
        public:
            virtual ~HasTable();
            
            std::string get_db_code();

            friend class Collection;
        };
    };
};
#endif