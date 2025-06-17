#include "tlv.h"

Tlv::Tlv(int32_t value) {
    clearAll();
    type = new int32_t(int32_tId);
    length = new int32_t(4);
    int32_tValue = new int32_t(value);
    tlvSize = new int32_t(12);
    int64_tValue = nullptr;
    stringValue = nullptr;
    allConnectedBytes = nullptr;
}

Tlv::Tlv(int64_t value) {
    clearAll();
    type = new int32_t(int64_tId);
    length = new int32_t(8);
    tlvSize = new int32_t(16);
    int64_tValue = new int64_t(value);
    int32_tValue = nullptr;
    stringValue = nullptr;
    allConnectedBytes = nullptr;
}

Tlv::Tlv(std::string value) {
    clearAll();
    type = new int32_t(stringId);
    length = new int32_t(static_cast<int32_t>(value.size()));
    tlvSize = new int32_t(8+ static_cast<int32_t>(value.size()));
    stringValue = new std::string(value);
    int32_tValue = nullptr;
    int64_tValue = nullptr;
    allConnectedBytes = nullptr;
}

Tlv::Tlv(std::vector<uint8_t> allBytes) {
    clearAll();
    allConnectedBytes = new std::vector<uint8_t>(allBytes);
}

Tlv::~Tlv() {
    clearAll();
}

void Tlv::clearAll() {
    delete type;
    delete length;
    delete int32_tValue;
    delete int64_tValue;
    delete stringValue;
    delete allConnectedBytes;
    delete tlvSize;
    type = nullptr;
    length = nullptr;
    int32_tValue = nullptr;
    int64_tValue = nullptr;
    stringValue = nullptr;
    allConnectedBytes = nullptr;
    tlvSize = nullptr;
}

void Tlv::setAllInt32_t(int32_t value) {
    clearAll();
    type = new int32_t(int32_tId);
    length = new int32_t(4);
    int32_tValue = new int32_t(value);
    tlvSize = new int32_t(12);
    int64_tValue = nullptr;
    stringValue = nullptr;
    allConnectedBytes = nullptr;
}

void Tlv::setAllInt64_t(int64_t value) {
    clearAll();
    type = new int32_t(int64_tId);
    length = new int32_t(8);
    tlvSize = new int32_t(8+ *length);
    int64_tValue = new int64_t(value);
    int32_tValue = nullptr;
    stringValue = nullptr;
    allConnectedBytes = nullptr;
}

void Tlv::setAllString(std::string value) {
    clearAll();
    type = new int32_t(stringId);
    length = new int32_t(static_cast<int32_t>(value.size()));
    stringValue = new std::string(value);
    tlvSize = new int32_t(8 + *length);
    int32_tValue = nullptr;
    int64_tValue = nullptr;
    allConnectedBytes = nullptr;
}

int32_t Tlv::getType() {
    return type ? *type : 0;
}

int32_t Tlv::getLength() {
    return length ? *length : 0;
}

std::vector<uint8_t> Tlv::getAllConnectedBytes() {
    if (allConnectedBytes)
        return *allConnectedBytes;
    return std::vector<uint8_t>();
}

void Tlv::loadAllBytesToDecode(std::vector<uint8_t> newBytes) {
    clearAll();
    allConnectedBytes = new std::vector<uint8_t>(newBytes);
}

void Tlv::decode() {
    if (!allConnectedBytes || allConnectedBytes->size() < 8) {
        return;
    }
    //clearAll();
    std::vector<uint8_t> typeBytes;
    for (int i=0;i<=3;i++){
        typeBytes.push_back((*allConnectedBytes)[i]);
    }
    std::vector<uint8_t> LenghtBytes;
    for (int i = 4; i <= 6; i++) {
        typeBytes.push_back((*allConnectedBytes)[i]);
    }
    type = new int32_t();
    UnmarshalInt32_t(type, &typeBytes);

    std::vector<uint8_t> lengthBytes(allConnectedBytes->begin() + 4, allConnectedBytes->begin() + 8);
    length = new int32_t();
    UnmarshalInt32_t(length, &lengthBytes);
    tlvSize = new int32_t(8 + *length);

    if (*type == int32_tId && allConnectedBytes->size() >= 12) {
        std::vector<uint8_t> valueBytes(allConnectedBytes->begin() + 8, allConnectedBytes->begin() + 12);
        int32_tValue = new int32_t();
        UnmarshalInt32_t(int32_tValue, &valueBytes);
    }
    else if (*type == int64_tId && allConnectedBytes->size() >= 16) {
        std::vector<uint8_t> valueBytes(allConnectedBytes->begin() + 8, allConnectedBytes->begin() + 16);
        int64_tValue = new int64_t();
        UnmarshalInt64_t(int64_tValue, &valueBytes);
    }
    else if (*type == stringId && allConnectedBytes->size() >= 8 + *length) {
        std::vector<uint8_t> valueBytes(allConnectedBytes->begin() + 8, allConnectedBytes->begin() + 8 + *length);
        stringValue = new std::string();
        UnmarshalString(stringValue, &valueBytes);
    }
}

int32_t Tlv::GetStringByteSizeNoNull(const std::string& s) {
    return static_cast<int32_t>(s.size());
}

void Tlv::showTlv() {
    std::cout << "Type: " << (type ? *type : 0) << std::endl;
    std::cout << "Length: " << (length ? *length : 0) << std::endl;
    
    if (type && *type == int32_tId && int32_tValue)
        std::cout << "Value: " << *int32_tValue << std::endl;
    else if (type && *type == int64_tId && int64_tValue)
        std::cout << "Value: " << *int64_tValue << std::endl;
    else if (type && *type == stringId && stringValue)
        std::cout << "Value: " << *stringValue << std::endl;
    
}

std::vector<uint8_t> Tlv::marshalTlv() {
    std::vector<uint8_t> result;
    if (!type || !length)
        return result;

    std::vector<uint8_t>* typeBytes = marshalInt32_t(*type);
    std::vector<uint8_t>* lengthBytes = marshalInt32_t(*length);

    result.insert(result.end(), typeBytes->begin(), typeBytes->end());
    result.insert(result.end(), lengthBytes->begin(), lengthBytes->end());

    if (*type == int32_tId && int32_tValue) {
        std::vector<uint8_t>* valueBytes = marshalInt32_t(*int32_tValue);
        result.insert(result.end(), valueBytes->begin(), valueBytes->end());
        delete valueBytes;
    }
    else if (*type == int64_tId && int64_tValue) {
        std::vector<uint8_t>* valueBytes = marshalInt64_t(*int64_tValue);
        result.insert(result.end(), valueBytes->begin(), valueBytes->end());
        delete valueBytes;
    }
    else if (*type == stringId && stringValue) {
        std::vector<uint8_t>* valueBytes = marshalString(*stringValue);
        result.insert(result.end(), valueBytes->begin(), valueBytes->end());
        delete valueBytes;
    }

    delete typeBytes;
    delete lengthBytes;
    return result;
}

int32_t Tlv::getTlvSize() {
    if (tlvSize != nullptr) {
        return *tlvSize;
    }
    else {
        std::cerr << "Error : getting size before decode or set value" << std::endl;
        return -1;
    }
}

allVars Tlv::getValue() {
    if (int32_tValue != nullptr) {
        return *int32_tValue;
    }
    else if (int64_tValue != nullptr) {
        return *int64_tValue;
    }
    else if (stringValue != nullptr) {
        return *stringValue;
    }
    else {
        std::cerr << "Error: No value set in Tlv" << std::endl;
        return {};
    }
}