
#include "column.h"
#include "tlv.h"
#include "dataBase.h"
int main() {
    Database db;
    std::string name = "test1";
    db.addTable(name);
    db.addColumn(name, "t1", 1, true);
    db.commit();

    Database db1;
    db1.loadDataBase();
    db1.addColumn(name, "t3", 2, true);
    db1.commit();

    Database db2;
    db2.loadDataBase();
    db2.select(name, { "*" });
    db2.showSqQuery();
}
