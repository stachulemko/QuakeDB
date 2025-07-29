
#include "column.h"
#include "tlv.h"
#include "dataBase.h"
int main() {
	/*
	Database db;
	// Example usage
	db.addTable("Users");
	db.addColumn("Users", "id", int32_tId, false);
	db.addColumn("Users", "name", stringId, true);

	db.addRecord("Users", { 1, std::string("Alice") });
	db.addRecord("Users", { 2, std::string("Bob") });
	db.addRecord("Users", { 1, std::string("Charlie") });
	db.addRecord("Users", { 1, std::string("Charlie") });
	db.addBtree("Users", "id");
	db.commit();
	*/


	Database loadDb;
	loadDb.loadDataBase();
	loadDb.select("Users", { "id", "name" });
	loadDb.where("id", "=", 1);
	loadDb.showSqQuery();

	//db.select("Users", { "id", "name" }).where("name", "=", std::string("Alice")).showSqQuery();
	//return 0;
}
