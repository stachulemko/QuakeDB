
#include "column.h"
#include "tlv.h"
#include "dataBase.h"
int main() {
    std::string tableName = "bTreeTestTable";
    Database db;

    // Utwórz tabelê z kolumnami
    db.addTable(tableName);
    db.addColumn(tableName, "id", int32_tId, false);
    db.addColumn(tableName, "name", stringId, false);
    db.addColumn(tableName, "value", int64_tId, false);

    // Dodaj rekordy, które wype³ni¹ przynajmniej dwa bloki
    for (int i = 1; i <= 50; i++) {
        std::string name = "Record-" + std::to_string(i);
        db.addRecord(tableName, { i, name, (int64_t)(i * 100) });
    }

    // Dodaj wiêcej rekordów z d³u¿szymi ci¹gami znaków
    for (int i = 51; i <= 100; i++) {
        std::string name = "LongRecord-" + std::to_string(i) + std::string(500, 'X');
        db.addRecord(tableName, { i, name, (int64_t)(i * 100) });
    }

    // Dodaj B-tree dla kolumny "name"
    //ASSERT_NO_THROW(db.addBtree(tableName, "name")) << "Dodanie B-tree zakoñczy³o siê b³êdem";

    // SprawdŸ numery bloków
    std::vector<int64_t> blockNums = db.getTableBlockNums(tableName);
    //ASSERT_GE(blockNums.size(), 2) << "Tabela powinna mieæ co najmniej 2 bloki";

    // SprawdŸ czy getBlockNum zwraca prawid³owy numer bloku
    int blockNum1 = db.getBlockNum(tableName, "name", std::string("Record-10"));
    //EXPECT_GE(blockNum1, 0) << "getBlockNum powinien zwróciæ prawid³owy numer bloku";

    // SprawdŸ numer bloku dla elementu z drugiego bloku
    std::string longRecordName = "LongRecord-75" + std::string(500, 'X');
    int blockNum2 = db.getBlockNum(tableName, "name", longRecordName);
    //EXPECT_EQ(blockNum2, blockNums[1]) << "getBlockNum dla elementu z drugiego bloku powinien zwróciæ numer drugiego bloku";

    // Wyczyœæ po teœcie
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}
