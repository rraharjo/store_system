#include <string>
#ifndef ITEMINVENTORY_HPP
#define ITEMINVENTORY_HPP
namespace inventory
{
    class Item
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