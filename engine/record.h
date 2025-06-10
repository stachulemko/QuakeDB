#ifndef RECORD_H  
#define RECORD_H
#include <iostream>
#include <vector>
#include <variant>
#include "column.h"
#include "typeManager.h"
#include "tlv.h"
#include <cassert>


class Record {
private:
    int32_t recordId = 5;
    int32_t* recordSize = nullptr;
    std::vector<uint8_t>* allConnectedBytes = nullptr;
    std::vector<Tlv*> recordDataTlv;
public:
    Record() {};
    Record(std::vector<allVars> recordData, std::vector<Column*> vec);
    bool isDataTypeCorrect(std::vector<allVars> recordData, std::vector<Column*> vec);
    std::vector<uint8_t> MarshalRecord();
    void showRecord();
    std::vector<Tlv*> getRecordDataTlv();
    void clearAll();
    void loadAllConnectedBytes(std::vector<uint8_t> bytes);
    void decode();
};
#endif

