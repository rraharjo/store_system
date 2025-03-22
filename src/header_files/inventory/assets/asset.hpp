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
        std::string name;
        double value = 0;
        double residual_value;
        int year_useful_life;
        std::unique_ptr<util::Date> expiry_date;
        std::unique_ptr<util::Date> last_depreciation_date;
        std::unique_ptr<util::Date> date_bought;

        void add_existing_purchase_entry(std::unique_ptr<PurchaseEntry> entry) override;

        Asset(util::enums::PrimaryKeyPrefix primary_key_prefix,
              std::string db_code,
              std::string name,
              std::string item_code,
              double total_value,
              double residual_value,
              int year_useful_life,
              std::unique_ptr<util::Date> date_bought,
              std::unique_ptr<util::Date> last_depreciation_date,
              std::unique_ptr<util::Date> date_sold);

        Asset(util::enums::PrimaryKeyPrefix primary_key_prefix,
              std::string name,
              std::string item_code,
              double residual_value,
              int year_useful_life,
              std::unique_ptr<util::Date> date_bought);

    public:
        virtual ~Asset();

        void add_purchase(std::shared_ptr<PurchaseEntry> entry) override;

        double sell_items(std::shared_ptr<SellingEntry> entry) override;

        virtual double get_reduced_value_at_year(int year) = 0;

        virtual double get_reduced_value_current_year() = 0;

        double get_residual_value();

        int get_year_useful_life();

        util::Date *get_date_bought();

        util::Date *get_last_depreciation_date();

        util::Date *get_expiry_date();

        double get_total_value();

        double get_current_value();

        virtual void set_total_value(double new_value);

        void set_last_depreciation_date(std::unique_ptr<util::Date> new_date);

        std::string to_string();

        friend class Equipment;
    };
}
#endif