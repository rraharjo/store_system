#include <vector>
#ifndef DEPRECIATION_HPP
#define DEPRECIATION_HPP
namespace util
{
    // namespace formula
    class DepreciationMethod
    {
    protected:
        double cost;
        double residualValue;
        int yearUsefulLife;

    public:
        virtual double getAccumulatedDepreciationAtYear(int year) = 0;

        virtual double getDepreciationExpenseAtYear(int year) = 0;

        virtual void setCost(double cost);

        DepreciationMethod(double cost, double residualValue, int yearUsefulLife);
    };

    class StraightLineDepreciation : public DepreciationMethod
    {
    public:
        StraightLineDepreciation(double cost, double residualValue, int yearUsefulLife);

        double getAccumulatedDepreciationAtYear(int year) override;

        double getDepreciationExpenseAtYear(int year) override;
    };

    class DoubleDecliningDepreciation : public DepreciationMethod
    {
    private:
        std::vector<double> accumulatedDepreciation;

        void initiateAccumulatedDepreciation();

    public:
        void setCost(double cost) override;

        DoubleDecliningDepreciation(double cost, int yearUsefulLife);

        double getAccumulatedDepreciationAtYear(int year) override;

        double getDepreciationExpenseAtYear(int year) override;
    };

};
#endif