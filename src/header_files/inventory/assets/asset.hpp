#include "util/depreciation.hpp"
#include "util/date.hpp"
#include "inventory/assets/items.hpp"
#ifndef INVENTORYASSET_HPP
#define INVENTORYASSET_HPP
namespace inventory
{
    class Asset : public Item
    {
    private:
        std::string name;
        double value = 0;
        double residualValue;
        util::Date *expiryDate;

    protected:
        int yearUsefulLife;
        util::Date *dateBought;
        void setTable() override;

        std::vector<std::string> getInsertParameter() override;

        Asset(std::string name, std::string itemCode, double residualValue, int yearUsefulLife, util::Date *dateBought);

    public:
        void addPurchase(PurchaseEntry *entry) override;

        double sellItems(SellingEntry *entry) override;

        double getResidualValue();

        int getYearUsefulLife();

        util::Date *getDateBought();

        util::Date *getExpiryDate();

        double getTotalValue();

        double getCurrentValue();

        void setTotalValue(double newValue);

        std::string toString();
    };
}
#endif