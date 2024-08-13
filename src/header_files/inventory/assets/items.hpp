#include <string>
#include "util/class/base_class.hpp"
#ifndef ITEMINVENTORY_HPP
#define ITEMINVENTORY_HPP
namespace inventory
{
    class Item : public util::baseclass::HasTable
    {
        protected:
            std::string name;
            std::string itemCode;

        protected:
            int databaseCode;
            Item(std::string name, std::string itemCode);
    };
};

#endif