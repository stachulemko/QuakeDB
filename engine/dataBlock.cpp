#include "dataBlock.h"


void DataBlock::LoadColumnsDefinition(std::vector<uint8_t> allBinary) {
    size_t offset = 0;
    int32_t size = 0;
    int32_t type = 0;

    while (offset + 4 <= allBinary.size()) {
        std::vector<uint8_t> typeBinary(allBinary.begin() + offset, allBinary.begin() + offset + 4);
        std::vector<uint8_t> sizeBinary(allBinary.begin() + offset + 4, allBinary.begin() + offset + 8);
        UnmarshalInt32_t(&type, &typeBinary);
        UnmarshalInt32_t(&size, &sizeBinary);
        if (type == columnTypeId) {
            Column* column = new Column();
            column->loadAllBytesToDecode(std::vector<uint8_t>(allBinary.begin() + offset, allBinary.begin() + offset + 8 + size));
            //std::cout << "---------todecode-------" << std::endl;
            //showBytes(std::vector<uint8_t>(allBinary.begin() + offset, allBinary.begin() + offset + 8 + size + 24));
            //std::cout << "---------todecode-------" << std::endl;
            column->decodeColumn();
            columns.push_back(column);
            //column->showColumn();
            offset += 8 + size;
            column->showColumn();
        }
        else {
            lastColumnOffset = offset;
            //std::cout << "size : " << columns.size() << std::endl;
            break;
        }
    }
    lastColumnOffset = offset;
}

void DataBlock::LoadRecordDefinition(std::vector<uint8_t>allBinary) {
    size_t offset = lastColumnOffset;
    int32_t size = 0;
    int32_t type = 0;
    while (offset + 4 <= allBinary.size()) {
        std::vector<uint8_t> typeBinary(allBinary.begin() + offset, allBinary.begin() + offset + 4);
        std::vector<uint8_t> sizeBinary(allBinary.begin() + offset + 4, allBinary.begin() + offset + 8);
        UnmarshalInt32_t(&size, &sizeBinary);
        UnmarshalInt32_t(&type, &typeBinary);
        if (type == recordTypeId) {
            Record record;
            record.loadAllConnectedBytes(std::vector<uint8_t>(allBinary.begin() + offset, allBinary.begin() + offset + size));
            record.decode();
            records.push_back(new Record(record));
            offset += size;
        }
        else {
            break;
        }
    }
    currentBlockSize = offset;
}

DataBlock::DataBlock(int64_t& blockIdBefore, Wal* wal) {
    this->wal = wal;
    blockNum = blockIdBefore + 1;
    blockIdBefore++;
}

DataBlock::DataBlock(int64_t& blockIdBefore, Wal* wal, const std::vector<Column*>& columnsToClone) {
    this->wal = wal;
    blockNum = blockIdBefore + 1;
    blockIdBefore++;

    // Kopiowanie kolumn
    for (auto& col : columnsToClone) {
        std::cout <<"col type : " << col->getColumnType() << std::endl;
        columns.push_back(new Column(col->getColumnName(), col->getColumnType(), col->isAllowNull()));
    }
    std::cout <<"columns size() : " << columns.size() << std::endl;
}


DataBlock::~DataBlock() {
    for (auto column : columns) {
        delete column;
    }
    for (auto record : records) {
        delete record;
    }
}



std::vector<Record*> DataBlock::getRecordsRaw() {
    return records;
}
void DataBlock::setCurrentBlockSize(int number) {
    currentBlockSize = number;
}

void DataBlock::addColumn(std::string columnName, int type, bool allowNull, std::vector<DataBlock>& dataBlocks) {
    Column* newColumn = new Column(columnName, type, allowNull);
    if (currentBlockSize + newColumn->getColumnSize() + 8 < MaxBlockSize) {
        currentBlockSize += newColumn->getColumnSize() + 8;
        columns.push_back(newColumn);
        wal->addToWall(newColumn->MarshalColumn(), addColumnTypeId, { newColumn->getColumnType(),newColumn->getColumnSize(),newColumn->getColumnName() }, newColumn->getColumnSize());
    }
    else {
        //dataBlocks.push_back(DataBlock(blockNum, wal));
        dataBlocks.emplace_back(DataBlock(blockNum, wal));
        dataBlocks[dataBlocks.size() - 1].addColumn(columnName, type, allowNull, dataBlocks);
        setCurrentBlockSize(newColumn->getColumnSize() + 8);
    }

}

