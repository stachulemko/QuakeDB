
#ifndef COLUMN_H  
#define COLUMN_H
#include <iostream>
#include "vector"
#include <string>
#include "binaryConverter.h"
#include "tlv.h"

class Column {
private:
    const int32_t columnTypeId = 4;
    int32_t* columnSize = nullptr;
    Tlv* columnName = nullptr;
    Tlv* columnTypeTlv = nullptr;
    Tlv* allowNull = nullptr;
    std::vector<uint8_t>* allConnectedBytes = nullptr;

public:
    Column(std::string name, int32_t columnType, bool allowNUll);
    ~Column();
    void SetColumn(std::string name, int32_t columnType, bool allowNUll);

    int32_t getColumnSize();
    std::string getColumnName();

    int32_t getColumnType();
    bool isAllowNull();
    std::vector<uint8_t> MarshalColumn();

    void loadAllBytesToDecode(std::vector<uint8_t> bytes);
    void decodeColumn();

    void clearAll();
    void showColumn();
};

#endif
