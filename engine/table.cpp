#include "table.h"
Table::Table(std::string name, std::string path, Wal* wal) {
    this->wal = wal;
    tableName = name;
    this->path = path;
    dataBlocks.push_back(DataBlock(blockBefore,wal));

}

Table::~Table() {
    clearAll();
}

void Table::addColumn(std::string columnName, int type, bool allowNull) {
	dataBlocks[dataBlocks.size()-1].addColumn(columnName, type, allowNull, dataBlocks);
    //Column* newColumn = new Column(columnName, type, allowNull);
    //wal->addToWall(newColumn->MarshalColumn(), addColumnTypeId, { newColumn->getColumnType(),newColumn->getColumnSize(),newColumn->getColumnName() },newColumn->getColumnSize());
    //columns.push_back(newColumn);
}

void Table::showTable() {
    std::cout << "Table Name: " << tableName << std::endl;
    std::cout << "Columns:" << std::endl;

    // Zamiast tego:
    // for (const auto& column : columns) {
    //     column->showColumn();
    // }

    // U¿yj tego:
    for (const auto& dataBlock : dataBlocks) {
        for (const auto& column : dataBlock.getColumns()) {  // Dodaj metodê getColumns() w DataBlock
            if (column != nullptr) {
                column->showColumn();
            }
        }
    }
}

std::string Table::getTableName() const {
    return tableName;
}

void Table::clearAll() {
    dataBlocks.clear();
    blockBefore = 0;

    // Inicjalizacja nowego bloku danych
    //dataBlocks.push_back(DataBlock(blockBefore));

    lastColumnOffset = 0;
}
std::vector<std::string> Table::getColumnName() {
    std::vector<std::string>vec;
    for (int i = 0; i < dataBlocks.size(); i++) {
		std::vector<std::string> blockColumnNames = dataBlocks[i].getColumnName();
        vec.insert(vec.end(), blockColumnNames.begin(), blockColumnNames.end());
    }
    return vec;
}
/*
std::vector<uint8_t>Table::getColumnDefinition() {
    std::vector<uint8_t> result;
    for (const auto& column : columns) {
        std::vector<uint8_t> columnBytes = column->MarshalColumn();
        result.insert(result.end(), columnBytes.begin(), columnBytes.end());
    }
    return result;
}

std::vector<uint8_t> Table::getRecordDefinition() {
	std::vector<uint8_t> result;
	for (const auto& record : records) {
		std::vector<uint8_t> recordBytes = record->MarshalRecord();
		result.insert(result.end(), recordBytes.begin(), recordBytes.end());
	}
	return result;
}

void Table::LoadColumnsDefinition(std::vector<uint8_t> allBinary) {
    size_t offset = 0;
    int32_t size = 0;
    int32_t type = 0;

    while (offset + 4 <= allBinary.size()) {
        std::vector<uint8_t> typeBinary(allBinary.begin() + offset, allBinary.begin() + offset + 4);
        std::vector<uint8_t> sizeBinary(allBinary.begin() + offset + 4, allBinary.begin() + offset + 8);
        UnmarshalInt32_t(&type, &typeBinary);
        UnmarshalInt32_t(&size, &sizeBinary);
        if (type == columnTypeId) {
            Column* column = new Column("", 0, false);
            column->loadAllBytesToDecode(std::vector<uint8_t>(allBinary.begin() + offset, allBinary.begin() + offset + 8 + size));
            //std::cout << "---------todecode-------" << std::endl;
            //showBytes(std::vector<uint8_t>(allBinary.begin() + offset, allBinary.begin() + offset + 8 + size + 24));
            //std::cout << "---------todecode-------" << std::endl;
            column->decodeColumn();
            columns.push_back(column);
            //column->showColumn();
            offset += 8 + size;
        }
        else {
			lastColumnOffset = offset;
            //std::cout << "size : " << columns.size() << std::endl;
            break;
        }
    }
}
*/
std::vector<uint8_t> Table::marshalTable() {
	std::vector<uint8_t> result;
    for (auto block : dataBlocks) {
		std::vector<uint8_t>dataBytes = block.marshall();
		result.insert(result.end(), dataBytes.begin(), dataBytes.end());
    }
    return result;
}

/*
void Table::LoadRecordDefinition(std::vector<uint8_t>allBinary) {
    size_t offset = lastColumnOffset;
    int32_t size = 0;
    int32_t type = 0;
    while (offset + 4 <= allBinary.size()) {
		std::vector<uint8_t> typeBinary(allBinary.begin() + offset, allBinary.begin() + offset + 4);
        std::vector<uint8_t> sizeBinary(allBinary.begin() + offset + 4, allBinary.begin() + offset + 8);
        UnmarshalInt32_t(&size, &sizeBinary);
		UnmarshalInt32_t(&type, &typeBinary);
        if(type == recordTypeId) {
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
}
*/

