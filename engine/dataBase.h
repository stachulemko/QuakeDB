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
#include "wal.h"

class Database {
private:
    Wal *wal;
    std::vector<Table*> tables;
    std::string path = "dataBaseMemoryManagment";

    //---Query needed variables(data)-----
    std::vector<std::vector<allVars>> sqlQueryBytes;
    std::vector<std::vector<allVars>> AllTableBytes;
	bool selectAcomplished = false;
	std::string tmpTableName = "";
	std::vector<std::string> tmpColumnNames;

    std::string bTreePath = "bTreeData";
    //Table* selectTable = nullptr;

public:
    Database();

    ~Database();

    void commit();

    void addTable(std::string tableName);

    void addColumn(std::string tableName, std::string columnName, int dataType, bool allowNull);

    void showWal();

    void addRecord(std::string tableName, std::vector<allVars> record);

    void clearAll();

    int getTableSize();

    void showFile();

    Database &select(std::string tableName, std::vector<std::string> columnNames);

    Database &where(std::string columnName, std::string mathOperator, allVars ifValue);

    Database& showSqQuery();

    void clearQueryVariables();

    std::vector<std::vector<allVars>> getSqlQueryBytes();

    void showRecords();

    std::vector<std::vector<std::string>> getTableColumnsNames();

    std::vector<std::vector<std::vector<int32_t>>> getTypeAndAllowNUllTables();

    void loadDataBase();

    std::string getPath();

    std::vector<Table*> getTables();

    std::vector<int64_t> getTableBlockNums(std::string tableName);

    void addBtree(std::string tableName, std::string columnName);
    // test fuction only for test reason 
    std::vector<int> getBlockNum(std::string tableName, std::string columnName, allVars data);

    std::vector<int> getIndexsOfColumnNames(std::string tableName, std::vector<std::string>columnNames);

    std::vector<std::vector<allVars>> getByIndex(std::string tableName, std::string columnName, std::vector<std::string>columnNames, allVars data);

    bool ifBtreeColumnExists(std::string tableName, std::string columnName);
    
    void loadBtrees() {
        const std::string folder = bTreePath;
        try {
            for (const auto& entry : fs::directory_iterator(folder)) {
                if (!fs::is_directory(entry.status())) {
                    std::string fileName = entry.path().filename().string();
                    std::vector<uint8_t> fileBytes = readFileBytes(entry.path().string());
                    int index = 0;
                    while (index+12 < fileBytes.size()) {
                        Tlv idexTlv(fileBytes);
                        idexTlv.decode();
                        const int32_t * columnIndex = idexTlv.getInt32Value();//variantToInt32(idexTlv.getValue());
                        for (int i = 0; i < tables.size(); i++) {
                            if (tables[i]->getTableName() == fileName) {
								int32_t tmp = *columnIndex;
								addBtree(tables[i]->getTableName(), tables[i]->getColumnNameByIndex(tmp));
                            }
                        }
                    }
                }
            }
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    void marshallBtree() {
        for (int i = 0; i < tables.size(); i++) {
            if (!fs::is_directory(bTreePath)) {
				addToFileBytes("bTreePath"+tables[i]->getTableName() + ".bin", tables[i]->marshallBtrees());
            }
        }
    }
};

#endif
