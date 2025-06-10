
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
    int32_t lastColumnOffset = 0;

public:
    Table(std::string name, std::string path);
    ~Table();
    std::string getTableName() const;
    std::vector<std::string> getColumnName();
    std::string getTableName();
    std::vector<std::vector<int32_t>> getTypeAndAllowNUll();
	std::vector<Record*> getRecords() {
		return records;
	}
    std::vector<std::vector<allVars>>getTableDefinition() {
        std::vector<std::vector<allVars>>tableData;
        tableData.push_back({});
        for (int i = 0; i < columns.size(); i++) {
            tableData[0].push_back(columns[i]->getColumnName());
        }
        for (int k = 0; k < records.size(); k++) {
            tableData.push_back({});
            std::vector<Tlv*> tlvvec = records[k]->getRecordDataTlv();
            for (int j = 0; j < tlvvec.size(); j++) {
                tableData[k + 1].push_back(tlvvec[j]->getValue());
            }
        }
        return tableData;
    }
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
