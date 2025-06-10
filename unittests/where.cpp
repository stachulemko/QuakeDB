#include "pch.h"
#include "dataBase.h"
TEST(WhereTests, EqualSimpleWhereCondition) {
	Database db;
	db.addTable("testTable");
	db.addColumn("testTable", "id", int32_tId, false);
	db.addColumn("testTable", "name", stringId, true);
	db.addRecord("testTable", { 1, "Alice" });
	db.addRecord("testTable", { 2, "Bob" });
	db.commit();

	Database db1;
	db1.loadDataBase();
	db1.select("testTable", { "id", "name" })
		.where("id", "=", 1);
	std::vector<std::vector<allVars>> result = db1.getSqlQueryBytes();
	EXPECT_EQ(std::get<std::string>(result[0][0]), "id");
	EXPECT_EQ(std::get<std::string>(result[0][1]), "name");
	EXPECT_EQ(std::get<int32_t>(result[1][0]), 1);
	EXPECT_EQ(std::get<std::string>(result[1][1]), "Alice");

	deleteFile(db.getPath() + "/testTable.bin");
}


TEST(WhereTests, GreaterThanEdgeCondition) {
    Database db;
    db.addTable("testTable");
    db.addColumn("testTable", "id", int32_tId, false);
    db.addColumn("testTable", "value", int32_tId, false);

    // Dodanie rekordów z wartoœciami brzegowymi
    db.addRecord("testTable", { 1, 100 });
    db.addRecord("testTable", { 2, 101 });
    db.addRecord("testTable", { 3, 99 });
    db.commit();

    Database db1;
    db1.loadDataBase();
    db1.select("testTable", { "id", "value" })
        .where("value", ">", 100);
    std::vector<std::vector<allVars>> result = db1.getSqlQueryBytes();

    ASSERT_EQ(result.size(), 2); 
    EXPECT_EQ(std::get<std::string>(result[0][0]), "id");
    EXPECT_EQ(std::get<std::string>(result[0][1]), "value");

    EXPECT_EQ(std::get<int32_t>(result[1][0]), 2);
    EXPECT_EQ(std::get<int32_t>(result[1][1]), 101);

    deleteFile(db.getPath() + "/testTable.bin");
}


TEST(WhereTests, DifferentDataTypesCondition) {
    Database db;
    db.addTable("complexTable");
    db.addColumn("complexTable", "id", int32_tId, false);
    db.addColumn("complexTable", "amount", int64_tId, false);
    db.addColumn("complexTable", "name", stringId, false);

    db.addRecord("complexTable", { 1, static_cast<int64_t>(1000000000000), "Product1" });
    db.addRecord("complexTable", { 2, static_cast<int64_t>(2000000000000), "Product2" });
    db.addRecord("complexTable", { 3, static_cast<int64_t>(3000000000000), "Product3" });
    db.commit();

    Database db1;
    db1.loadDataBase();

    db1.select("complexTable", { "id", "amount", "name" })
        .where("amount", ">", static_cast<int64_t>(2000000000000));
    std::vector<std::vector<allVars>> result = db1.getSqlQueryBytes();

    ASSERT_EQ(result.size(), 2); 
    EXPECT_EQ(std::get<int32_t>(result[1][0]), 3);
    EXPECT_EQ(std::get<int64_t>(result[1][1]), static_cast<int64_t>(3000000000000));
    EXPECT_EQ(std::get<std::string>(result[1][2]), "Product3");

    deleteFile(db.getPath() + "/complexTable.bin");
}

TEST(WhereTests, StringCaseSensitiveCondition) {
    Database db;
    db.addTable("stringTable");
    db.addColumn("stringTable", "id", int32_tId, false);
    db.addColumn("stringTable", "text", stringId, false);

    db.addRecord("stringTable", { 1, "Hello" });
    db.addRecord("stringTable", { 2, "hello" });
    db.addRecord("stringTable", { 3, "HELLO" });
    db.commit();

    Database db1;
    db1.loadDataBase();
    db1.select("stringTable", { "id", "text" })
        .where("text", "=", "Hello");
    std::vector<std::vector<allVars>> result = db1.getSqlQueryBytes();

    ASSERT_EQ(result.size(), 2); 
    EXPECT_EQ(std::get<int32_t>(result[1][0]), 1);
    EXPECT_EQ(std::get<std::string>(result[1][1]), "Hello");

    deleteFile(db.getPath() + "/stringTable.bin");
}

