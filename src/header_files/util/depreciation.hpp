#include <vector>
#ifndef DEPRECIATION_HPP
#define DEPRECIATION_HPP
namespace util
{
    class DepreciationMethod
    {
    protected:
        double cost;
        double residual_value;
        int year_useful_life;

    public:
        virtual double get_accumulated_depreciation_at_year(int year) = 0;

        virtual double get_depreciation_expense_at_year(int year) = 0;

        virtual void set_cost(double cost);

        DepreciationMethod(double cost, double residual_value, int year_useful_life);
    };

    class StraightLineDepreciation : public DepreciationMethod
    {
    public:
        StraightLineDepreciation(double cost, double residual_value, int year_useful_life);

        double get_accumulated_depreciation_at_year(int year) override;

        double get_depreciation_expense_at_year(int year) override;
    };

    class DoubleDecliningDepreciation : public DepreciationMethod
    {
    private:
        std::vector<double> accumulated_depreciation;

        void initiate_accumulated_depreciation();

    public:
        void set_cost(double cost) override;

        DoubleDecliningDepreciation(double cost, int year_useful_life);

        double get_accumulated_depreciation_at_year(int year) override;

        double get_depreciation_expense_at_year(int year) override;
    };

};
#endif