#include "util/database/tables.hpp"
#include <vector>
#include <string>
#ifndef BASECLASS_HPP
#define BASECLASS_HPP
namespace util
{
    namespace baseclass
    {
        class HasTable
        {
        private:
            std::string db_code;

        protected:

            HasTable();

            void set_db_code(std::string db_code);

            virtual std::vector<std::string> get_insert_parameter() = 0;

            virtual std::vector<std::string> get_update_parameter() = 0;

            void insert_to_db_with_table(util::Table *table, bool set_db_code = true);

            void update_to_db_with_table(util::Table *table);

        public:
            std::string get_db_code();

            virtual void insert_to_db() = 0;

            virtual void update_to_db() = 0;
        };
    };
};
#endif