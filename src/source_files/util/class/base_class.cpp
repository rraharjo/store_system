#include "util/class/base_class.hpp"
using namespace util;
baseclass::HasTable::HasTable(util::enums::PrimaryKeyPrefix primary_key_prefix)
    : primary_key_prefix(primary_key_prefix)
{
}

baseclass::HasTable::~HasTable()
{
#ifdef DEBUG
    std::cout << "Deleting Has Table" << std::endl;
#endif
}

void util::baseclass::HasTable::set_db_code(std::string db_code)
{
    this->db_code = db_code;
}

std::string util::baseclass::HasTable::get_db_code()
{
    return this->db_code;
}