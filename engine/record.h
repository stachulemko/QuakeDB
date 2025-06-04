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
	int32_t* recordSize = nullptr;
	std::vector<uint8_t>* allConnectedBytes = nullptr;
	std::vector<Tlv*> recordDataTlv;
public:
	Record(std::vector<allVars> recordData, std::vector<Column*> vec);
	bool isDataTypeCorrect(std::vector<allVars> recordData,std::vector<Column*> vec);
	std::vector<uint8_t> MarshalRecord();
	void showRecord() {
		//std::cout << "Record Size: " << *recordSize << std::endl;
		for (const auto& tlv : recordDataTlv) {
			tlv->showTlv();
		}
	}
	void clearAll() {
		delete recordSize;
		for (auto& tlv : recordDataTlv) {
			delete tlv;
		}
		recordDataTlv.clear();
		delete allConnectedBytes;
		allConnectedBytes = nullptr;
		recordSize = nullptr;
	}
	void loadAllConnectedBytes(std::vector<uint8_t> bytes) {
		clearAll();
		allConnectedBytes = new std::vector<uint8_t>(bytes);
	}
	void decode();





};
#endif
