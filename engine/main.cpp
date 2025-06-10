
#include "column.h"
#include "tlv.h"
#include "dataBase.h"
int main() {
    /*
    Database db;
    db.addTable("duplicateTable");
    db.addColumn("duplicateTable", "id", int32_tId, false);
    db.addColumn("duplicateTable", "category", int32_tId, false);
    db.addColumn("duplicateTable", "name", stringId, false);
    //--------------------
    db.addRecord("duplicateTable", { 1, 10, "Item1" });
    db.addRecord("duplicateTable", { 2, 20, "Item2" });
    db.addRecord("duplicateTable", { 3, 10, "Item3" });
    db.addRecord("duplicateTable", { 4, 10, "Item4" });
    db.addRecord("duplicateTable", { 5, 20, "Item5" });
    db.commit();
    */

    Database db1;
    db1.loadDataBase();
    db1.select("duplicateTable", { "id", "name" });
    db1.where("category", "=", 10);
    std::cout << "##################" << std::endl;
    db1.showSqQuery();
    std::vector<std::vector<allVars>> result = db1.getSqlQueryBytes();
    
}