TEST(WhereTests, VeryLongStringCondition) {
    Database db;
    db.addTable("longStringTable");
    db.addColumn("longStringTable", "id", int32_tId, false);
    db.addColumn("longStringTable", "description", stringId, false);

    std::string longString1(1000, 'A');
    std::string longString2(1000, 'B');

    db.addRecord("longStringTable", { 1, longString1 });
    db.addRecord("longStringTable", { 2, longString2 });
    db.commit();

    Database db1;
    db1.loadDataBase();
    db1.select("longStringTable", { "id", "description" })
        .where("description", "=", longString1);
    std::vector<std::vector<allVars>> result = db1.getSqlQueryBytes();

    // Sprawdzamy czy znaleziono w³aœciwy rekord
    ASSERT_EQ(result.size(), 2); // Nag³ówki + 1 rekord
    EXPECT_EQ(std::get<int32_t>(result[1][0]), 1);
    EXPECT_EQ(std::get<std::string>(result[1][1]), longString1);

    deleteFile(db.getPath() + "/longStringTable.bin");
}

// Test scenariusza kiedy ¿aden rekord nie spe³nia warunku
TEST(WhereTests, NoMatchingRecords) {
    Database db;
    db.addTable("emptyResultTable");
    db.addColumn("emptyResultTable", "id", int32_tId, false);
    db.addColumn("emptyResultTable", "value", int32_tId, false);

    db.addRecord("emptyResultTable", { 1, 100 });
    db.addRecord("emptyResultTable", { 2, 200 });
    db.commit();

    Database db1;
    db1.loadDataBase();
    db1.select("emptyResultTable", { "id", "value" })
        .where("value", "=", 999); // ¯aden rekord nie ma takiej wartoœci
    std::vector<std::vector<allVars>> result = db1.getSqlQueryBytes();

    // Powinniœmy mieæ tylko nag³ówki, bez rekordów
    ASSERT_EQ(result.size(), 1); // Tylko nag³ówki
    EXPECT_EQ(std::get<std::string>(result[0][0]), "id");
    EXPECT_EQ(std::get<std::string>(result[0][1]), "value");

    deleteFile(db.getPath() + "/emptyResultTable.bin");
}

// Test filtrowania po wielu rekordach z jednakow¹ wartoœci¹
TEST(WhereTests, MultipleMatchingRecords) {
    Database db;
    db.addTable("duplicateTable");
    db.addColumn("duplicateTable", "id", int32_tId, false);
    db.addColumn("duplicateTable", "category", int32_tId, false);
    db.addColumn("duplicateTable", "name", stringId, false);

    db.addRecord("duplicateTable", { 1, 10, "Item1" });
    db.addRecord("duplicateTable", { 2, 20, "Item2" });
    db.addRecord("duplicateTable", { 3, 10, "Item3" });
    db.addRecord("duplicateTable", { 4, 10, "Item4" });
    db.addRecord("duplicateTable", { 5, 20, "Item5" });
    db.commit();

    Database db1;
    db1.loadDataBase();
    db1.select("duplicateTable", { "id", "name" })
        .where("category", "=", 10);
    std::vector<std::vector<allVars>> result = db1.getSqlQueryBytes();

    // Powinniœmy mieæ 3 rekordy z category=10
    ASSERT_EQ(result.size(), 4); // Nag³ówki + 3 rekordy

    // Sprawdzamy nag³ówki
    EXPECT_EQ(std::get<std::string>(result[0][0]), "id");
    EXPECT_EQ(std::get<std::string>(result[0][1]), "name");

    // Sprawdzamy czy zwrócono wszystkie 3 rekordy
    std::vector<int> foundIds;
    for (size_t i = 1; i < result.size(); i++) {
        foundIds.push_back(std::get<int32_t>(result[i][0]));
    }

    // Sortujemy wyniki, aby uniezale¿niæ test od kolejnoœci rekordów
    std::sort(foundIds.begin(), foundIds.end());

    // Sprawdzamy czy znaleŸliœmy ID 1, 3 i 4
    EXPECT_EQ(foundIds.size(), 3);
    EXPECT_EQ(foundIds[0], 1);
    EXPECT_EQ(foundIds[1], 3);
    EXPECT_EQ(foundIds[2], 4);

    deleteFile(db.getPath() + "/duplicateTable.bin");
}

// Test z³o¿onych warunków z operatorami mniejszoœci
TEST(WhereTests, LessThanComplexCondition) {
    Database db;
    db.addTable("rangeTable");
    db.addColumn("rangeTable", "id", int32_tId, false);
    db.addColumn("rangeTable", "score", int32_tId, false);

    // Dodajemy rekordy z ró¿nymi wartoœciami
    for (int i = 1; i <= 10; i++) {
        db.addRecord("rangeTable", { i, i * 10 });
    }
    db.commit();

    Database db1;
    db1.loadDataBase();
    db1.select("rangeTable", { "id", "score" })
        .where("score", "<", 50);
    std::vector<std::vector<allVars>> result = db1.getSqlQueryBytes();

    // Powinniœmy mieæ 4 rekordy z score < 50 (10, 20, 30, 40)
    ASSERT_EQ(result.size(), 5); // Nag³ówki + 4 rekordy

    // Sprawdzamy, czy mamy wszystkie ID od 1 do 4
    std::vector<int> expectedIds = { 1, 2, 3, 4 };
    std::vector<int> foundIds;

    for (size_t i = 1; i < result.size(); i++) {
        foundIds.push_back(std::get<int32_t>(result[i][0]));
    }

    std::sort(foundIds.begin(), foundIds.end());
    EXPECT_EQ(foundIds, expectedIds);

    deleteFile(db.getPath() + "/rangeTable.bin");
}

