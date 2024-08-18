create database store_system;

drop table selling_entry;
drop table purchase_entry;
drop table selling_transaction;
drop table purchase_transaction;
drop table sellable;
drop table accounting_transaction_entry;
drop table accounting_transaction;

create table sellable(
    database_code    serial          primary key,
    item_code        varchar(255),
    item_name        varchar(255),
    selling_price    numeric(12, 2)    not null
);

create table purchase_transaction(
    database_code    serial      primary key,
    date_purchased   date        not null,
    seller          varchar(100)
);

create table purchase_entry(
    database_code    serial          primary key,
    sellable_db_code  int             not null,
    purchase_db_Code  int             not null,
    purchase_price   numeric(12, 2)    not null,
    qty             numeric(7)       not null,
    available_qty    numeric(7)       not null
);

alter table purchase_entry
add constraint fk_purchase_transaction foreign key (purchase_db_code)
    references purchase_transaction (database_code);

alter table purchase_entry
add constraint fk_sellable foreign key (sellable_db_code)
    references sellable (database_code);

create table selling_transaction(
    database_code       serial          primary key,
    transaction_date    date            not null      
);

create table selling_entry(
    database_code       serial          primary key,
    sellable_db_code    int,
    selling_transaction_db_code int,
    qty                 int
);

alter table selling_entry
add constraint fk_sellable_selling_entry foreign key (sellable_db_code)
    references sellable (database_code);

alter table selling_entry
add constraint fk_selling_transaction foreign key (selling_transaction_db_code)
    references selling_transaction (database_code);

create table accounting_transaction(
    database_code       serial          primary key,
    transaction_name    varchar(50),
    transaction_date    date
);

create table accounting_transaction_entry(
    database_code       serial          primary key,
    at_db_code          int             not null,
    debit               boolean         not null,
    amount              numeric(12, 2)  not null,
    t_account_number    varchar(25),
    account_title       varchar(10)
);

alter table accounting_transaction_entry
add constraint fk_accounting_transaction foreign key (at_db_code)
    references accounting_transaction (database_code);