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

	std::vector<Table*> getTables() {
		return tables;
	}

    std::vector<int64_t> getTableBlockNums(std::string tableName) {
        std::vector<int64_t> vecNums;
        for (int i = 0; i < tables.size(); i++) {
            if (tables[i]->getTableName() == tableName) {
                vecNums = tables[i]->getBlokcNum();
            }
        }
        return vecNums; // Dodana instrukcja return
    }
    void addBtree(std::string tableName,std::string columnName) {
        for (int i = 0; i < tables.size(); i++) {
            if (tables[i]->getTableName() == tableName) {
				tables[i]->addBtree(columnName);
            }
            else {
                //assert not exist table - later !! 
            }
        }
    }
    // test fuction only for test reason 
    std::vector<int> getBlockNum(std::string tableName, std::string columnName,allVars data) {
        for (int i = 0; i < tables.size(); i++) {
            if (tables[i]->getTableName() == tableName) {
				return tables[i]->getBlockNum(columnName, data);
            }
            else {
                //assert not exist table - later !! 
            }
        }
    }

    
    std::vector<int> getIndexsOfColumnNames(std::string tableName,std::vector<std::string>columnNames) {
        std::vector<int>indexes;
        for (int i = 0; i < tables.size(); i++) {
            if (tables[i]->getTableName() == tableName) {
                //std::vector<std::vector<allVars>> vec = tables[i]->getRowsByBtree(columnName, data);
                for (int j = 0; j < columnNames.size(); j++) {
                    for (int k = 0; k < tables[i]->getColumnName().size(); k++) {
                        if (columnNames[j] == tables[i]->getColumnName()[k]) {
							indexes.push_back(k);
						}
                    }
                }
                break;
            }
        }
        return indexes;
    }
    std::vector<std::vector<allVars>> getByIndex(std::string tableName, std::string columnName,std::vector<std::string>columnNames,allVars data) {
		std::vector<std::vector<allVars>> AllTableBytes;
        for (int i = 0; i < tables.size(); i++) {
            if (tables[i]->getTableName() == tableName) {
				std::vector<std::vector<allVars>> vec = tables[i]->getRowsByBtree(columnName, data);
                for (int j = 0; j < columnNames.size(); j++) {
                    for (int k = 0; k < tables[i]->getColumnName().size(); k++) {
						if (columnNames[j] != tables[i]->getColumnName()[k]) {
							//make assert !! 
						}
                    }
                }
                std::vector<allVars>names;
                for (int j = 0; j < columnNames.size(); j++) {
					names.push_back(std::string(columnNames[j]));
                }
                AllTableBytes.push_back(names);
				std::vector<int>indexes = getIndexsOfColumnNames(tableName, columnNames);
                for (int j = 0; j < vec.size(); j++) {
					std::vector<allVars> row;
                    for (int k = 0; k < vec[j].size(); k++) {
                        for (int w = 0; w < indexes.size(); w++) {
                            if (k == indexes[w]) {
								row.push_back(vec[j][k]);
                            }
                        }
                    }
					AllTableBytes.push_back(row);
                    
                }
				//AllTableBytes.insert(AllTableBytes.end(), vec.begin(), vec.end());
				return AllTableBytes; 
            }
        }
    }
    bool ifBtreeColumnExists(std::string tableName,std::string columnName) {
        for (int i = 0; i < tables.size(); i++) {
			if (tables[i]->getTableName() == tableName) {
				return tables[i]->ifBtreeColumnExists(columnName);
			}
			else {
				//assert not exist table - later !! 
			}
        }
    }
    

};

#endif