void Table::decodeBlock(std::vector<uint8_t>allBinary,int32_t &maxBlockNum) {
    int32_t offset = 0;

    std::cout << "--------"<<std::endl;
	showBytes(allBinary);
    std::cout << "--------" << std::endl;

    while (offset <allBinary.size()) {
        if (offset + 4 <= allBinary.size()) {
            std::vector<uint8_t> typeBinary(allBinary.begin() + offset, allBinary.begin() + offset + 4);
            int32_t type = 0;
            UnmarshalInt32_t(&type, &typeBinary);
            if (type == blockId) {
				std::vector<uint8_t> sizeBinary(allBinary.begin() + offset + 4, allBinary.begin() + offset + 8);

                std::cout << "======" << std::endl;
				showBytes(sizeBinary);
                std::cout << "======" << std::endl;

				int32_t size = 0;
				UnmarshalInt32_t(&size, &sizeBinary);
				if (offset + 8 + size <= allBinary.size()) {
					std::vector<uint8_t> blockNumBytes(allBinary.begin() + offset + 8, allBinary.begin() + offset + 8 + size);
                    std::cout << "=============" << std::endl;
                    showBytes(blockNumBytes);
                    std::cout << "=============" << std::endl;
                    int64_t blockNum = 0;
					UnmarshalInt64_t(&blockNum, &blockNumBytes);
                    //==================================
                    int32_t lenghtType = 0;
                    std::vector<uint8_t> blockIdBytes(allBinary.begin() + offset+16, allBinary.begin() + offset + 20);
                    std::cout << "=============" << std::endl;
                    showBytes(blockIdBytes);
                    std::cout << "=============" << std::endl;
                    UnmarshalInt32_t(&lenghtType, &blockIdBytes);
                    if (lenghtType == int32_tId) {
                        int32_t lenghtNum = 0;
                        std::vector<uint8_t> LenghtIdBytes(allBinary.begin() + offset+ 20, allBinary.begin()+ offset + 24);
                        std::cout << "=============" << std::endl;
                        showBytes(LenghtIdBytes);
                        std::cout << "=============" << std::endl;
                        UnmarshalInt32_t(&lenghtNum, &LenghtIdBytes);
                        if (lenghtNum == 4) {
                            int32_t valueLenght = 0;
                            std::vector<uint8_t> LenghtlockBytes(allBinary.begin() + offset + 24, allBinary.begin() +24+ offset+ lenghtNum);
                            UnmarshalInt32_t(&valueLenght, &LenghtlockBytes);
                            std::vector<uint8_t>data(allBinary.begin() + offset+ lenghtNum+ 24, allBinary.begin() + offset + lenghtNum + 24 + valueLenght);
							dataBlocks[dataBlocks.size() - 1].decodeBlock(data);
							//DataBlock newBlock(blockNum);
                            //maxBlockNum= blockNum;
							//newBlock.decodeBlock(data);
                            std::cout << "columnNamesColumnNamesColumnNames" << std::endl;
                            /*
                            for(int i=0;i<newBlock.getColumnName().size();i++) {
								std::cout << "Column Name: " << newBlock.getColumnName()[i] << std::endl;
							}		
                            */
                            std::cout << "columnNamesColumnNamesColumnNames" << std::endl;

                            std::cout << "RecordRecordRecordRecordRecord" << std::endl;
                            //showVariantVectorOfVector(newBlock.getRecords());
                            std::cout << "RecordRecordRecordRecordRecord" << std::endl;
                            //dataBlocks.push_back(newBlock);
							offset += 28 + valueLenght;
                        }
                        //DataBlock newBlock(blockData);
                        //dataBlocks.push_back(newBlock);
                        //offset += 8 + size;
                    }
				}
				else {
					std::cerr << "Error: Insufficient data for block decoding" << std::endl;
					return;
				}
			}
            else {
                std::cerr << "Error: Invalid block type" << std::endl;
                return;
            }
        }

    }
}



std::string Table::getTableName() {
	return tableName;
}

/*
std::vector<std::vector<int32_t>> Table::getTypeAndAllowNUll() {
    std::vector<std::vector<int32_t>>allvec;
    for (int i = 0; i < columns.size(); i++) {
		if (columns[i]->isAllowNull()) {
			allvec.push_back({ columns[i]->getColumnType(), 1 });
		}
		else {
			allvec.push_back({ columns[i]->getColumnType(), 0 });
		}
    }
	return allvec;
}

*/

void Table::addRecord(std::vector< allVars>record) {
    dataBlocks[dataBlocks.size() - 1].addRecord(record, dataBlocks);
    /*
    Record* newRecord = new Record(record, columns);
    if (newRecord->isDataTypeCorrect(record, columns)) {
        records.push_back(newRecord);
        wal->addToWall(newRecord->MarshalRecord(), insertTypeId, newRecord->getRecordData(), newRecord->getRecordSize());
    }
    else {
        delete newRecord;
    }
    */
}

std::vector<Record*> Table::getRecords() {
	std::vector<Record*> allRecords;
    for (int i = 0; i < dataBlocks.size(); i++) {
        std::vector<Record*> allRecordTmp = dataBlocks[i].getRecordsRaw();
        allRecords.insert(allRecords.end(), allRecordTmp.begin(), allRecordTmp.end());
    }
    return allRecords;
}

std::vector<std::vector<allVars>> Table::getTableDefinition() {
    std::cout << "dataBlockSize()"<<dataBlocks.size() << std::endl;
    for (int i = 0; i < dataBlocks.size(); i++) {
        //std::cout << "IIIIIIIIIIIIIIIIIIIIIII" << std::endl;
        //std::vector<std::string> tmpColumnName=dataBlocks[i].getColumnName();
        //for (int j = 0; j < tmpColumnName.size(); j++) {
        //    std::cout << tmpColumnName[j] << std::endl;
        //}
        //std::cout << "IIIIIIIIIIIIIIIIIIIIIII" << std::endl;
    }
    std::vector<std::vector<allVars>>tableData;
    tableData.push_back({});
    for (int i = 0; i < dataBlocks.size(); i++) {
		std::vector<std::string> columns = dataBlocks[i].getColumnName();
		tableData[0].insert(tableData[0].end(), columns.begin(), columns.end());
    }
    for (int i = 0; i < dataBlocks.size(); i++) {
		std::vector<std::vector<allVars>> records = dataBlocks[i].getRecords();
		for (const auto& record : records) {
			tableData.push_back(record);
		}
    }
    return tableData;
}

