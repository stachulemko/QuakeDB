#pragma once
#include "column.h"
#include "tlv.h"

int main() {
    // Wyœwietl komunikat przed utworzeniem obiektu
    //std::cout << "Starting program..." << std::endl;

    // Tworzenie obiektu Tlv
    Tlv tlv(2);
	std::vector<uint8_t> bytes = tlv.marshalTlv();
	for (size_t i = 0; i < bytes.size(); ++i) {
		std::cout << std::bitset<8>(bytes[i]) << " ";
	}	
	//std::cout << "test1" << std::endl;
	tlv.loadAllBytesToDecode(bytes);
	tlv.decode();
	tlv.showTlv();
	
	Column nullableColumn("Nullable", 1, true);
	std::vector<uint8_t> bytes1;
	bytes1 = nullableColumn.MarshalColumn();
	for (size_t i = 0; i < bytes1.size(); ++i) {
		std::cout << std::bitset<8>(bytes1[i]) << " ";
	}
	nullableColumn.loadloadAllBytesToDecode(bytes1);
	nullableColumn.decodeColumn();
	nullableColumn.showColumn();
}
