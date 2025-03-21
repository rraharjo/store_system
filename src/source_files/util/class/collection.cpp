#include "util/class/collection.hpp"

namespace util
{
    namespace baseclass
    {
        //Object returned by collection does not belong to the collection class anymore
        Collection::Collection(util::enums::PrimaryKeyPrefix primary_key_prefix, util::Table *table) : primary_key_prefix(primary_key_prefix), table(table) {}

        std::vector<util::baseclass::HasTable *> Collection::get_from_database(std::vector<util::TableCondition> &conditions)
        {
            std::string exception_msg = util::enums::primary_key_prefix_map[this->primary_key_prefix] +
                                        " get_from_database with conditions is unimplemented...";
            throw std::runtime_error(exception_msg);
            return std::vector<util::baseclass::HasTable *>();
        }

        Collection::~Collection(){
            #ifdef DEBUG
                std::cout << "Deleting Collection (Parent) class for " << util::enums::primary_key_prefix_map[this->primary_key_prefix] << std::endl;
            #endif
        }

        void Collection::validate_insert(HasTable *new_item)
        {
            if (new_item->primary_key_prefix != this->primary_key_prefix)
            {
                throw std::invalid_argument("Cannot insert an object of type " +
                                            util::enums::primary_key_prefix_map[new_item->primary_key_prefix] +
                                            " to a table of " + util::enums::primary_key_prefix_map[this->primary_key_prefix] + "...\n");
            }
            if (new_item->get_db_code() != "")
            {
                throw std::invalid_argument("Cannot insert object that has been inserted: object " +
                                            new_item->get_db_code() + "...\n");
            }
        }

        void Collection::validate_update(HasTable *new_item)
        {
            if (new_item->primary_key_prefix != this->primary_key_prefix)
            {
                throw std::invalid_argument("Cannot update an object of type " + util::enums::primary_key_prefix_map[new_item->primary_key_prefix] +
                                            " to a table of " + util::enums::primary_key_prefix_map[this->primary_key_prefix] + "...\n");
            }
            if (new_item->get_db_code() == "")
            {
                throw std::invalid_argument("Cannot update object that has not been inserted: ...\n");
            }
        }

        void Collection::set_db_code(HasTable *new_item, const std::string db_code)
        {
            new_item->set_db_code(db_code);
        }
    }
}
