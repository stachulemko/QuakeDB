#ifndef DATABLOCK_H
#define DATABLOCK_H

#include <iostream>
#include <vector>
#include <string>
#include "column.h"
#include "record.h"
#include "tlv.h"
#include "typeManager.h" 
#include "binaryConverter.h"  
#include "fileOperationManager.h"  
#include "wal.h"

class DataBlock {
private:
    Wal* wal;

    std::vector<Column*> columns;

    std::vector<Record*> records;

    const int32_t MaxBlockSize = 4108;   // 4096b - 4kb plus next pointer to the block 12b = 4108b

    int32_t currentBlockSize = 0;

    int64_t blockNum = 0;

    int32_t lastColumnOffset = 0;

private:
    void LoadColumnsDefinition(std::vector<uint8_t> allBinary);
    
    void LoadRecordDefinition(std::vector<uint8_t>allBinary);
public:
    DataBlock(int64_t& blockIdBefore, Wal* wal);

    DataBlock(int64_t& blockIdBefore, Wal* wal, const std::vector<Column*>& columnsToClone);

    DataBlock(const DataBlock& other);

    ~DataBlock();

    std::vector<Record*> getRecordsRaw();

    void setCurrentBlockSize(int number);

    void addColumnPointer(Column* column) {
        columns.push_back(column);
    }

    bool ifColumnExists(std::string columnName) {
        for (int i = 0; i < columns.size(); i++) {
			if (columns[i]->getColumnName() == columnName) {
				return true;
			}
        }
        return false;
    }

    int getColumnIndex(std::string columnName) {
        for (int i = 0; i < columns.size(); i++) {
            if (columns[i]->getColumnName() == columnName) {
                return i;
            }
        }
    }

    std::vector<allVars> getColumnValues(std::string columnName) {
		int index = getColumnIndex(columnName);
        std::vector<allVars>vec;
        for (int i = 0; i < records.size(); i++) {
            vec.push_back(records[i]->getRecordDataTlv()[index]->getValue());
            /*
			if (index < records[i]->getRecordDataTlv().size()) {
				return 
			}
            */
        }
		return vec;
    }

    void addColumn(std::string columnName, int type, bool allowNull, std::vector<DataBlock>& dataBlocks);

    void addRecord(std::vector<allVars> record, std::vector<DataBlock>& dataBlocks);
  
    std::vector<uint8_t> marshall();

    void decodeBlock(std::vector<uint8_t>allBinary);

    std::vector<std::string> getColumnName();

    std::vector<std::vector<allVars>> getRecords();

    const std::vector<Column*>& getColumns() const;

    int64_t getBlockNum();

    
};

#endif
