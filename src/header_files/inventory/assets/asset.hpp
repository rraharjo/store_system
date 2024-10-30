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
        static util::Table *classTable;
        int yearUsefulLife;
        util::Date *dateBought;

        std::vector<std::string> getUpdateParameter() override;

        Asset(std::string dbCode, std::string name, std::string itemCode,
              double totalValue, double residualValue, int yearUsefulLife, util::Date *dateBought, util::Date *dateSold);

        Asset(std::string name, std::string itemCode, double residualValue, int yearUsefulLife, util::Date *dateBought);

    public:
        void insertToDB() override;

        void updateToDB() override;

        void addPurchase(PurchaseEntry *entry) override;

        double sellItems(SellingEntry *entry) override;

        double getResidualValue();

        int getYearUsefulLife();

        util::Date *getDateBought();

        util::Date *getExpiryDate();

        double getTotalValue();

        double getCurrentValue();

        virtual void setTotalValue(double newValue);

        std::string toString();
    };
}
#endif