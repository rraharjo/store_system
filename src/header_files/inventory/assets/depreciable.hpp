#include <chrono>
#include "util/depreciation.hpp"
#include "util/date.hpp"
#include "inventory/assets/items.hpp"
#ifndef DEPRECIABLEINVENTORY_HPP
#define DEPRECIABLEINVENTORY_HPP
namespace inventory
{
    class Depreciable: public Item
    {
    private:
        double purchaseCost;
        double residualValue;
        int yearUsefulLife;
        util::Date *dateBought;
        util::DepreciationMethod *depreciationMethod;

    protected:
        void setTable() override;

        Depreciable(std::string name, std::string itemCode, double purchaseCost, double residualValue, int yearUsefulLife, util::Date *dateBought);

        double getDepreciationExpenseAtYear(int year);

        double getAccumulatedDepreciationAtYear(int year);

        double getValueAtYear(int year);

        double getCurrentDepreciationExpense();

        double getCurrentAccumulatedDepreciation();

        double getCurrentValue();
    };
}

#endif