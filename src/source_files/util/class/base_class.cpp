#include "util/class/base_class.hpp"
using namespace util;
baseclass::HasTable::HasTable() {
};

void util::baseclass::HasTable::setDBCode(std::string dbCode)
{
    this->dbCode = dbCode;
}

void util::baseclass::HasTable::insertToDBWithTable(util::Table *table, bool setDBCode)
{
    std::vector<std::string> args = this->getInsertParameter();
    if (setDBCode)
    {
        this->setDBCode(table->insertRow(args)[0]);
    }
    else{
        table->insertRow(args);
    }
}

void util::baseclass::HasTable::updateToDBWithTable(util::Table *table)
{
    std::vector<std::string> args = this->getUpdateParameter();
    table->updateRow(this->dbCode, args);
}

std::string util::baseclass::HasTable::getDBCode()
{
    return this->dbCode;
}