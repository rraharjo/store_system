#include "util/class/base_class.hpp"
using namespace util;
baseclass::HasTable::HasTable(){
    //this->setTable();
};

void util::baseclass::HasTable::setDBCode(std::string dbCode){
    this->dbCode = dbCode;
}

void util::baseclass::HasTable::insertToDB(){
    //set the DBCode
    std::vector<std::string> args = this->getInsertParameter();
    this->setDBCode(this->table->insertRow(args)[0]);
}

void util::baseclass::HasTable::updateToDB(){
    std::vector<std::string> args = this->getInsertParameter();
    this->table->updateRow(this->dbCode, args);
}

std::string util::baseclass::HasTable::getDBCode(){
    return this->dbCode;
}