--create database store_system;

delete from selling_entry;
delete from purchase_entry;
delete from selling_transaction;
delete from purchase_transaction;
delete from accounting_transaction_entry;
delete from accounting_transaction;
delete from inventory;
delete from assets;

drop table selling_entry;
drop table purchase_entry;
drop table selling_transaction;
drop table purchase_transaction;
drop table accounting_transaction_entry;
drop table accounting_transaction;
drop table inventory;
drop table assets;

create table inventory(
    database_code    text          primary key,
    item_code        varchar(255),
    item_name        varchar(255),
    selling_price    numeric(12, 2)    not null
);

create table assets(
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
    seller           varchar(100),
    is_finished      boolean    not null
);

create table purchase_entry(
    database_code       text           primary key,
    inventory_db_code    text,
    assets_db_code  text,
    purchase_db_Code    text             not null,
    purchase_price      numeric(12, 2)   not null,
    qty                 numeric(7)       not null,
    available_qty       numeric(7)       not null
);

alter table purchase_entry
add constraint fk_purchase_transaction foreign key (purchase_db_code)
    references purchase_transaction (database_code);

alter table purchase_entry
add constraint fk_inventory foreign key (inventory_db_code)
    references inventory (database_code);

alter table purchase_entry
add constraint fk_assets foreign key (assets_db_code)
    references assets (database_code);

alter table purchase_entry
add constraint inventory_assets_or_not_null check 
    (inventory_db_code is null and assets_db_code is not null or 
        inventory_db_code is not null and assets_db_code is null);

create table selling_transaction(
    database_code       text            primary key,
    transaction_date    date            not null,
    is_finished         boolean         not null      
);

create table selling_entry(
    database_code       text            primary key,
    inventory_db_code    text,
    assets_db_code  text,
    selling_transaction_db_code text,
    selling_price       numeric(12, 2),
    qty                 int
);

alter table selling_entry
add constraint fk_assets_selling_entry foreign key (assets_db_code)
    references assets (database_code);

alter table selling_entry
add constraint fk_inventory_selling_entry foreign key (inventory_db_code)
    references inventory (database_code);

alter table selling_entry
add constraint inventory_assets_or_not_null check 
    (inventory_db_code is null and assets_db_code is not null or 
        inventory_db_code is not null and assets_db_code is null);

alter table selling_entry
add constraint fk_selling_transaction foreign key (selling_transaction_db_code)
    references selling_transaction (database_code);

create table accounting_transaction(
    database_code       text          primary key,
    transaction_name    varchar(50),
    transaction_date    date,
    entity_id text
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