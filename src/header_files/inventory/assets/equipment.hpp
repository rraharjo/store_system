#ifndef EQUIPMENTINVENTORY_HPP
#define EQUIPMENTINVENTORY_HPP
#include "util/depreciation.hpp"
#include "util/date.hpp"
#include "inventory/assets/asset.hpp"

namespace util
{
    namespace baseclass
    {
        class EquipmentCollection;
    }
}

namespace inventory
{
    class Equipment : public Asset
    {
    private:
        std::unique_ptr<util::DepreciationMethod> depreciation_method;

    public:
        void set_total_value(double new_value) override;

        Equipment(std::string db_code,
                  std::string name,
                  std::string item_code,
                  double total_value,
                  double residual_value,
                  int year_useful_life,
                  std::unique_ptr<util::Date> date_bought,
                  std::unique_ptr<util::Date> last_depreciation_date,
                  std::unique_ptr<util::Date> date_sold);

        Equipment(std::string name,
                  std::string item_code,
                  double residual_value,
                  int year_useful_life,
                  std::unique_ptr<util::Date> date_bought);

        ~Equipment();

        double get_reduced_value_at_year(int year) override;

        double get_reduced_value_current_year() override;

        double get_depreciation_expense_at_year(int year);

        double get_accumulated_depreciation_at_year(int year);

        double get_value_at_year(int year);

        double get_current_depreciation_expense();

        double get_current_accumulated_depreciation();

        double get_current_value();

        friend class util::baseclass::EquipmentCollection;
    };
}

#endif