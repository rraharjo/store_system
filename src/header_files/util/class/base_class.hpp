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
            int dbCode;

        protected:
            util::Table *table;

            HasTable();

            void setDBCode(int dbCode);

            virtual void setTable() = 0;

            virtual std::vector<std::string> getInsertParameter() = 0;

        public:
            int getDBCode();

            void insertToDB();
        };
    };
};
#endif