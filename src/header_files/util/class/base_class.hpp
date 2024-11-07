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

            HasTable();

            void setDBCode(std::string dbCode);

            virtual std::vector<std::string> getInsertParameter() = 0;

            virtual std::vector<std::string> getUpdateParameter() = 0;

            void insertToDBWithTable(util::Table *table);

            void updateToDBWithTable(util::Table *table);

        public:
            std::string getDBCode();

            virtual void insertToDB() = 0;

            virtual void updateToDB() = 0;
        };
    };
};
#endif