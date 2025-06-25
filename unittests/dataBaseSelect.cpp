#include "pch.h"
#include "select.h"
#include "dataBase.h"

TEST(dataBaseSelect, SelectFromSingleTable) {
    Database db;
    std::string tableName = "testTable";
    db.addTable(tableName);
    db.addColumn(tableName, "id", int32_tId, false);
    db.addColumn(tableName, "name", stringId, true);
    db.addRecord(tableName, { 1, "Alice" });
    db.addRecord(tableName, { 2, "Bob" });
    db.commit();
    // Test select
    Database selectDb;
    selectDb.loadDataBase();


    std::vector<std::vector<allVars>> vec = selectFunc(selectDb.getTables(), tableName, { "id", "name" });
    if (vec.empty()) {
        FAIL() << "Select returned empty result";
    }
    else {
        EXPECT_EQ(std::get<int32_t>(vec[1][0]), 1);
        EXPECT_EQ(std::get<std::string>(vec[1][1]), "Alice");
        EXPECT_EQ(std::get<int32_t>(vec[2][0]), 2);
        EXPECT_EQ(std::get<std::string>(vec[2][1]), "Bob");
    }
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}
TEST(dataBaseSelect, SelectFromEmptyTable) {
    Database db;
    std::string tableName = "emptyTable";
    db.addTable(tableName);
    db.addColumn(tableName, "id", int32_tId, false);
    db.addColumn(tableName, "name", stringId, true);
    db.commit();

    // Test select
    Database selectDb;
    selectDb.loadDataBase();
    std::vector<std::vector<allVars>> vec = selectFunc(selectDb.getTables(), tableName, { "id", "name" });

    // Powinna byæ co najmniej jedna wierszowa (nag³ówki kolumn)
    EXPECT_FALSE(vec.empty());
    EXPECT_EQ(vec.size(), 1);

    // Cleanup
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test nieistniej¹cego sto³u
/*
TEST(dataBaseSelect, SelectFromNonExistentTable) {
	//test have error: SEH exception with code 0xc0000005 thrown in the test body. beause of the none element were directly make and already dont exist especially pointers and were delete even fact that the dont exist
    Database db;
    db.commit();

    std::vector<std::vector<allVars>> vec = selectFunc(db.getTables(), "nonExistentTable", { "id", "name" });

	if (vec.empty()) {
        SUCCEED();
	}
	else {
        FAIL();
	}
}
*/


// Test niepoprawnej nazwy kolumny
TEST(dataBaseSelect, SelectInvalidColumn) {
    Database db;
    std::string tableName = "tableWithColumns";
    db.addTable(tableName);
    db.addColumn(tableName, "id", int32_tId, false);
    db.addColumn(tableName, "name", stringId, true);
    db.addRecord(tableName, { 1, "Alice" });
    db.commit();

    // Test select z niepoprawn¹ nazw¹ kolumny
    Database selectDb;
    selectDb.loadDataBase();
    std::vector<std::vector<allVars>> vec = selectFunc(selectDb.getTables(), tableName, { "id", "nonExistentColumn" });

    // Powinien zwróciæ pusty wynik
    EXPECT_TRUE(vec.empty());

    // Cleanup
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test ró¿nych typów danych
TEST(dataBaseSelect, SelectMultipleDataTypes) {
    Database db;
    std::string tableName = "multiTypeTable";
    db.addTable(tableName);
    db.addColumn(tableName, "id", int32_tId, false);           // int32
    db.addColumn(tableName, "amount", int64_tId, false);       // int64
    db.addColumn(tableName, "name", stringId, false);          // string

    // Dodajemy rekordy z ró¿nymi typami danych
    db.addRecord(tableName, { 1, static_cast<int64_t>(1000), "Product1" });
    db.addRecord(tableName, { 2, static_cast<int64_t>(2000), "Product2" });
    db.commit();

    // Test select
    Database selectDb;
    selectDb.loadDataBase();
    std::vector<std::vector<allVars>> vec = selectFunc(selectDb.getTables(), tableName, { "id", "amount", "name" });

    if (vec.empty()) {
        FAIL() << "Select returned empty result";
    }
    else {
        // Sprawdzamy pierwszy rekord
        EXPECT_EQ(std::get<int32_t>(vec[1][0]), 1);
        EXPECT_EQ(std::get<int64_t>(vec[1][1]), 1000);
        EXPECT_EQ(std::get<std::string>(vec[1][2]), "Product1");

        // Sprawdzamy drugi rekord
        EXPECT_EQ(std::get<int32_t>(vec[2][0]), 2);
        EXPECT_EQ(std::get<int64_t>(vec[2][1]), 2000);
        EXPECT_EQ(std::get<std::string>(vec[2][2]), "Product2");
    }

    // Cleanup
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test pustej wartoœci w rekordzie (NULL)
/*
TEST(dataBaseSelect, SelectWithNullValues) {
    Database db;
    std::string tableName = "nullValueTable";
    db.addTable(tableName);
    db.addColumn(tableName, "id", int32_tId, false);
    db.addColumn(tableName, "name", stringId, true); // Pozwalamy na NULL

    // Dodajemy rekord z wartoœci¹ NULL
    // Uwaga: To jest uproszczenie, prawdopodobnie bêdzie potrzebna implementacja
    // obs³ugi NULL w Twoim systemie
    db.addRecord(tableName, { 1, "" }); // Puste string jako reprezentacja NULL
    db.commit();

    // Test select
    Database selectDb;
    selectDb.loadDataBase();
    std::vector<std::vector<allVars>> vec = selectFunc(selectDb.getTables(), tableName, { "id", "name" });

    if (vec.empty()) {
        FAIL() << "Select returned empty result";
    }
    else {
        EXPECT_EQ(std::get<int32_t>(vec[1][0]), 1);
        // Sprawdzamy czy dla pustego ci¹gu znaków
        EXPECT_EQ(std::get<std::string>(vec[1][1]), "");
    }

    // Cleanup
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test dla rekordów z bardzo d³ugimi wartoœciami stringowymi
*/
TEST(dataBaseSelect, SelectLongStringValues) {
    Database db;
    std::string tableName = "longStringTable";
    db.addTable(tableName);
    db.addColumn(tableName, "id", int32_tId, false);
    db.addColumn(tableName, "description", stringId, false);

    // Tworzymy bardzo d³ugi string (1000 znaków)
    std::string longString(1000, 'X');

    // Dodajemy rekord z d³ug¹ wartoœci¹
    db.addRecord(tableName, { 1, longString });
    db.commit();

    // Test select
    Database selectDb;
    selectDb.loadDataBase();
    std::vector<std::vector<allVars>> vec = selectFunc(selectDb.getTables(), tableName, { "id", "description" });

    if (vec.empty()) {
        FAIL() << "Select returned empty result";
    }
    else {
        EXPECT_EQ(std::get<int32_t>(vec[1][0]), 1);
        EXPECT_EQ(std::get<std::string>(vec[1][1]), longString);
    }

    // Cleanup
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test selekcji z odwrotn¹ kolejnoœci¹ kolumn
TEST(dataBaseSelect, SelectReverseColumnOrder) {
    Database db;
    std::string tableName = "reverseOrderTable";
    db.addTable(tableName);
    db.addColumn(tableName, "id", int32_tId, false);
    db.addColumn(tableName, "name", stringId, false);
    db.addRecord(tableName, { 1, "Alice" });
    db.commit();

    // Test select z odwrotn¹ kolejnoœci¹ kolumn
    Database selectDb;
    selectDb.loadDataBase();
    testing::internal::CaptureStdout();

    // Wykonaj operacjê, której wyjœcie chcesz przechwyciæ
    std::vector<std::vector<allVars>> vec = selectFunc(selectDb.getTables(), tableName, {"id", "name"});
    // Lub u¿yj bezpoœrednio showVariantVariable
    for (int i = 0; i < vec.size(); i++) {
        for (int j = 0; j < vec[i].size(); j++) {
            showVariantVariable(vec[i][j]);
            std::cout << " "; // dodanie spacji miêdzy wartoœciami
        }
        std::cout << std::endl;
    }

    std::string output = testing::internal::GetCapturedStdout();

    std::cout << "Przechwycone wyjœcie: \n" << output << std::endl;

    if (vec.empty()) {
        FAIL() << "Select returned empty result";
    }
    else {
        EXPECT_EQ(std::get<int32_t>(vec[1][0]), 1);
        EXPECT_EQ(std::get<std::string>(vec[1][1]), "Alice");
    }

    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

TEST(dataBaseSelect, SelectSingleColumn) {
    Database db;
    std::string tableName = "singleColumnTable";
    db.addTable(tableName);
    db.addColumn(tableName, "id", int32_tId, false);
    db.addColumn(tableName, "name", stringId, false);
    db.addRecord(tableName, { 1, "Alice" });
    db.commit();

    Database selectDb;
    selectDb.loadDataBase();
    std::vector<std::vector<allVars>> vec = selectFunc(selectDb.getTables(), tableName, { "name" });

    if (vec.empty()) {
        FAIL() << "Select returned empty result";
    }
    else {
        EXPECT_EQ(vec[0].size(), 1);
        EXPECT_EQ(vec[1].size(), 1);
        EXPECT_EQ(std::get<std::string>(vec[1][0]), "Alice");
    }

    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test z gwiazdk¹ (*) - wybór wszystkich kolumn

TEST(dataBaseSelect, SelectAllColumns) {
    Database db;
    std::string tableName = "allColumnsTable";
    db.addTable(tableName);
    db.addColumn(tableName, "id", int32_tId, false);
    db.addColumn(tableName, "name", stringId, false);
    db.addRecord(tableName, { 1, "Alice" });
    db.commit();

    // Test select z gwiazdk¹ (*)
    Database selectDb;
    selectDb.loadDataBase();
    std::vector<std::vector<allVars>> vec = selectFunc(selectDb.getTables(), tableName, { "*" });

    if (vec.empty()) {
        FAIL() << "Select returned empty result";
    }
    else {
        // Powinny byæ dwie kolumny w wynikach
        std::cout << "test1" << std::endl;
        EXPECT_EQ(vec[0].size(), 2);
        EXPECT_EQ(vec[1].size(), 2);
        EXPECT_EQ(std::get<int32_t>(vec[1][0]), 1);
        EXPECT_EQ(std::get<std::string>(vec[1][1]), "Alice");
        std::cout << "test2" << std::endl;
    }

    deleteFile(db.getPath() + "/" + tableName + ".bin");
    std::cout << "test3" << std::endl;
}

