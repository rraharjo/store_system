#include "util/class/base_class.hpp"
using namespace util;
baseclass::HasTable::HasTable(){
    //this->setTable();
};

void util::baseclass::HasTable::setDBCode(int dbCode){
    this->dbCode = dbCode;
}

void util::baseclass::HasTable::insertToDB(){
    //set the DBCode
    std::vector<std::string> args = this->getInsertParameter();
    this->setDBCode(std::stoi(this->table->insertRow(args)[0]));
}

void util::baseclass::HasTable::updateToDB(){
    std::vector<std::string> args = this->getInsertParameter();
    this->table->updateRow(this->dbCode, args);
}

int util::baseclass::HasTable::getDBCode(){
    return this->dbCode;
}