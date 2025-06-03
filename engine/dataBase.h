
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
    void clearAll();
    void showFile();
    std::vector<std::vector<std::string>> getTableColumnsNames();
	std::vector<std::vector<std::vector<int32_t>>> getTypeAndAllowNUllTables();
    void loadDataBase();
    std::string getPath() {
		return path;
    }
};

#endif
