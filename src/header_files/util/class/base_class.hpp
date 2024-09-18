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
            std::string dbCode;

        protected:
            util::Table *table;

            HasTable();

            void setDBCode(std::string dbCode);

            virtual void setTable() = 0;

            virtual std::vector<std::string> getInsertParameter() = 0;

            virtual std::string createDBCode() = 0;

        public:
            std::string getDBCode();

            void insertToDB();

            void updateToDB();
        };
    };
};
#endif