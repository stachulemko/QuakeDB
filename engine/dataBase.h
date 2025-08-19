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
#include "relations.h"
#include "join.h"

class Database {
private:
    Wal *wal;
    std::vector<Table*> tables;
    std::string path = "dataBaseMemoryManagment";

    //---Query needed variables(data)-----
    std::vector<std::vector<allVars>> sqlQueryBytes;
    std::vector<std::vector<allVars>> AllTableBytes;
	bool selectAcomplished = false;
	bool joinAcomplished = false;
	bool whereAcomplished = false;
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

    Database& showSqlQuery();

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
    
    void loadBtrees();

    void marshallBtree();


    bool isTableExists(std::string tableName) {
        for (auto table : tables) {
            if (table->getTableName() == tableName) {
                return true;
            }
        }
        //assert here
        return false;
    }
    bool isColumnExists(std::string tableName, std::string columnName) {
        for (auto table : tables) {
            if (table->getTableName() == tableName) {
                for (auto column : table->getColumnName()) {
                    if (column == columnName) {
                        return true;
                    }
                }
            }
        }
        //assert here
        return false;
    }
    bool isTableAndColumnExists(std::string tableName, std::string columnName) {
        if (isTableExists(tableName) && isColumnExists(tableName, columnName)) {
            return true;
        }
        else {
            return false;
        }
        //assert here
    }



    void addRelation(std::string tableName, std::string columnName, std::string foreignTableName, std::string foreignColumnName, std::string onDelete) {
        if (isTableAndColumnExists(tableName, columnName) && isTableAndColumnExists(foreignTableName, foreignColumnName)) {
            addRelation(tableName, columnName, foreignTableName, foreignColumnName, onDelete);
        }
        
    }
    

    Database& innerJoin(std::string secondTableName, std::string secondTableColumnName, std::string firstTableColumnName);



};

#endif
