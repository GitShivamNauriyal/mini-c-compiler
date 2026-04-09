#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <map>
#include <vector>
#include <iostream>

enum DataType
{
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_UNKNOWN
};

std::string typeToString(DataType type);

struct SymbolInfo
{
    std::string name;
    DataType type;
    int lineDeclared;
};

class SymbolTable
{
private:
    std::map<std::string, SymbolInfo> table;

public:
    bool exists(const std::string &name);
    bool insert(const std::string &name, DataType type, int line);
    SymbolInfo *lookup(const std::string &name);
    void print(std::ostream& out = std::cout) const;
};

#endif
