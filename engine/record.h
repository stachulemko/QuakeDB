#ifndef RECORD_H  
#define RECORD_H
#include <iostream>
#include <vector>
#include <variant>
#include "column.h"
#include "typeManager.h"
#include "tlv.h"

class Record {
private:
	int32_t recordId=5;
	std::vector<uint8_t>* allConnectedBytes = nullptr;
	std::vector<allVars> recordData;
	std::vector<Tlv*> recordDataTlv;
public:
	Record(std::vector<allVars> recordData);
	bool isDataTypeCorrect(std::vector<allVars> recordData,std::vector<Column*> vec);
	std::vector<uint8_t> MarshalRecord() {
		std::vector<uint8_t> result;
		for (int i = 0; i < recordData.size(); i++) {
			//Tlv* tlv = new Tlv(recordData[i]);
			//std::vector<uint8_t> tlvBytes = tlv->marshalTlv();
			//result.insert(result.end(), tlvBytes.begin(), tlvBytes.end());
			//recordDataTlv.push_back(tlv);
		}
		return result;
	}






};
#endif
