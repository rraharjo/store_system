#include <string>
#ifndef COLUMN_SCHEMA_HPP
#define COLUMN_SCHEMA_HPP
namespace util
{

    namespace enums
    {
        enum class ColumnTypes
        {
            SERIALCOL = 0,
            NUMBERCOL,
            FLOATCOL,
            TEXTCOL,
            DATECOL,
            BOOLCOL
        };
    }
    typedef struct columnSchema
    {
        std::string columnName;
        enums::ColumnTypes type;
    } ColumnSchema;
};
#endif