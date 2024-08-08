#include "util/date.hpp"
#ifndef INVENTORYSTRUCTS
#define INVENTORYSTRUCTS
namespace inventory
{
    typedef struct transactionEntry
    {
        int itemDBCode;
        int transactionDBCode;
        util::Date *purchaseDate;
        int qty;
        double price;
    } TransactionEntry;
}
#endif