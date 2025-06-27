#include "record.h"

Record::Record(std::vector<allVars> recordData, std::vector<Column*> vec) {
    if (isDataTypeCorrect(recordData, vec)) {
        recordSize = new int32_t(8);
        for (int i = 0; i < recordData.size(); i++) {
            Tlv* tlv = std::visit([](auto&& arg) -> Tlv* {
                return new Tlv(arg);
                }, recordData[i]);
            *recordSize += tlv->getTlvSize();
            if (tlv) {
                recordDataTlv.push_back(tlv);
            }
        }
    }
}

bool Record::isDataTypeCorrect(std::vector<allVars> recordData, std::vector<Column*> vec) {
    if (vec.size() != recordData.size()) {
        assert(false && "Error(insert): Number of columns does not match number of data types in record.");   //might be error
        //std::cerr << "Error(insert): Number of columns does not match number of data types in record." << std::endl;
        return false;
    }
    else {
        for (int i = 0; i < recordData.size(); i++) {
            if (vec[i]->getColumnType() != getTypeId(recordData[i]) /* and (vec[i]->getColumnType() != int64_tId and getTypeId(recordData[i]) != int32_tId)*/) {
                for (int i = 0; i < vec.size(); i++)
                {
                    std::cout << vec[i]->getColumnType() << std::endl;
                }
                assert(false && "Error(insert): Data type mismatch for column '");
                //std::cerr <<  << vec[i]->getColumnName() << "'. Expected type: " << vec[i]->getColumnType() << ", but got: " << getTypeId(recordData[i]) << std::endl;
                return false;
            }
			else if (getTypeId(recordData[i]) == stringId and std::get<std::string>(recordData[i]).empty()) {
                assert(false && "record data string empty");   //might be error
			}
        }
    }
    return true;
}

std::vector<uint8_t> Record::MarshalRecord() {
    std::vector<uint8_t> result;
    std::vector<uint8_t>* recordTypeBytes = marshalInt32_t(recordId);
    std::vector<uint8_t>* recordSizeBytes = marshalInt32_t(*recordSize);
    result.insert(result.end(), recordTypeBytes->begin(), recordTypeBytes->end());
    result.insert(result.end(), recordSizeBytes->begin(), recordSizeBytes->end());
    for (int i = 0; i < recordDataTlv.size(); i++) {
        std::vector<uint8_t> tlvBytes = recordDataTlv[i]->marshalTlv();
        result.insert(result.end(), tlvBytes.begin(), tlvBytes.end());
    }
    return result;
}

void Record::showRecord() {
    //std::cout << "Record Size: " << *recordSize << std::endl;
    for (const auto& tlv : recordDataTlv) {
        tlv->showTlv();
    }
}

std::vector<Tlv*> Record::getRecordDataTlv() {
    return recordDataTlv;
}

void Record::clearAll() {
    delete recordSize;
    for (auto& tlv : recordDataTlv) {
        delete tlv;
    }
    recordDataTlv.clear();
    delete allConnectedBytes;
    allConnectedBytes = nullptr;
    recordSize = nullptr;
}

void Record::loadAllConnectedBytes(std::vector<uint8_t> bytes) {
    clearAll();
    allConnectedBytes = new std::vector<uint8_t>(bytes);
}

void Record::decode() {
    if (allConnectedBytes->size() < 8) {
        std::cerr << "Insufficient data for record decoding" << std::endl;
        return;
    }
    else {
        std::vector<uint8_t> typeBytes(allConnectedBytes->begin(), allConnectedBytes->begin() + 4);
        int32_t* type = new int32_t();
        UnmarshalInt32_t(type, &typeBytes);
        if (*type == recordId) {
            std::vector<uint8_t> sizeBytes(allConnectedBytes->begin() + 4, allConnectedBytes->begin() + 8);
            int32_t* size = new int32_t();
            UnmarshalInt32_t(size, &sizeBytes);
            recordSize = new int32_t(*size);
            if (*size== allConnectedBytes->size()) {
                int32_t offset = 8;
                int32_t* typeRecordTlv = new int32_t();
                int32_t* lengthRecordTlv = new int32_t();
                while (offset < allConnectedBytes->size()) {
                    std::vector<uint8_t> dataTlvTypeBytes(allConnectedBytes->begin() + offset, allConnectedBytes->begin() + offset + 4);
                    UnmarshalInt32_t(typeRecordTlv, &dataTlvTypeBytes);
                    offset += 4;
                    std::vector<uint8_t> dataTlvLengthBytes(allConnectedBytes->begin() + offset, allConnectedBytes->begin() + offset + 4);
                    UnmarshalInt32_t(lengthRecordTlv, &dataTlvLengthBytes);
                    offset += 4;
                    if (*lengthRecordTlv > 0 && *lengthRecordTlv <= allConnectedBytes->size() - offset) {
                        std::vector<uint8_t> dataTlvValue(allConnectedBytes->begin() + offset - 8, allConnectedBytes->begin() + offset + *lengthRecordTlv);
                        offset += *lengthRecordTlv;
                        Tlv* tlv = new Tlv(dataTlvValue);
                        tlv->decode();
                        recordDataTlv.push_back(tlv);
                    }
                    else {
                        //std::cerr << "Error : Invalid dataTlv record size " << std::endl;
                        assert(false && "Invalid dataTlv record size");   //might be error
                        delete typeRecordTlv;
                        delete lengthRecordTlv;
                        delete type;
                        return;
                    }
                }
            }
            else {
                delete recordSize;
                std::cerr << "Error : Invalid dataTlv record size " << std::endl;
            }
        }
        else {
            std::cerr << "Invalid record type ID: " << *type << std::endl;
            delete type;
            return;
        }
    }
}
int32_t Record::getRecordSize() {
    if (recordSize != nullptr) {
        return *recordSize;
    }
    else {
        std::cerr << "Error: Record size is not set." << std::endl;
        return -1;
    }
}

std::vector<allVars> Record::getRecordData() {
    std::vector<allVars> data;
    for (const auto& tlv : recordDataTlv) {
        data.push_back(tlv->getValue());
    }
    return data;
}

