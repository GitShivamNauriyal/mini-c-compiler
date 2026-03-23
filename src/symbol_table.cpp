#include "symbol_table.h"
#include <iomanip> // Needed for formatting the output table cleanly

bool SymbolTable::insert(const std::string &name, DataType type, int line)
{
    if (exists(name))
    {
        return false;
    }

    SymbolInfo info = {name, type, line};
    table[name] = info;
    return true;
}

SymbolInfo *SymbolTable::lookup(const std::string &name)
{
    if (exists(name))
    {
        return &table[name];
    }
    return nullptr;
}

bool SymbolTable::exists(const std::string &name)
{
    return table.find(name) != table.end();
}

void SymbolTable::printTable()
{
    std::cout << "\n================ SYMBOL TABLE ================\n";
    std::cout << std::left << std::setw(15) << "Identifier"
              << std::setw(15) << "Data Type"
              << "Line Declared\n";
    std::cout << "----------------------------------------------\n";

    for (const auto &pair : table)
    {
        std::string typeStr;
        switch (pair.second.type)
        {
        case TYPE_INT:
            typeStr = "int";
            break;
        case TYPE_FLOAT:
            typeStr = "float";
            break;
        case TYPE_CHAR:
            typeStr = "char";
            break;
        default:
            typeStr = "unknown";
            break;
        }

        std::cout << std::left << std::setw(15) << pair.second.name
                  << std::setw(15) << typeStr
                  << pair.second.line_declared << "\n";
    }
    std::cout << "==============================================\n\n";
}