Environment folders:
1. "./src/source_files/.env"
  -> create a new file to instantiate global variable local_postgres defined in ./src/header_files/.env/private.hpp

External libraries:
1. PostgreSQL C libraries
  -> get from https://www.postgresql.org/download/
  -> add include folder to the project include depending on IDE (e.g. ".vscode/c_cpp_properties.json")
  -> add include and lib file during compilation (e.g. ".vscode/tasks.json")
2. nlohmann json parser
  -> get from https://github.com/nlohmann/json/
  -> add include folder to the project include depending on IDE (e.g. ".vscode/c_cpp_properties.json")
  -> add include file (no Lib or .cpp files) during compilation (e.g. ".vscode/tasks.json")
