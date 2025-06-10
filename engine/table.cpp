#include "table.h"
Table::Table(std::string name, std::string path) {
    tableName = name;
    this->path = path;
}

Table::~Table() {
    clearAll();
}

void Table::addColumn(std::string columnName, int type, bool allowNull) {
    Column* newColumn = new Column(columnName, type, allowNull);
    columns.push_back(newColumn);
}

void Table::showTable() {
    //std::cout << "Table Name: " << tableName << std::endl;
    //std::cout << "Columns:" << std::endl;
    for (const auto& column : columns) {
        column->showColumn();
    }
}

std::string Table::getTableName() const {
    return tableName;
}

void Table::clearAll() {
    for (auto column : columns) {
        if (column != nullptr) {
            column->clearAll();
            delete column;
        }
    }
    columns.clear();
}
std::vector<std::string> Table::getColumnName() {
    std::vector<std::string>vec;
    for (int i = 0; i < columns.size(); i++) {
        vec.push_back(columns[i]->getColumnName());
    }
    return vec;
}
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
            column->loadAllBytesToDecode(std::vector<uint8_t>(allBinary.begin() + offset, allBinary.begin() + offset + 8 + size + 24));
            //std::cout << "---------todecode-------" << std::endl;
            //showBytes(std::vector<uint8_t>(allBinary.begin() + offset, allBinary.begin() + offset + 8 + size + 24));
            //std::cout << "---------todecode-------" << std::endl;
            column->decodeColumn();
            columns.push_back(column);
            offset += 8 + size + 24;
        }
        else {
			lastColumnOffset = offset;
            //std::cout << "size : " << columns.size() << std::endl;
            break;
        }
    }
}


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


std::string Table::getTableName() {
	return tableName;
}

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
void Table::addRecord(std::vector< allVars>record) {
    Record* newRecord = new Record(record, columns);
    if (newRecord->isDataTypeCorrect(record, columns)) {
        records.push_back(newRecord);
    }
    else {
        delete newRecord;
    }
}

