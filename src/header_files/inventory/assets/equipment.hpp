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
        std::vector<std::string> getInsertParameter() override;

    public:
        void setTotalValue(double newValue) override;
        
        Equipment(std::string name, std::string itemCode, double residualValue, int yearUsefulLife, util::Date *dateBought);

        double getDepreciationExpenseAtYear(int year);

        double getAccumulatedDepreciationAtYear(int year);

        double getValueAtYear(int year);

        double getCurrentDepreciationExpense();

        double getCurrentAccumulatedDepreciation();

        double getCurrentValue();
    };
}

#endif