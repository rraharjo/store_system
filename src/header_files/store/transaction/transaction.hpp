#include <string>
#include <vector>
#include <memory>
#include "util/date.hpp"
#include "util/database/tables.hpp"
#include "util/class/base_class.hpp"
#include "inventory/transaction/entry.hpp"
#ifndef STORETRANSACTION_HPP
#define STORETRANSACTION_HPP
namespace util
{
    namespace baseclass
    {
        class PurchaseTransactionCollection;
        class SellingTransactionCollection;
    }
}
namespace store
{
    class Transaction : public util::baseclass::HasTable
    {
    private:
        std::unique_ptr<util::Date> transaction_date;
        double paid_cash;
        double paid_credit;
        std::vector<std::shared_ptr<inventory::Entry>> entries;

    protected:
        bool is_finished;

        Transaction(util::enums::PrimaryKeyPrefix primary_key_prefix,
                    std::string db_code,
                    util::Date *transaction_date,
                    double paid_cash,
                    double paid_credit);

        Transaction(util::enums::PrimaryKeyPrefix primary_key_prefix, util::Date *transaction_date);

        virtual ~Transaction();

    public:
        util::Date *get_date();

        double get_paid_cash();

        double get_paid_credit();

        double get_transaction_amount();

        void add_entry(inventory::Entry *entry);

        void set_paid_cash(double amount);

        void set_paid_credit(double amount);

        std::vector<std::shared_ptr<inventory::Entry>> get_all_entries();
    };

    /*****************************************PURCHASETRANSACTION*****************************************/

    class PurchaseTransaction : public Transaction
    {
    private:
        std::string seller;

    public:
        PurchaseTransaction(std::string seller, util::Date *purchase_date);

        PurchaseTransaction(std::string db_code, std::string seller, util::Date *purchase_date, double paid_cash, double paid_credit);

        std::string get_seller();

        friend class util::baseclass::PurchaseTransactionCollection;
    };

    /*****************************************SELLINGTRANSACTION*****************************************/

    class SellingTransaction : public Transaction
    {
    public:
        SellingTransaction(util::Date *transaction_date);

        SellingTransaction(std::string db_code, util::Date *transaction_date, double paid_cash, double paid_credit);

        friend class util::baseclass::SellingTransactionCollection;
    };
};
#endif