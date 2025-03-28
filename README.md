A. Instruction for starting the program:

Environment folders:
1. "./src/source_files/.env"
  -> create a new file to instantiate global variable local_postgres defined in ./src/header_files/.env/private.hpp

External libraries:
1. PostgreSQL C libraries
  -> get from https://www.postgresql.org/download/
  -> don't forget to add the include folder when compiling (use -lpq flag for the source files)
  -> Note: PostgreSQL 16 was used for development
2. nlohmann json parser
  -> get from https://github.com/nlohmann/json/
  -> don't forget to add the include folder when compiling (no source files needed)
3. Microsoft WinSock2 SDK
  -> get from https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/
  -> Only needed if the program is run on Windows environment

First time setup:
1. Install PostgreSQL, copy the SQL script from ./database/init_database.sql and execute it from PostgreSQL

GUI for this app: https://github.com/rraharjo/store_front


B. input rules
commands rules (tokenized): 
-Commands are tokenized using space character delimiter. 
-Text inside double quotes are not tokenized. 
-Date is always in dd/MM/yyyy format.
-[commands_here]+ means the commands inside square bracket can be repeated once or more time.

commands rules (JSON):
-Date is in dd/MM/yyyy format

commands list:
1. Add a new inventory:
  -format: 1 Product_name item_code selling_price ENDCMD
  -example: 1 "my Product" COD111 100.00 ENDCMD
  -json:
  {
    "main_command": 1,
    "product_name": string,
    "item_code": string,
    "price": double
  }
  -response:
  {
    "status": bool,
    "body": {}
  }

2. Purchase inventory:
  -format: 2 date seller_name [item_db price_each qty]+ paid_cash ENDCMD
  -example: 2 10/10/2024 Orange INV000001 90 100 INV000002 80 100 15000 ENDCMD
  -json:
  {
    "main_command": 2,
    "date": string,
    "seller": string,
    "items": [
      {
        "dbcode": string,
        "price": double,
        "qty": int
      },
    ],
    "paid_cash": double
  }
  -response:
  {
    "status": bool,
    "body": {}
  }

3. Purchase assets:
  -format: 3 date eq_name item_code cost residual yr_useful paid_cash ENDCMD
  -example: 3 10/10/2024 Car "" 12000 1000 10 12000 ENDCMD
  -json:
  json:
  {
    "main_command": 3,
    "date": string,
    "name": string,
    "item_code": string,
    "cost": double,
    "residual_value": double
    "useful_life": int,
    "paid_cash": double
  }
  -response:
  {
    "status": bool,
    "body": {}
  }

4. Capitalize assets:
  -format: 4 date db_code capt_amt paid_cash ENDCMD
  -example: 4 10/10/2024 EQP000001 2500 0 ENDCMD
  -json:
  {
    "main_command": 4,
    "date": string,
    "dbcode": string
    "cost": double,
    "paid_cash": double
  }
  -response:
  {
    "status": bool,
    "body": {}
  }

5. Sell inventory: 
  -format: 5 date [db_code qty]+ paid_cash ENDCMD
  -example: 5 10/10/2024 INV000001 10 0 ENDCMD
  -json:
  {
    "main_command": 5,
    "date": string,
    "items": [
      {
        "dbcode": string,
        "qty": int
      },
    ],
    "paid_cash": double
  }
  -response:
  {
    "status": bool,
    "body": {}
  }

6. Sell assets:
  -format: 6 date db_code price paid_cash ENDCMD
  -example: 6 11/11/2024 EQP000001 10000 10000 ENDCMD
  -json:
  {
    "main_command": 6,
    "date": string,
    "dbcode": string,
    "price": double,
    "paid_cash": double,
  }
  -response:
  {
    "status": bool,
    "body": {}
  }

7. End of year adjustment:
  -format: 7 ENDCMD
  -example: 7 ENDCMD
  -json:
  {
    "main_command": 7
  }
  -response:
  {
    "status": bool,
    "body": {}
  }

8. Inventory information:
  -format: 8 ENDCMD
  -json:
  {
    "main_command": 8
  }
  -response:
  {
    "status": bool,
    "body":
    {
      "data":
      [
        {
          "dbcode": string,
          "item_code": string,
          "name": string,
          "price": double,
        },

      ]
    }
  }

9. Assets information:
  -format: 9 ENDCMD
  -json:
  {
    "main_command": 9
  }
  -response:
  {
    "status": bool,
    "body":
    {
      "data":
      [
        {
          "dbcode": string,
          "name": string,
          "cost": double,
          "residual_value": double,
          "useful_life": int,
          "date_purchased": dd/MM/yyyy,
          "last_depreciation_date": dd/MM/yyyy
        },

      ]
    }
  }


C. Development note
1. pointer notes:
  -> a function returns a raw pointer: ownership belongs to the function
  -> a function returns a unique_ptr: ownership belongs to the caller
  -> a function returns a shared_ptr: both parties have ownership
  -> a function request a raw pointer: ownership belongs to caller
  -> a function request a unique_ptr: ownership belongs to function
  -> a function request a shared_ptr: both parties have ownership
2. MessageHeader format (network):
    |-----------------------------bytes-----------------------------|
    | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10| 11| 12| 13| 14| 15|
    |---------------------------------------------------------------|
  +0|hheader|           header_len          |    payload_len        |
 +16|       |flg|
  -> hheader: 2 bytes, value is always 0x5555
  -> header_len: 8 bytes, length of the header (19 bytes currently)
  -> payload_len: 8 bytes, length of the payload
  -> flag: 1 bytes, not sure what this is for :o