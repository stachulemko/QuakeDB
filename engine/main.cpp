
#include "column.h"
#include "tlv.h"
#include "dataBase.h"
int main() {
	Database dataBase;

	dataBase.loadDataBase();
	dataBase.select("klienci", { "imie", "id" });
	
	//dataBase.addTable("klienci");
	//dataBase.addColumn("klienci", "id", int32_tId, false);
	//dataBase.addColumn("klienci", "imie", stringId, true);
	//dataBase.addRecord("klienci", { 1, "Jan" });
	//dataBase.commit();
	// 
	//dataBase.addRecord("klienci", { 2, "Anna" });
    //dataBase.addRecord("klienci", { 3, "Piotr" });
	//dataBase.commit();
	//Column column("testColumn", 1, false);
	//Record record({ 1,}, { &column });
	//record.showRecord();
	
	
	
	//dataBase.loadDataBase();
	//std::cout << "tableSize ----------------------------------------- [[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[ " << dataBase.getTableSize() << std::endl;
	//dataBase.showRecords();
	//std::cout << "test1";
	
	//dataBase.loadDataBase();
	//dataBase.showRecords();

}
