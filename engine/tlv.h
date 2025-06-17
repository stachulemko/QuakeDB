#ifndef TLV_H   
#define TLV_H
#include <iostream>
#include <string>
#include <bitset>
#include <cstdint>
#include <vector>
#include "binaryConverter.h"
#include "typeManager.h"

class Tlv {
private:
    //const int32_t int32_tId = 1;
    //const int32_t int64_tId = 2;
    //const int32_t stringId = 3;

    int32_t* type = nullptr;
    int32_t* length = nullptr;

    int32_t* int32_tValue = nullptr;
    int64_t* int64_tValue = nullptr;
    std::string* stringValue = nullptr;

    int32_t* tlvSize = nullptr;

    std::vector<uint8_t>* allConnectedBytes = nullptr;
public:
    Tlv(int32_t value);

    Tlv(int64_t value);

    Tlv(std::string value);

    Tlv(std::vector<uint8_t> allConnectedBytes);

    ~Tlv();
    const int32_t* getInt32Value() const { return int32_tValue; }

    const int64_t* getInt64Value() const { return int64_tValue; }

    const std::string* getStringValue() const { return stringValue; }

    int32_t getTlvSize();

    allVars getValue();

    void clearAll();

    void setAllInt32_t(int32_t value);

    void setAllInt64_t(int64_t value);

    void setAllString(std::string value);

    int32_t getType();

    int32_t getLength();

    std::vector<uint8_t> getAllConnectedBytes();

    void loadAllBytesToDecode(std::vector<uint8_t> newBytes);

    void decode();

    int32_t GetStringByteSizeNoNull(const std::string& s);

    void showTlv();

    std::vector<uint8_t> marshalTlv();

};

#endif
