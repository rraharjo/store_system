#include "util/depreciation.hpp"
#include "util/date.hpp"
#include "inventory/assets/items.hpp"
#ifndef INVENTORYASSET_HPP
#define INVENTORYASSET_HPP
namespace inventory
{
    class Asset : public Item
    {
    protected:
        static util::Table *classTable;
        std::string name;
        double value = 0;
        double residualValue;
        int yearUsefulLife;
        util::Date *expiryDate;
        util::Date *lastDepreciationDate;
        util::Date *dateBought;

        void addExistingPurchaseEntry(PurchaseEntry *entry) override;

        std::vector<std::string> getUpdateParameter() override;

        Asset(std::string dbCode, std::string name, std::string itemCode,
              double totalValue, double residualValue, int yearUsefulLife,
              util::Date *dateBought, util::Date *lastDepreciationDate, util::Date *dateSold);

        Asset(std::string name, std::string itemCode, double residualValue, int yearUsefulLife, util::Date *dateBought);

    public:
        void insertToDB() override;

        void updateToDB() override;

        void addPurchase(PurchaseEntry *entry) override;

        double sellItems(SellingEntry *entry) override;

        virtual double getReducedValueAtYear(int year) = 0;

        virtual double getReducedValueCurrentYear() = 0;

        double getResidualValue();

        int getYearUsefulLife();

        util::Date *getDateBought();

        util::Date *getLastDepreciationDate();
        
        util::Date *getExpiryDate();

        double getTotalValue();

        double getCurrentValue();

        virtual void setTotalValue(double newValue);

        std::string toString();

        friend class Equipment;
    };
}
#endif