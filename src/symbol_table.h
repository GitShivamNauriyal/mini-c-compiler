#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <string>
#include <unordered_map>

// Enum to represent the supported data types in Mini C
enum DataType
{
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_UNKNOWN
};

struct SymbolInfo
{
    std::string name;
    DataType type;
    int line_declared;
};

class SymbolTable
{
private:
    std::unordered_map<std::string, SymbolInfo> table;

public:
    bool insert(const std::string &name, DataType type, int line);

    SymbolInfo *lookup(const std::string &name);

    bool exists(const std::string &name);

    void printTable();
};

#endif