// Test warunków brzegowych dla int32_t (wartoœci min/max)
TEST(WhereTests, Int32EdgeValuesCondition) {
    Database db;
    db.addTable("int32EdgeTable");
    db.addColumn("int32EdgeTable", "id", int32_tId, false);
    db.addColumn("int32EdgeTable", "value", int32_tId, false);

    // Dodajemy rekordy z wartoœciami brzegowymi dla int32_t
    db.addRecord("int32EdgeTable", { 1, INT32_MAX });
    db.addRecord("int32EdgeTable", { 2, INT32_MIN });
    db.addRecord("int32EdgeTable", { 3, 0 });
    db.commit();

    Database db1;
    db1.loadDataBase();

    // Test dla INT32_MAX
    db1.select("int32EdgeTable", { "id", "value" })
        .where("value", "=", INT32_MAX);
    std::vector<std::vector<allVars>> resultMax = db1.getSqlQueryBytes();
    ASSERT_EQ(resultMax.size(), 2); // Nag³ówki + 1 rekord
    EXPECT_EQ(std::get<int32_t>(resultMax[1][0]), 1);
    EXPECT_EQ(std::get<int32_t>(resultMax[1][1]), INT32_MAX);

    // Test dla INT32_MIN
    db1.select("int32EdgeTable", { "id", "value" })
        .where("value", "=", INT32_MIN);
    std::vector<std::vector<allVars>> resultMin = db1.getSqlQueryBytes();
    ASSERT_EQ(resultMin.size(), 2); // Nag³ówki + 1 rekord
    EXPECT_EQ(std::get<int32_t>(resultMin[1][0]), 2);
    EXPECT_EQ(std::get<int32_t>(resultMin[1][1]), INT32_MIN);

    deleteFile(db.getPath() + "/int32EdgeTable.bin");
}
/*
TEST(WhereTests, EmptyStringCondition) {
    Database db;
    db.addTable("emptyStringTable");
    db.addColumn("emptyStringTable", "id", int32_tId, false);
    db.addColumn("emptyStringTable", "text", stringId, true);

    ASSERT_NO_FATAL_FAILURE({
        db.addRecord("emptyStringTable", { 1, "text" });
        });

    ASSERT_NO_FATAL_FAILURE({
        db.addRecord("emptyStringTable", { 2, "" });
        });
    deleteFile(db.getPath() + "/emptyStringTable.bin");
}
*/
TEST(WhereTests, SelectAllColumnsWithWhere) {
    Database db;
    db.addTable("allColsTable");
    db.addColumn("allColsTable", "id", int32_tId, false);
    db.addColumn("allColsTable", "value", int32_tId, false);
    db.addColumn("allColsTable", "name", stringId, false);

    db.addRecord("allColsTable", { 1, 100, "Item1" });
    db.addRecord("allColsTable", { 2, 200, "Item2" });
    db.commit();

    Database db1;
    db1.loadDataBase();
    db1.select("allColsTable", { "*" })
        .where("id", "=", 2);
    std::vector<std::vector<allVars>> result = db1.getSqlQueryBytes();

    ASSERT_EQ(result.size(), 2);
    ASSERT_EQ(result[0].size(), 3); 
    EXPECT_EQ(std::get<int32_t>(result[1][0]), 2);
    EXPECT_EQ(std::get<int32_t>(result[1][1]), 200);
    EXPECT_EQ(std::get<std::string>(result[1][2]), "Item2");

    deleteFile(db.getPath() + "/allColsTable.bin");
}

