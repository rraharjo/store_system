#include "util/depreciation.hpp"
#include "util/date.hpp"
#include "inventory/assets/asset.hpp"
#ifndef DEPRECIABLEINVENTORY_HPP
#define DEPRECIABLEINVENTORY_HPP
namespace inventory
{
    class Depreciable: public Asset
    {
    private:
        static int nextItemCode;
        util::DepreciationMethod *depreciationMethod;

    protected:

        std::string createDBCode() override;

    public:

        Depreciable(std::string name, std::string itemCode, double residualValue, int yearUsefulLife, util::Date *dateBought);

        double getDepreciationExpenseAtYear(int year);

        double getAccumulatedDepreciationAtYear(int year);

        double getValueAtYear(int year);

        double getCurrentDepreciationExpense();

        double getCurrentAccumulatedDepreciation();

        double getCurrentValue();

        void setTotalValue(double newValue);
    };
}

#endif