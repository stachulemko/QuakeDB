
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
#include "dataBlock.h"
#include "bTreeManager.h"
#include "simpleOperations.h"

class Table {
private:
    Wal *wal;
    std::string tableName;
    //std::vector<Column*> columns;
	//std::vector<Record*> records;
    std::string path = "";
    int32_t lastColumnOffset = 0;

    //-----------------------
	std::vector<DataBlock>dataBlocks;
    int64_t blockBefore = 0;

	std::vector<BtreeManager<allVars>*> bTreeManagers;

public:
    Table(std::string name, std::string path,Wal *wal);

    ~Table();

    void setBlockBefore(int32_t blockNum) {
        blockBefore = blockNum;
    }

    int getDataBlocksSize() {
        return dataBlocks.size();
    }

    std::string getTableName() const;

    std::vector<std::string> getColumnName();

    std::string getTableName();

    //std::vector<std::vector<int32_t>> getTypeAndAllowNUll();

    std::vector<Record*> getRecords();

    std::vector<std::vector<allVars>>getTableDefinition();

    void addRecord(std::vector< allVars>record);        // future update

    void addColumn(std::string columnName, int type, bool allowNull);  // future update

    void showTable();

    /*
	void showRecords() {
		for (const auto& record : records) {
			record->showRecord();
		}
	}
    */

    void showDataBlockRecord() {
        for (int i = 0; i < dataBlocks.size(); i++) {
            std::cout << "##################################<" << std::endl;
            showVariantVectorOfVector(dataBlocks[i].getRecords());
            std::cout << "##################################<" << std::endl;
        }
    }

    void clearAll();
    
    //std::vector<uint8_t>getColumnDefinition();
    
    //std::vector<uint8_t> getRecordDefinition();

    //void LoadColumnsDefinition(std::vector<uint8_t> allBinary);

    //void LoadRecordDefinition(std::vector<uint8_t>allBinary);

    void decodeBlock(std::vector<uint8_t > allBinary, int32_t& maxBlockNum);

    std::vector<uint8_t> marshalTable();

    std::vector<int64_t>getBlokcNum();

    //std::vector<DataBlock> getBlocks() {
	//	return dataBlocks;
    //}

    void addBtree(std::string columnName);
    
    std::vector<int> getBlockNum(std::string columnName, allVars data);
    std::vector<std::vector<allVars>>getRowsByBtree(std::string columnName, allVars data);

    bool ifBtreeColumnExists(std::string columName);
    
    
};

#endif 