void DataBlock::addRecord(std::vector<allVars> record, std::vector<DataBlock>& dataBlocks) {
    Record* newRecord = new Record(record, columns);
    if (newRecord->isDataTypeCorrect(record, columns) && currentBlockSize + newRecord->getRecordSize() < MaxBlockSize) {
        records.push_back(newRecord);
        currentBlockSize += newRecord->getRecordSize();
        wal->addToWall(newRecord->MarshalRecord(), insertTypeId, newRecord->getRecordData(), newRecord->getRecordSize());
    }
    else {
        //dataBlocks.push_back(DataBlock(blockNum, wal,columns));
        dataBlocks.emplace_back(blockNum, wal, columns);
        dataBlocks[dataBlocks.size() - 1].addRecord(record,dataBlocks);
        //dataBlocks[dataBlocks.size() - 1].addRecord(record, dataBlocks);
        setCurrentBlockSize(newRecord->getRecordSize());
    }
}

std::vector<uint8_t> DataBlock::marshall() {
    std::vector<uint8_t> allBytes;
    std::vector<uint8_t>* blockIdBytes = marshalInt32_t(blockId);
    std::vector<uint8_t>* blockSizeBytes = marshalInt32_t(8);
    std::vector<uint8_t>* value = marshalInt64_t(blockNum);

    std::vector<uint8_t>* type = marshalInt32_t(int32_tId);
    std::vector<uint8_t>* lenghtType = marshalInt32_t(4);
    std::vector<uint8_t>* valueType = marshalInt32_t(currentBlockSize);

    allBytes.insert(allBytes.end(), blockIdBytes->begin(), blockIdBytes->end());
    allBytes.insert(allBytes.end(), blockSizeBytes->begin(), blockSizeBytes->end());
    std::cout << "000000000000" << std::endl;
    showBytes(allBytes);
    std::cout << "000000000000" << std::endl;
    allBytes.insert(allBytes.end(), value->begin(), value->end());
    allBytes.insert(allBytes.end(), type->begin(), type->end());
    allBytes.insert(allBytes.end(), lenghtType->begin(), lenghtType->end());
    allBytes.insert(allBytes.end(), valueType->begin(), valueType->end());

    for (const auto& column : columns) {
        std::vector<uint8_t> columnBytes = column->MarshalColumn();
        allBytes.insert(allBytes.end(), columnBytes.begin(), columnBytes.end());
    }
    for (const auto& record : records) {
        std::vector<uint8_t> recordBytes = record->MarshalRecord();
        allBytes.insert(allBytes.end(), recordBytes.begin(), recordBytes.end());
    }

    delete blockIdBytes;
    delete blockSizeBytes;
    return allBytes;
}
void DataBlock::decodeBlock(std::vector<uint8_t>allBinary) {
    LoadColumnsDefinition(allBinary);
    LoadRecordDefinition(allBinary);
}
std::vector<std::string> DataBlock::getColumnName() {
    std::vector<std::string> columnNames;
    for (const auto& column : columns) {
        if (column != nullptr) {
            columnNames.push_back(column->getColumnName());
        }
    }
    return columnNames;  // DODANE: Brakuj¹cy return
}
std::vector<std::vector<allVars>> DataBlock::getRecords() {
    std::vector<std::vector<allVars>>tableData;
    for (int k = 0; k < records.size(); k++) {
        tableData.push_back({});
        std::vector<Tlv*> tlvvec = records[k]->getRecordDataTlv();
        for (int j = 0; j < tlvvec.size(); j++) {
            tableData[k].push_back(tlvvec[j]->getValue());
        }
    }
    return tableData;
}
const std::vector<Column*>& DataBlock::getColumns() const {
    return columns;
}
int64_t DataBlock::getBlockNum() {
    return blockNum;
}