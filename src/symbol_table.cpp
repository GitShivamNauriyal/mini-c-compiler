#include "../include/symbol_table.h"
#include <iostream>
#include <iomanip>

/**
 * @brief Checks if a symbol exists in the symbol table.
 */
bool SymbolTable::exists(const std::string &name)
{
    return table.find(name) != table.end();
}

/**
 * @brief Inserts a new symbol into the table.
 */
bool SymbolTable::insert(const std::string &name, DataType type, int line)
{
    if (exists(name)) return false;
    table[name] = {name, type, line};
    return true;
}

/**
 * @brief Looks up a symbol and returns its information.
 */
SymbolInfo *SymbolTable::lookup(const std::string &name)
{
    if (exists(name))
        return &table[name];
    return nullptr;
}

/**
 * @brief Formats the DataType enum into a human-readable string.
 */
std::string typeToString(DataType type)
{
    switch (type)
    {
    case TYPE_INT:
        return "int";
    case TYPE_FLOAT:
        return "float";
    case TYPE_CHAR:
        return "char";
    default:
        return "unknown";
    }
}

/**
 * @brief Prints the entire symbol table.
 */
void SymbolTable::print(std::ostream& out) const
{
    out << "\n================ SYMBOL TABLE ================\n";
    out << std::left << std::setw(15) << "Identifier"
              << std::setw(15) << "Data Type"
              << "Line Declared\n";
    out << "----------------------------------------------\n";

    for (auto const &[name, info] : table)
    {
        out << std::left << std::setw(15) << name
                  << std::setw(15) << typeToString(info.type)
                  << info.lineDeclared << "\n";
    }
    out << "==============================================\n";
}
