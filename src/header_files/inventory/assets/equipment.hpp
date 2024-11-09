#include "util/depreciation.hpp"
#include "util/date.hpp"
#include "inventory/assets/asset.hpp"
#ifndef EQUIPMENTINVENTORY_HPP
#define EQUIPMENTINVENTORY_HPP
namespace inventory
{
    class Equipment : public Asset
    {
    private:
        util::DepreciationMethod *depreciationMethod;

    protected:
        void addExistingPurchaseEntry(PurchaseEntry *entry) override;

        std::vector<std::string> getInsertParameter() override;

    public:
        static std::vector<Asset *> generateFromDatabase();

        void setTotalValue(double newValue) override;

        Equipment(std::string dbCode, std::string name, std::string itemCode,
                  double totalValue, double residualValue, int yearUsefulLife, util::Date *dateBought, util::Date *dateSold);

        Equipment(std::string name, std::string itemCode, double residualValue, int yearUsefulLife, util::Date *dateBought);

        double getReducedValueAtYear(int year) override;

        double getReducedValueCurrentYear() override;

        double getDepreciationExpenseAtYear(int year);

        double getAccumulatedDepreciationAtYear(int year);

        double getValueAtYear(int year);

        double getCurrentDepreciationExpense();

        double getCurrentAccumulatedDepreciation();

        double getCurrentValue();
    };
}

#endif