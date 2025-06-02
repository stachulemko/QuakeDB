
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
    void LoadColumnsDefinition(std::vector<uint8_t> allBinary) {
        size_t offset = 0;
        int32_t size = 0;
        int32_t type = 0;

        while (offset + 4 <= allBinary.size()) {
            std::vector<uint8_t> typeBinary(allBinary.begin() + offset, allBinary.begin() + offset + 4);
            std::vector<uint8_t> sizeBinary(allBinary.begin() + offset + 4, allBinary.begin() + offset + 8);
            UnmarshalInt32_t(&type, &typeBinary);
            UnmarshalInt32_t(&size, &sizeBinary);
            if (type == columnTypeId) {
                Column* column = new Column("", 0, false);
                column->loadAllBytesToDecode(std::vector<uint8_t>(allBinary.begin() + offset, allBinary.begin() + offset + 8 + size+24));
                std::cout << "---------todecode-------" << std::endl;
                showBytes(std::vector<uint8_t>(allBinary.begin() + offset, allBinary.begin() + offset + 8 + size+24));
                std::cout << "---------todecode-------" << std::endl;
                column->decodeColumn();
                columns.push_back(column);
                offset += 8 + size+24;
            }
        }
    }
};

#endif 
