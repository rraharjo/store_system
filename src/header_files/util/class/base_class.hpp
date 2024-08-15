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
        protected:
            util::Table *table;

            HasTable();

            virtual void setTable() = 0;

            int insertToDB(std::vector<std::string> &values);
        };
    };
};
#endif