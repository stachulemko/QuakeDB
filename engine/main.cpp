
#include "column.h"
#include "tlv.h"
#include "dataBase.h"
int main() {
    Database db;
	db.addTable("testTable");
	db.addColumn("testTable", "id", int32_tId, false);
	db.addColumn("testTable", "name", stringId, false);
	db.addRecord("testTable", { 1, "TestName1" });
	db.addRecord("testTable", { 2, "TestName2" });
	db.addRecord("testTable", { 3, "TestName1" }); // Duplikat "TestName1"
	db.commit();
	db.select("testTable", { "id","name" }).showSqQuery();


    // Oczekiwany wynik zale¿y od implementacji - mo¿e byæ pusty wektor lub b³¹d
    // Tutaj zak³adam, ¿e funkcja zwraca pusty wektor jeœli nie ma indeksu
    //EXPECT_TRUE(blockNums.empty()) << "Dla kolumny bez indeksu B-tree powinien zostaæ zwrócony pusty wektor";
}
