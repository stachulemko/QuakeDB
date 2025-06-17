
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
#include "wal.h"

class Table {
private:
    Wal *wal;
    std::string tableName;
    std::vector<Column*> columns;
	std::vector<Record*> records;
    std::string path = "";
    int32_t lastColumnOffset = 0;

public:
    Table(std::string name, std::string path,Wal *wal);

    ~Table();

    std::string getTableName() const;

    std::vector<std::string> getColumnName();

    std::string getTableName();

    std::vector<std::vector<int32_t>> getTypeAndAllowNUll();

    std::vector<Record*> getRecords();

    std::vector<std::vector<allVars>>getTableDefinition();

    void addRecord(std::vector< allVars>record);

    void addColumn(std::string columnName, int type, bool allowNull);

    void showTable();

	void showRecords() {
		for (const auto& record : records) {
			record->showRecord();
		}
	}

    void clearAll();

    std::vector<uint8_t>getColumnDefinition();

    std::vector<uint8_t> getRecordDefinition();

    void LoadColumnsDefinition(std::vector<uint8_t> allBinary);

    void LoadRecordDefinition(std::vector<uint8_t>allBinary);


};

#endif 
