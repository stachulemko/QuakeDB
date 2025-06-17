#include "column.h"

Column::Column(std::string name, int32_t columnType, bool allowNUll) {
    clearAll();
    columnName = new Tlv(name);
    columnTypeTlv = new Tlv(columnType);
    allowNull = new Tlv(allowNUll ? 1 : 0);
    columnSize = new int32_t(columnName->getTlvSize() + columnTypeTlv->getTlvSize() + allowNull->getTlvSize());
    /*
	if (name.empty()) {
        assert(false && "record data string empty"); 
	}
    else {
        
    }
    */
}

Column::~Column() {
    clearAll();
}

void Column::SetColumn(std::string name, int32_t columnType, bool allowNUll) {
    clearAll();
    columnName = new Tlv(name);
    columnTypeTlv = new Tlv(columnType);
    allowNull = new Tlv(allowNUll ? 1 : 0);
    columnSize = new int32_t(columnName->getTlvSize() + columnTypeTlv->getTlvSize() + allowNull->getTlvSize());
}

int32_t Column::getColumnSize() {
    int32_t sum = columnName->getTlvSize() + columnTypeTlv->getTlvSize() + allowNull->getTlvSize() ;
    return sum;
}

std::string Column::getColumnName() {
    if (columnName != nullptr) {
        return *(columnName->getStringValue());
    }
    else {
        std::cerr << "Column name is not set." << std::endl;
        return "";
    }
}

int32_t Column::getColumnType() {
    if (columnTypeTlv != nullptr) {
        return *(columnTypeTlv->getInt32Value());
    }
    else {
        std::cerr << "Column type is not set." << std::endl;
        return -1;
    }
}

bool Column::isAllowNull() {
    if (allowNull != nullptr) {
        return *(allowNull->getInt32Value()) == 1;
    }
    else {
        std::cerr << "Allow null is not set." << std::endl;
        return false;
    }
}

std::vector<uint8_t> Column::MarshalColumn() {
    std::vector<uint8_t> result;
    std::vector<uint8_t>* columnTypeBytes = marshalInt32_t(columnTypeId);
    std::vector<uint8_t> nameBytes = columnName->marshalTlv();
    std::vector<uint8_t> typeBytes = columnTypeTlv->marshalTlv();
    std::vector<uint8_t> allowNullBytes = allowNull->marshalTlv();

    std::vector<uint8_t>* lengthColumn = marshalInt32_t(*columnSize);

    result.insert(result.end(), columnTypeBytes->begin(), columnTypeBytes->end());
    result.insert(result.end(), lengthColumn->begin(), lengthColumn->end());
    result.insert(result.end(), nameBytes.begin(), nameBytes.end());
    result.insert(result.end(), typeBytes.begin(), typeBytes.end());
    result.insert(result.end(), allowNullBytes.begin(), allowNullBytes.end());

    delete columnTypeBytes;
    delete lengthColumn;

    return result;
}

void Column::loadAllBytesToDecode(std::vector<uint8_t> bytes) {
    clearAll();
    allConnectedBytes = new std::vector<uint8_t>(bytes);
}

