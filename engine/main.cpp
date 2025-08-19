#include "column.h"
#include "tlv.h"
#include "dataBase.h"
#include "relation.h"
#include "relations.h"
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
	//Relation relation("firstTable", "idYzytkownika", "secondTable", "idYzytkownika", "sdasdas");
	//Relation relation2("thirdTable", "idYzytkownika", "fourthTable", "idYzytkownika", "sdasdas");
	//std::vector<uint8_t> bin = relation2.marshall();
	//relation.decode(bin);
	/*
	Database db1;
	db1.addTable("klienci");
	db1.addColumn("klienci", "id", int32_tId, false);
	db1.addColumn("klienci", "name", stringId, false);
	db1.addRecord("klienci", { 1, std::string("Jan Kowalski") });
	db1.addRecord("klienci", { 2, std::string("Anna Nowak") });
	db1.addTable("zamowienia");
	db1.addColumn("zamowienia", "id", int32_tId, false);
	db1.addColumn("zamowienia", "idKlienta", int32_tId, false);
	db1.addColumn("zamowienia", "produkt", stringId, false);
	db1.addRecord("zamowienia", { 1, 1, std::string("Laptop") });
	db1.addRecord("zamowienia", { 2, 2, std::string("Smartfon") });
	db1.addRecord("zamowienia", { 3, 1, std::string("Tablet") });
	db1.addRecord("zamowienia", {4,1,std::string("komputer")});
	//db1.addRelation("klienci", "id", "zamowienia", "idKlienta", "CASCADE");
	db1.commit();
    */


	//Database db1;
	//db1.show
	//Database db;
	//db.loadDataBase();
	//db.select("zamowienia", { "produkt","idKlienta"});
	//db.innerJoin("klienci", "id", "idKlienta");
	//db.where("idKlienta", "=", 1);
	//db.showSqlQuery();













	//addRelation("firstTable", "idYzytkownika", "secondTable", "idYzytkownika", "sdasdas");
	//db1.commit();

	//addRelation("firstTable", "idYzytkownika", "secondTable", "idYzytkownika", "sdasdas");
	//test35();


}