// Test obs³ugi rekordów o maksymalnej liczbie kolumn (stress test)
TEST(WhereTests, ManyColumnsStressTest) {
    const int NUM_COLUMNS = 50; // Du¿a liczba kolumn

    Database db;
    db.addTable("manyColsTable");

    // Dodajemy wiele kolumn
    std::cout << "===============columns=======================" << std::endl;
    for (int i = 0; i < NUM_COLUMNS; i++) {
        std::string colName = "col" + std::to_string(i);
        db.addColumn("manyColsTable", colName, i % 3 == 0 ? int32_tId : (i % 3 == 1 ? int64_tId : stringId), false);
		std::cout << colName << std::endl;
    }
    std::cout << "===============columns=======================" << std::endl;
    
    std::cout << "===============record=======================" << std::endl;
    std::vector<allVars> recordData;
    for (int i = 0; i < NUM_COLUMNS; i++) {
        if (i % 3 == 0) {
            recordData.push_back(i); 
            std::cout << i << std::endl;
        }
        else if (i % 3 == 1) {
            recordData.push_back(static_cast<int64_t>(i * 1000)); 
            std::cout << static_cast<int64_t>(i * 1000) << std::endl;
        }
        else {
            recordData.push_back("Value" + std::to_string(i)); 
            std::cout << "Value" + std::to_string(i) << std::endl;
        }
        
    }
    std::cout << "===============record=======================" << std::endl;

    // Dodaj rekord z wieloma kolumnami
    db.addRecord("manyColsTable", recordData);
    db.commit();

    Database db1;
    db1.loadDataBase();

    // Wybieramy kolumny o indeksie podzielnym przez 10
    std::vector<std::string> selectedCols;
    for (int i = 0; i < NUM_COLUMNS; i += 10) {
        selectedCols.push_back("col" + std::to_string(i));
    }

    db1.select("manyColsTable", selectedCols)
        .where("col0", "=", 0);
    std::vector<std::vector<allVars>> result = db1.getSqlQueryBytes();

    ASSERT_EQ(result.size(), 2); 
    ASSERT_EQ(result[0].size(), selectedCols.size());

    for (size_t i = 0; i < selectedCols.size(); i++) {
        EXPECT_EQ(std::get<std::string>(result[0][i]), selectedCols[i]);
    }
    //std::cout << "Test1" << std::endl;
    //EXPECT_EQ(std::get<int64_t>(result[1][0]), 0); 

    //if (selectedCols.size() > 1) {
    //    EXPECT_EQ(std::get<int64_t>(result[1][1]), 10); 
    ///}
    //std::cout << "Test2" << std::endl;
    //if (selectedCols.size() > 2) {
    //    EXPECT_EQ(std::get<int64_t>(result[1][2]), 20); 
    //}
    //std::cout << "Test3" << std::endl;
    deleteFile(db.getPath() + "/manyColsTable.bin");
}

TEST(WhereTests, MultipleSelectWhereSequence) {
    Database db;
    db.addTable("sequenceTable");
    db.addColumn("sequenceTable", "id", int32_tId, false);
    db.addColumn("sequenceTable", "type", int32_tId, false);
    db.addColumn("sequenceTable", "name", stringId, false);

    // Dodajemy ró¿norodne rekordy
    db.addRecord("sequenceTable", { 1, 1, "Apple" });
    db.addRecord("sequenceTable", { 2, 1, "Banana" });
    db.addRecord("sequenceTable", { 3, 2, "Carrot" });
    db.addRecord("sequenceTable", { 4, 2, "Potato" });
    db.addRecord("sequenceTable", { 5, 3, "Beef" });
    db.commit();

    Database db1;
    db1.loadDataBase();

    // Sekwencja operacji select-where bez wymazywania poprzednich wyników
    db1.select("sequenceTable", { "id", "name" })
        .where("type", "=", 1);
    std::vector<std::vector<allVars>> result1 = db1.getSqlQueryBytes();

    db1.select("sequenceTable", { "id", "name" })
        .where("type", "=", 2);
    std::vector<std::vector<allVars>> result2 = db1.getSqlQueryBytes();

    db1.select("sequenceTable", { "id" })
        .where("type", "=", 3);
    std::vector<std::vector<allVars>> result3 = db1.getSqlQueryBytes();

    // Sprawdzamy wyniki pierwszej operacji
    ASSERT_EQ(result1.size(), 3); // Nag³ówki + 2 rekordy
    EXPECT_EQ(std::get<int32_t>(result1[1][0]), 1);
    EXPECT_EQ(std::get<std::string>(result1[1][1]), "Apple");
    EXPECT_EQ(std::get<int32_t>(result1[2][0]), 2);
    EXPECT_EQ(std::get<std::string>(result1[2][1]), "Banana");

    // Sprawdzamy wyniki drugiej operacji
    ASSERT_EQ(result2.size(), 3); // Nag³ówki + 2 rekordy
    EXPECT_EQ(std::get<int32_t>(result2[1][0]), 3);
    EXPECT_EQ(std::get<std::string>(result2[1][1]), "Carrot");
    EXPECT_EQ(std::get<int32_t>(result2[2][0]), 4);
    EXPECT_EQ(std::get<std::string>(result2[2][1]), "Potato");

    // Sprawdzamy wyniki trzeciej operacji
    ASSERT_EQ(result3.size(), 2); // Nag³ówki + 1 rekord
    EXPECT_EQ(std::get<int32_t>(result3[1][0]), 5);

    deleteFile(db.getPath() + "/sequenceTable.bin");
}
