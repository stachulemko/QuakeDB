#include "column.h"
#include "tlv.h"
#include "dataBase.h"
#include "relation.h"
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

	//Database db;
	//db.loadDataBase();
	//addRelation("Users", "id", "Orders", "user_id" , "yes");
	//addRelation("Users", "id", "Orders", "user_id" , "yes");
	//db.commit();
	//db.select("Users", { "id", "name" }).where("name", "=", std::string("Alice")).showSqQuery();
	//return 0;
	//std::string sourceTable, std::string sourceColumn, std::string targetTable, std::string targetColumn, std::string onDelete
	Relation relation("firstTable", "idYzytkownika", "secondTable", "idYzytkownika", "sdasdas");
	Relation relation2("thirdTable", "idYzytkownika", "fourthTable", "idYzytkownika", "sdasdas");
	std::vector<uint8_t> bin = relation2.marshall();
	relation.decode(bin);


}
