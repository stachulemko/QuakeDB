#include "table.h"
Table::Table(std::string name, std::string path) {
    tableName = name;
    this->path = path;
}

Table::~Table() {
    clearAll();
}

void Table::addColumn(std::string columnName, int type, bool allowNull) {
    Column* newColumn = new Column(columnName, type, allowNull);
    columns.push_back(newColumn);
}

void Table::showTable() {
    std::cout << "Table Name: " << tableName << std::endl;
    std::cout << "Columns:" << std::endl;
    for (const auto& column : columns) {
        column->showColumn();
    }
}

std::string Table::getTableName() const {
    return tableName;
}

void Table::clearAll() {
    for (auto column : columns) {
        if (column != nullptr) {
            column->clearAll();
            delete column;
        }
    }
    columns.clear();
}
