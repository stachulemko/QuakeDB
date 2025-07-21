
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

    std::vector<int64_t>getBlokcNum() {
        std::vector<int64_t>nums;
        for (int i = 0; i < dataBlocks.size(); i++) {
            nums.push_back(dataBlocks[i].getBlockNum());
        }
        return nums;
    }

    //std::vector<DataBlock> getBlocks() {
	//	return dataBlocks;
    //}

    void addBtree(std::string columnName) {
        for (int i = 0; i < dataBlocks.size(); i++) {
            for (int w = 0; w < dataBlocks.size(); w++) {
                std::cout << w << " " << dataBlocks[w].getBlockNum() << std::endl;
            }
            if (dataBlocks[i].ifColumnExists(columnName)) {
                int index = dataBlocks[i].getColumnIndex(columnName);
                // Tworzymy wektor par <allVars, int>
                //std::vector<std::pair<allVars, int>> namesVector;
                // Dodajemy parê (columnName przekonwertowane na allVars, numer bloku)
                //namesVector.push_back({ columnName, static_cast<int>(dataBlocks[i].getBlockNum()) });
                BtreeManager<allVars>* bTreeManager = new BtreeManager<allVars>(3, tableName, index);
                for (int j = 0; j < dataBlocks.size(); j++) {
                    std::vector<allVars> columnValues;
					columnValues = dataBlocks[j].getColumnValues(columnName);
                    /*
                    std::cout << "--------------" << std::endl;
                    for (int i = 0; i < columnValues.size(); i++) {
                        showVariantVariable(columnValues[i]);
                    }
                    std::cout << "--------------" << std::endl;
                    */
					for (int k = 0; k < columnValues.size(); k++) {
						bTreeManager->insert(columnValues[k], dataBlocks[j].getBlockNum());
					}
                }
                bTreeManagers.push_back(bTreeManager);
                //bTreeManager->createBtree();
                break;
            }
        }
    }
    
    int getBlockNum(std::string columnName, allVars data) {
        for (int i = 0; i < dataBlocks.size(); i++) {
            if (dataBlocks[i].ifColumnExists(columnName)) {
                for (int j = 0; j < bTreeManagers.size(); j++) {
                    if (bTreeManagers[j]->getColumnIndex() == dataBlocks[i].getColumnIndex(columnName)) {
						//return bTreeManagers[j]->getBlockNum(data);
                    }
                }
                
            }
            else {
                //assert column dont exists 
            }
        }
        return 0;
    }
    
    
};

#endif 
