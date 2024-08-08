#include "util/class/base_class.hpp"
using namespace util;
baseclass::HasTable::HasTable(){
    //this->setTable();
};

int util::baseclass::HasTable::insertToDB(std::vector<std::string> &values){
    return std::stoi(this->table->insertRow(values)[0]);
}

int util::baseclass::HasTable::getDBCode(){
    return this->dbCode;
}