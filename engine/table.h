
#ifndef TABLE_H  
#define TABLE_H
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "column.h"
#include "fileOperationManager.h"
#include "record.h"
#include "typeManager.h"

class Table {
private:
    std::string tableName;
    std::vector<Column*> columns;
	std::vector<Record*> records;
    std::string path = "";
    

public:
    Table(std::string name, std::string path);
    ~Table();
    std::string getTableName() const;
    std::vector<std::string> getColumnName();
    std::string getTableName();
    std::vector<std::vector<int32_t>> getTypeAndAllowNUll();
    void addRecord(std::vector< allVars>record);
    void addColumn(std::string columnName, int type, bool allowNull);
    void showTable();
    void clearAll();
    std::vector<uint8_t>getColumnDefinition();
    void LoadColumnsDefinition(std::vector<uint8_t> allBinary);
};

#endif 
