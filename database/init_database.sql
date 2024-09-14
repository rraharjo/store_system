--create database store_system;

delete from selling_entry;
delete from purchase_entry;
delete from selling_transaction;
delete from purchase_transaction;
delete from sellable;
delete from accounting_transaction_entry;
delete from accounting_transaction;
delete from depreciable_table;

drop table selling_entry;
drop table purchase_entry;
drop table selling_transaction;
drop table purchase_transaction;
drop table sellable;
drop table accounting_transaction_entry;
drop table accounting_transaction;
drop table depreciable_table;

create table sellable(
    database_code    text          primary key,
    item_code        varchar(255),
    item_name        varchar(255),
    selling_price    numeric(12, 2)    not null
);

create table depreciable_table(
    database_code       text          primary key,
    item_name           text,
    purchase_cost       numeric(12, 2)  not null,
    residual_value      numeric(12, 2)  not null,
    year_useful_life    int             not null,
    date_purchased      date            not null,
    date_sold           date
);

create table purchase_transaction(
    database_code    text      primary key,
    date_purchased   date        not null,
    seller           varchar(100)
);

create table purchase_entry(
    database_code       text           primary key,
    sellable_db_code    text,
    properties_db_code  text,
    purchase_db_Code    text             not null,
    purchase_price      numeric(12, 2)   not null,
    qty                 numeric(7)       not null,
    available_qty       numeric(7)       not null
);

alter table purchase_entry
add constraint fk_purchase_transaction foreign key (purchase_db_code)
    references purchase_transaction (database_code);

alter table purchase_entry
add constraint fk_sellable foreign key (sellable_db_code)
    references sellable (database_code);

alter table purchase_entry
add constraint fk_properties foreign key (properties_db_code)
    references depreciable_table (database_code);

alter table purchase_entry
add constraint sellable_properties_or_not_null check 
    (sellable_db_code is null and properties_db_code is not null or 
        sellable_db_code is not null and properties_db_code is null);

create table selling_transaction(
    database_code       text            primary key,
    transaction_date    date            not null      
);

create table selling_entry(
    database_code       text            primary key,
    sellable_db_code    text,
    properties_db_code  text,
    selling_transaction_db_code text,
    selling_price       numeric(12, 2),
    qty                 int
);

alter table selling_entry
add constraint fk_properties_selling_entry foreign key (properties_db_code)
    references depreciable_table (database_code);

alter table selling_entry
add constraint fk_sellable_selling_entry foreign key (sellable_db_code)
    references sellable (database_code);

alter table selling_entry
add constraint sellable_properties_or_not_null check 
    (sellable_db_code is null and properties_db_code is not null or 
        sellable_db_code is not null and properties_db_code is null);

alter table selling_entry
add constraint fk_selling_transaction foreign key (selling_transaction_db_code)
    references selling_transaction (database_code);

create table accounting_transaction(
    database_code       text          primary key,
    transaction_name    varchar(50),
    transaction_date    date
);

create table accounting_transaction_entry(
    database_code       text            primary key,
    at_db_code          text            not null,
    debit               boolean         not null,
    amount              numeric(12, 2)  not null,
    t_account_number    varchar(25),
    account_title       varchar(50)
);

alter table accounting_transaction_entry
add constraint fk_accounting_transaction foreign key (at_db_code)
    references accounting_transaction (database_code);