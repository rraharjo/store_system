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
        std::string name;
        double purchaseCost;
        double residualValue;
        int yearUsefulLife;
        util::Date *dateBought;
        util::Date *dateSold;
        util::DepreciationMethod *depreciationMethod;

    protected:
        void setTable() override;

        std::vector<std::string> getInsertParameter() override;

    public:

        Depreciable(std::string name, std::string itemCode, double purchaseCost, double residualValue, int yearUsefulLife, util::Date *dateBought);

        void dispose(util::Date *disposeDate);

        void dispose();

        double getPurchaseCost();
        
        double getResidualValue();

        int getYearUsefulLife();

        util::Date* getDateBought();

        util::Date* getDateSold();

        double getDepreciationExpenseAtYear(int year);

        double getAccumulatedDepreciationAtYear(int year);

        double getValueAtYear(int year);

        double getCurrentDepreciationExpense();

        double getCurrentAccumulatedDepreciation();

        double getCurrentValue();

        std::string toString();
    };
}

#endif