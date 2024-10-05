#include "util/class/base_class.hpp"
using namespace util;
baseclass::HasTable::HasTable(){
    //this->setTable();
};

void util::baseclass::HasTable::setDBCode(std::string dbCode){
    this->dbCode = dbCode;
}

void util::baseclass::HasTable::insertToDBWithTable(util::Table *table){
    //set the DBCode
    std::vector<std::string> args = this->getInsertParameter();
    this->setDBCode(table->insertRow(args)[0]);
}

void util::baseclass::HasTable::updateToDBWithTable(util::Table *table){
    std::vector<std::string> args = this->getInsertParameter();
    table->updateRow(this->dbCode, args);
}

std::string util::baseclass::HasTable::getDBCode(){
    return this->dbCode;
}