void Column::decodeColumn() {
    if (!allConnectedBytes || allConnectedBytes->size() < 8) {
        std::cerr << "Invalid or empty data for column decoding" << std::endl;
        return;
    }

    std::vector<uint8_t> typeBytes(allConnectedBytes->begin(), allConnectedBytes->begin() + 4);
    int32_t type = 0;
    UnmarshalInt32_t(&type, &typeBytes);

    if (type == columnTypeId) {
        std::vector<uint8_t> sizeBytes(allConnectedBytes->begin() + 4, allConnectedBytes->begin() + 8);
        columnSize = new int32_t(0);
        UnmarshalInt32_t(columnSize, &sizeBytes);

        size_t offset = 8;

        if (offset < allConnectedBytes->size()) {
            std::vector<uint8_t> nameTlvBytes;
            std::vector<uint8_t> nameTypeBytes(allConnectedBytes->begin() + offset, allConnectedBytes->begin() + offset + 4);
            int32_t nameType = 0;
            UnmarshalInt32_t(&nameType, &nameTypeBytes);

            std::vector<uint8_t> nameLengthBytes(allConnectedBytes->begin() + offset + 4, allConnectedBytes->begin() + offset + 8);
            int32_t nameLength = 0;
            UnmarshalInt32_t(&nameLength, &nameLengthBytes);
            size_t nameTlvSize = 8 + nameLength;
            if (offset + nameTlvSize <= allConnectedBytes->size()) {
                nameTlvBytes.assign(allConnectedBytes->begin() + offset, allConnectedBytes->begin() + offset + nameTlvSize);
                columnName = new Tlv(nameTlvBytes);
                columnName->decode();
                offset += nameTlvSize;
            }

            if (offset < allConnectedBytes->size()) {
                std::vector<uint8_t> typeTlvBytes;
                std::vector<uint8_t> typeTypeBytes(allConnectedBytes->begin() + offset, allConnectedBytes->begin() + offset + 4);
                int32_t typeType = 0;
                UnmarshalInt32_t(&typeType, &typeTypeBytes);

                std::vector<uint8_t> typeLengthBytes(allConnectedBytes->begin() + offset + 4, allConnectedBytes->begin() + offset + 8);
                int32_t typeLength = 0;
                UnmarshalInt32_t(&typeLength, &typeLengthBytes);

                size_t typeTlvSize = 8 + typeLength;
                if (offset + typeTlvSize <= allConnectedBytes->size()) {
                    typeTlvBytes.assign(allConnectedBytes->begin() + offset, allConnectedBytes->begin() + offset + typeTlvSize);
                    columnTypeTlv = new Tlv(typeTlvBytes);
                    columnTypeTlv->decode();
                    offset += typeTlvSize;
                }

                if (offset < allConnectedBytes->size()) {
                    std::vector<uint8_t> allowNullBytes;
                    std::vector<uint8_t> nullTypeBytes(allConnectedBytes->begin() + offset, allConnectedBytes->begin() + offset + 4);
                    int32_t nullType = 0;
                    UnmarshalInt32_t(&nullType, &nullTypeBytes);

                    std::vector<uint8_t> nullLengthBytes(allConnectedBytes->begin() + offset + 4, allConnectedBytes->begin() + offset + 8);
                    int32_t nullLength = 0;
                    UnmarshalInt32_t(&nullLength, &nullLengthBytes);

                    size_t nullTlvSize = 8 + nullLength;
                    if (offset + nullTlvSize <= allConnectedBytes->size()) {
                        allowNullBytes.assign(allConnectedBytes->begin() + offset, allConnectedBytes->begin() + offset + nullTlvSize);
                        allowNull = new Tlv(allowNullBytes);
                        allowNull->decode();
                    }
                }
                else {
                    std::cerr << "Insufficient data for allowNull TLV decoding" << std::endl;
                }
            }
            else {
                std::cerr << "Insufficient data for column type TLV decoding" << std::endl;
            }
        }
        else {
            std::cerr << "Insufficient data for column name decoding" << std::endl;
        }
    }
    else {
        std::cerr << "Invalid column type ID: " << type << std::endl;
    }
	//std::cout << "-------------Column decoded------------ "<< std::endl;
	//showColumn();
    //std::cout << "-------------Column decoded------------ " << std::endl;
}

void Column::clearAll() {
    delete columnSize;
    delete columnName;
    delete columnTypeTlv;
    delete allowNull;
    delete allConnectedBytes;

    columnSize = nullptr;
    columnName = nullptr;
    columnTypeTlv = nullptr;
    allowNull = nullptr;
    allConnectedBytes = nullptr;
}

void Column::showColumn() {
    std::cout << "--- Column Details ---" << std::endl;
    std::cout << "Column Type ID: " << columnTypeId << std::endl;

    if (columnSize != nullptr) {
        std::cout << "Column Size: " << *columnSize << std::endl;
    }
    else {
        std::cout << "Column Size: not set" << std::endl;
    }
    std::cout << "Name: " << getColumnName() << std::endl;
    std::cout << "Data Type: " << getColumnType() << std::endl;
    std::cout << "Allow NULL: " << (isAllowNull() ? "Yes" : "No") << std::endl;

    std::cout << "--------------------" << std::endl;
}
