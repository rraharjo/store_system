#include "nlohmann/json.hpp"
#include "store/store_system.hpp"
#include "util/misc.hpp"
#include "util/date.hpp"

#ifndef DRIVER_EXECUTOR
#define DRIVER_EXECUTOR
#define ENDCMD "ENDCMD"
#define ADD_INV 1
#define PURC_INV 2
#define PURC_ASS 3
#define CAPT_ASS 4
#define SELL_INV 5
#define SELL_ASS 6
#define EO_YEAR 7

namespace storedriver
{
    class Executor
    {
    protected:
        nlohmann::json request;

        virtual void execute(store::StoreSystem *) = 0;

    public:
        static void execute(store::StoreSystem *, std::string, bool);

        Executor(nlohmann::json);
    };

    class AddInventoryExecutor : public Executor
    {
    protected:
    public:
        void execute(store::StoreSystem *) override;
        AddInventoryExecutor(nlohmann::json);
    };
    class PurchaseInventoryExecutor : public Executor
    {
    private:
        void add_purchase_entry(store::PurchaseTransaction *p, std::string item_db, double price_per_item, int item_qty);

    public:
        void execute(store::StoreSystem *) override;
        PurchaseInventoryExecutor(nlohmann::json);
    };

    class PurchaseAssetsExecutor : public Executor
    {

    public:
        void execute(store::StoreSystem *) override;
        PurchaseAssetsExecutor(nlohmann::json);
    };

    class CapitalizeAssetExecutor : public Executor
    {

    public:
        void execute(store::StoreSystem *) override;
        CapitalizeAssetExecutor(nlohmann::json);
    };
    class SellInventoryExecutor : public Executor
    {

    public:
        void execute(store::StoreSystem *) override;
        SellInventoryExecutor(nlohmann::json);
    };
    class SellAssetExecutor : public Executor
    {

    public:
        void execute(store::StoreSystem *) override;
        SellAssetExecutor(nlohmann::json);
    };
    class EndOfYearExecutor : public Executor
    {

    public:
        void execute(store::StoreSystem *) override;
        EndOfYearExecutor(nlohmann::json);
    };
}
#endif