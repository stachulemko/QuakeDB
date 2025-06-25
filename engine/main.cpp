
#include "column.h"
#include "tlv.h"
#include "dataBase.h"
int main() {
	/*
	Database db;
	db.addTable("negrogarcy");
	db.addColumn("negrogarcy", "id", int32_tId, false);
	db.addColumn("negrogarcy", "name", stringId, false);
	db.addColumn("negrogarcy", "amount", int64_tId, false);
	db.addColumn("negrogarcy", "tempCol", int32_tId, true);
	db.addColumn("negrogarcy", "tempCol2", int32_tId, true);
	db.addRecord("negrogarcy", { 1, "Alice", 1000, 1, 2 });
	db.addRecord("negrogarcy", { 2, "Bob", 2000, 3, 4 });
	db.addRecord("negrogarcy", { 3, "Charlie", 3000, 5, 6 });
	db.commit();
	*/

	Database db;
	db.loadDataBase();
	std::vector<int64_t> blockNums = db.getTableBlockNums("negrogarcy");
	for (int64_t blockNum : blockNums) {
		std::cout << "Block number: " << blockNum << std::endl;
	}
	db.select("negrogarcy", { "id", "name", "amount", "tempCol", "tempCol2" });
	db.where("tempCol", ">", 2);
	db.showSqQuery();
	db.showWal();

}
