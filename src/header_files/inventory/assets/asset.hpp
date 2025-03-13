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
        util::Date *expiry_date;
        util::Date *last_depreciation_date;
        util::Date *date_bought;

        void add_existing_purchase_entry(PurchaseEntry *entry) override;

        Asset(std::string db_code, std::string name, std::string item_code,
              double total_value, double residual_value, int year_useful_life,
              util::Date *date_bought, util::Date *last_depreciation_date, util::Date *date_sold);

        Asset(std::string name, std::string item_code, double residual_value, int year_useful_life, util::Date *date_bought);

    public:
        void add_purchase(PurchaseEntry *entry) override;

        double sell_items(SellingEntry *entry) override;

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

        void set_last_depreciation_date(util::Date *);

        std::string to_string();

        friend class Equipment;
    };
}
#endif