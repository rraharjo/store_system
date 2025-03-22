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
#define INV_INFO 8
#define ASSETS_INFO 9
// Below aren't implemented
#define INV_PURC_HIST 10
#define INV_SELL_HIST 11
#define ASSETS_PURC_HIST 12
#define ASSETS_SELL_HIST 13

namespace storedriver
{
    class Executor
    {
    protected:
        nlohmann::json request;

        virtual nlohmann::json execute(store::StoreSystem *) = 0;

        virtual void validate_request(store::StoreSystem *s_system = NULL);

    public:
        static nlohmann::json execute(store::StoreSystem *, std::string, bool);

        Executor(nlohmann::json);

        virtual ~Executor();
    };

    class AddInventoryExecutor : public Executor
    {
    protected:
    public:
        nlohmann::json execute(store::StoreSystem *) override;

        AddInventoryExecutor(nlohmann::json);

        ~AddInventoryExecutor();
    };
    class PurchaseInventoryExecutor : public Executor
    {
    private:
        void add_purchase_entry(store::PurchaseTransaction *p, std::string item_db, double price_per_item, int item_qty);

    public:
        nlohmann::json execute(store::StoreSystem *) override;

        PurchaseInventoryExecutor(nlohmann::json);

        ~PurchaseInventoryExecutor();
    };

    class PurchaseAssetsExecutor : public Executor
    {

    public:
        nlohmann::json execute(store::StoreSystem *) override;

        PurchaseAssetsExecutor(nlohmann::json);

        ~PurchaseAssetsExecutor();
    };

    class CapitalizeAssetExecutor : public Executor
    {

    public:
        nlohmann::json execute(store::StoreSystem *) override;

        CapitalizeAssetExecutor(nlohmann::json);

        ~CapitalizeAssetExecutor();
    };
    class SellInventoryExecutor : public Executor
    {
    protected:
        void validate_request(store::StoreSystem *s_system = NULL) override;

    public:
        nlohmann::json execute(store::StoreSystem *) override;

        SellInventoryExecutor(nlohmann::json);

        ~SellInventoryExecutor();
    };
    class SellAssetExecutor : public Executor
    {

    public:
        nlohmann::json execute(store::StoreSystem *) override;

        SellAssetExecutor(nlohmann::json);

        ~SellAssetExecutor();
    };
    class EndOfYearExecutor : public Executor
    {

    public:
        nlohmann::json execute(store::StoreSystem *) override;

        EndOfYearExecutor(nlohmann::json);

        ~EndOfYearExecutor();
    };

    class InventoriesInfoExecutor : public Executor
    {

    public:
        nlohmann::json execute(store::StoreSystem *) override;

        InventoriesInfoExecutor(nlohmann::json);

        ~InventoriesInfoExecutor();
    };

    class AssetsInfoExecutor : public Executor
    {

    public:
        nlohmann::json execute(store::StoreSystem *) override;

        AssetsInfoExecutor(nlohmann::json);

        ~AssetsInfoExecutor();
    };
}
#endif