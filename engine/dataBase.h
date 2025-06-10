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
#include "where.h"

class Database {
private:
    std::vector<Table*> tables;
    std::string path = "dataBaseMemoryManagment";

    //---dataBase fuctions -----
    std::vector<std::vector<allVars>> sqlQueryBytes;
    std::vector<std::vector<allVars>> AllTableBytes;
	bool selectAcomplished = false;
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

    Database &select(std::string tableName, std::vector<std::string> columnNames);

    Database &where(std::string columnName, std::string mathOperator, allVars ifValue);

    Database& showSqQuery();

    void clearQueryVariables() {
        sqlQueryBytes.clear();
		selectAcomplished = false;
    }

    std::vector<std::vector<allVars>> getSqlQueryBytes() {
		std::cout << "sqlQueryBytes size: " << sqlQueryBytes.size() << std::endl;
        std::vector<std::vector<allVars>>tmpSqlQueryBytes = sqlQueryBytes;
		clearQueryVariables();
        return tmpSqlQueryBytes;

    }

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
