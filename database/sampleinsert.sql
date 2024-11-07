insert into inventory (
    database_code,
    item_code,
    item_name,
    selling_price
) values(
    concat('INV', lpad(nextval('inventory_seq')::text, 6, '0')),
    '',
    'Some item',
    1000
);

insert into purchase_transaction(database_code, date_purchased, seller, is_finished) 
values(concat('PTR', lpad(nextval('purchase_transaction_seq')::text, 6, '0')), 
to_date('22-10-2024', 'dd-MM-yyyy'),
'seller1',
true) returning *;