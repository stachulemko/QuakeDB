
#include "column.h"
#include "tlv.h"
#include "dataBase.h"
int main() {
	/*
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
	nullableColumn.loadAllBytesToDecode(bytes1);
	nullableColumn.decodeColumn();
	nullableColumn.showColumn();
	std::cout << "------------------------------------" << std::endl;
    */
	
	//Database dataBase;
	
	//dataBase.addTable("klienci");
	//dataBase.addTable("pracownicy");
	//dataBase.addColumn("klienci", "id", 1, false);
	//dataBase.addColumn("pracownicy", "id", 1, false);
	//dataBase.commit();
	//dataBase.addColumn("klienci", "name", 3, true);
	Database dataBase;

	//std::string ExecutionPath = executionFilePath();
	//std::cout << "Execution Path: " << ExecutionPath << std::endl;
	//Database dataBase;
	//dataBase.loadDataBase();
	//dataBase.showFile();
	//dataBase.loadDataBase();
	//dataBase.showFile();
	//Sleep(1000000);
	//dataBase.sh/ow;
// File("klienci");
	//Sleep(10000000000);
	//Table table1("testTable","dataBaseMemoryManagment");
	//table1.addColumn("Column1", 1, true);
	//table1.showTable();
	//table1.addColumn("Column2", 2, false);

}
