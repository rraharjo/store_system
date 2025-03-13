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
            std::string primary_key;
            std::string db_code;

        protected:

            HasTable();

            void set_db_code(std::string db_code);

            //void update_to_db_with_table(util::Table *table);

        public:
            std::string get_db_code();

            friend class Collection;
        };
    };
};
#endif