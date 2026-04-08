#include "../include/symbol_table.h"
#include <iostream>
#include <iomanip>

/**
 * @brief Checks if a symbol exists in the symbol table.
 * 
 * @param name The identifier to look up.
 * @return bool True if the identifier exists, false otherwise.
 */
bool SymbolTable::exists(const std::string &name)
{
    return table.find(name) != table.end();
}

/**
 * @brief Inserts a new symbol into the table.
 * 
 * @param name The identifier name.
 * @param type The data type (INT, FLOAT, CHAR).
 * @param line The line number where it was declared.
 * @return bool True if insertion succeeded, false if already exists.
 */
bool SymbolTable::insert(const std::string &name, DataType type, int line)
{
    if (exists(name)) return false;
    table[name] = {name, type, line};
    return true;
}

/**
 * @brief Looks up a symbol and returns its information.
 * 
 * @param name The identifier to look up.
 * @return SymbolInfo* Pointer to the symbol info if found, nullptr otherwise.
 */
SymbolInfo *SymbolTable::lookup(const std::string &name)
{
    if (exists(name))
        return &table[name];
    return nullptr;
}

/**
 * @brief Formats the DataType enum into a human-readable string.
 * 
 * @param type The DataType enum value.
 * @return std::string String representation ("int", "float", etc.).
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
 * @brief Prints the entire symbol table in a formatted grid.
 */
void SymbolTable::print() const
{
    std::cout << "\n================ SYMBOL TABLE ================\n";
    std::cout << std::left << std::setw(15) << "Identifier"
              << std::setw(15) << "Data Type"
              << "Line Declared\n";
    std::cout << "----------------------------------------------\n";

    for (auto const &[name, info] : table)
    {
        std::cout << std::left << std::setw(15) << name
                  << std::setw(15) << typeToString(info.type)
                  << info.lineDeclared << "\n";
    }
    std::cout << "==============================================\n";
}
