#ifndef DATABASE_H  
#define DATABASE_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "table.h"
#include <cstdint>
#include "fileOperationManager.h"
#include "select.h"

class Database {
private:
    std::vector<Table*> tables;
    std::string path = "dataBaseMemoryManagment";
public:
    Database();

    ~Database();

    void commit();

    void addTable(std::string tableName);

    void addColumn(std::string tableName, std::string columnName, int dataType, bool allowNull);

    void addRecord(std::string tableName, std::vector<allVars> record);

    void clearAll();

    int getTableSize();

    void showFile();

    void select(std::string tableName, std::vector<std::string> columnNames);

    void showRecords();

    std::vector<std::vector<std::string>> getTableColumnsNames();

    std::vector<std::vector<std::vector<int32_t>>> getTypeAndAllowNUllTables();

    void loadDataBase();

    std::string getPath();

	std::vector<Table*> getTables() {
		return tables;
	}
};

#endif
