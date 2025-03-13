#include "util/class/base_class.hpp"
using namespace util;
baseclass::HasTable::HasTable() {
};

void util::baseclass::HasTable::set_db_code(std::string db_code)
{
    this->db_code = db_code;
}

// void util::baseclass::HasTable::update_to_db_with_table(util::Table *table)
// {
//     std::vector<std::string> args = this->get_update_parameter();
//     table->update_row(this->db_code, args);
// }

std::string util::baseclass::HasTable::get_db_code()
{
    return this->db_code;
}