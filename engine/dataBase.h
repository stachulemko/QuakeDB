
#ifndef DATABASE_H  
#define DATABASE_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "table.h"
#include <cstdint>
#include "fileOperationManager.h"

class Database {
private:
    std::vector<Table*> tables;
    std::string path="dataBaseMemoryManagment";
public:
    Database();
    ~Database();
    void commit();
    void addTable(std::string tableName);
    void addColumn(std::string tableName, std::string columnName, int dataType, bool allowNull);
    void addRecord(std::string tableName, std::vector<allVars> record);
    void clearAll();
	int getTableSize() { return tables.size(); }
    void showFile();
    void showRecords() {
		for (int i = 0; i < tables.size(); i++) {
			std::cout << "Table: " << tables[i]->getTableName() << std::endl;
			std::vector<std::string> columnNames = tables[i]->getColumnName();
			for (const auto& colName : columnNames) {
				std::cout << colName << " ";
			}
			std::cout << std::endl;
		}
		std::cout << "-------------records-------------" << std::endl;
        for (int i = 0; i < tables.size(); i++) {
			tables[i]->showRecords();
        }
    }
    std::vector<std::vector<std::string>> getTableColumnsNames();
	std::vector<std::vector<std::vector<int32_t>>> getTypeAndAllowNUllTables();
    void loadDataBase();
    std::string getPath() {
		return path;
    }
};

#endif
