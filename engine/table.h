#pragma once
#ifndef TABLE_H  
#define TABLE_H
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "column.h"
#include "fileOperationManager.h"

class Table {
private:
    std::string tableName;
    std::vector<Column*> columns;
    std::string path = "";

public:
    Table(std::string name, std::string path);
    ~Table();
    std::string getTableName() const;
    void addColumn(std::string columnName, int type, bool allowNull);
    void showTable();
    void clearAll();
    std::vector<uint8_t>getColumnDefinition() {
		std::vector<uint8_t> result;
		for (const auto& column : columns) {
			std::vector<uint8_t> columnBytes = column->MarshalColumn();
			result.insert(result.end(), columnBytes.begin(), columnBytes.end());
		}
		return result;
    }
};

#endif 
