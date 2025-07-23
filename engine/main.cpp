
#include "column.h"
#include "tlv.h"
#include "dataBase.h"
int main() {
    
    /*
    Database d1;
    d1.addTable("klienci");
    d1.addColumn("klienci","data",int32_tId,true);
    d1.addColumn("klienci", "klient", stringId, true);
    d1.addRecord("klienci", { 1,"janek" });
    d1.addRecord("klienci", { 2,"zyd" });
    d1.addRecord("klienci", { 1,"niger" });
    d1.commit();
    */
    //Database loadDb;
    //loadDb.loadDataBase();
    //loadDb.select("testTable", { "*" }).showSqQuery();
    //loadDb.addBtree("klienci", "data");
    //std::vector<int> blokcNums = loadDb.getBlockNum("klienci", "data", 1);
    Database db;
    db.loadDataBase();
	db.addBtree("klienci", "data");
	//std::vector < std::vector <allVars>> vec = db.g("klienci", "data", { "data", "klient" }, 1);
    db.select("klienci", { "data", "klient" });
    db.where("data", "=", 1);
    db.showSqQuery();

    
    
}
