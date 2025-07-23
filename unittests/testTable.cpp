#include "pch.h"
#include "table.h"
#include "wal.h"

// Resetuje przechwytywanie stdout aby unikn¹æ b³êdu "Only one stdout capturer can exist at a time"

void ResetStdoutCapture() {
    testing::internal::GetCapturedStdout();
}


// Pomocnicza funkcja do sprawdzania, czy wektor zawiera rekord o okreœlonych wartoœciach
bool containsRecord(const std::vector<std::vector<allVars>>& records, const std::vector<allVars>& record) {
    for (const auto& r : records) {
        if (r.size() != record.size()) continue;

        bool match = true;
        for (size_t i = 0; i < r.size(); i++) {
            if (r[i] != record[i]) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}

TEST(TableTests, GetRowsByBtreeBasicTest) {
    // Utwórz tabelê z prostym schematem
    Wal* wal = new Wal();
    wal->createDirectoryAndFile();
    Table table("testGetRowsTable", ".", wal);

    // Dodaj kolumny
    table.addColumn("id", int32_tId, false);
    table.addColumn("name", stringId, false);
    table.addColumn("value", int64_tId, false);

    // Dodaj rekordy
    table.addRecord({ 1, std::string("Record1"), (int64_t)100 });
    table.addRecord({ 2, std::string("Record2"), (int64_t)200 });
    table.addRecord({ 3, std::string("Record3"), (int64_t)300 });
    table.addRecord({ 4, std::string("Record2"), (int64_t)400 }); // Duplikat nazwy "Record2"

    // Dodaj indeks B-tree dla kolumny "name"
    table.addBtree("name");

    // Wyszukaj rekordy po nazwie "Record2"
    std::vector<std::vector<allVars>> result = table.getRowsByBtree("name", std::string("Record2"));

    // SprawdŸ czy znaleziono dok³adnie 2 rekordy
    ASSERT_EQ(result.size(), 2) << "Powinny zostaæ znalezione 2 rekordy z nazw¹ 'Record2'";

    // SprawdŸ czy znalezione rekordy maj¹ odpowiednie wartoœci
    EXPECT_TRUE(containsRecord(result, { 2, std::string("Record2"), (int64_t)200 }))
        << "Nie znaleziono rekordu {2, 'Record2', 200}";
    EXPECT_TRUE(containsRecord(result, { 4, std::string("Record2"), (int64_t)400 }))
        << "Nie znaleziono rekordu {4, 'Record2', 400}";

    delete wal;
}

TEST(TableTests, GetRowsByBtreeWithIntegerKey) {
    // Test z kluczem typu int32_t
    Wal* wal = new Wal();
    wal->createDirectoryAndFile();
    Table table("testIntKeyTable", ".", wal);

    // Dodaj kolumny
    table.addColumn("id", int32_tId, false);
    table.addColumn("category", int32_tId, false);
    table.addColumn("name", stringId, false);

    // Dodaj rekordy - niektóre z t¹ sam¹ kategori¹
    table.addRecord({ 1, 10, std::string("Item1") });
    table.addRecord({ 2, 20, std::string("Item2") });
    table.addRecord({ 3, 10, std::string("Item3") }); // Ta sama kategoria co Item1
    table.addRecord({ 4, 30, std::string("Item4") });
    table.addRecord({ 5, 10, std::string("Item5") }); // Ta sama kategoria co Item1 i Item3

    // Dodaj indeks B-tree dla kolumny "category"
    table.addBtree("category");

    // Wyszukaj rekordy po kategorii 10
    std::vector<std::vector<allVars>> result = table.getRowsByBtree("category", 10);

    // SprawdŸ czy znaleziono dok³adnie 3 rekordy
    ASSERT_EQ(result.size(), 3) << "Powinny zostaæ znalezione 3 rekordy z kategori¹ 10";

    // SprawdŸ czy znalezione rekordy maj¹ odpowiednie wartoœci
    EXPECT_TRUE(containsRecord(result, { 1, 10, std::string("Item1") }));
    EXPECT_TRUE(containsRecord(result, { 3, 10, std::string("Item3") }));
    EXPECT_TRUE(containsRecord(result, { 5, 10, std::string("Item5") }));

    delete wal;
}

TEST(TableTests, GetRowsByBtreeEmptyResult) {
    // Test dla nieistniej¹cej wartoœci
    Wal* wal = new Wal();
    wal->createDirectoryAndFile();
    Table table("testEmptyResultTable", ".", wal);

    // Dodaj kolumny
    table.addColumn("id", int32_tId, false);
    table.addColumn("name", stringId, false);

    // Dodaj rekordy
    table.addRecord({ 1, std::string("Test1") });
    table.addRecord({ 2, std::string("Test2") });

    // Dodaj indeks B-tree dla kolumny "name"
    table.addBtree("name");

    // Wyszukaj rekordy po nieistniej¹cej nazwie
    std::vector<std::vector<allVars>> result = table.getRowsByBtree("name", std::string("NonExistent"));

    // SprawdŸ czy wynik jest pusty
    EXPECT_TRUE(result.empty()) << "Wynik powinien byæ pusty dla nieistniej¹cej wartoœci";

    delete wal;
}

TEST(TableTests, GetRowsByBtreeWithMultipleBlocks) {
    // Test z wieloma blokami danych
    Wal* wal = new Wal();
    wal->createDirectoryAndFile();
    Table table("testMultiBlockTable", ".", wal);

    // Dodaj kolumny
    table.addColumn("id", int32_tId, false);
    table.addColumn("group", int32_tId, false);
    table.addColumn("data", stringId, false); // Du¿e dane, aby wymusiæ wiele bloków

    // Dodaj du¿o rekordów, aby utworzyæ wiele bloków
    const int RECORDS_COUNT = 100;

    for (int i = 0; i < RECORDS_COUNT; i++) {
        int group = i % 5; // 5 ró¿nych grup (0-4)
        std::string largeData = "Data" + std::to_string(i) + std::string(300, 'X'); // Du¿e dane
        table.addRecord({ i, group, largeData });
    }

    // SprawdŸ, czy rzeczywiœcie utworzono wiele bloków
    ASSERT_GT(table.getDataBlocksSize(), 1) << "Test wymaga co najmniej 2 bloków danych";

    // Dodaj indeks B-tree dla kolumny "group"
    table.addBtree("group");

    // Wyszukaj rekordy z group=2
    std::vector<std::vector<allVars>> result = table.getRowsByBtree("group", 2);

    // Powinno byæ oko³o RECORDS_COUNT/5 rekordów z group=2
    int expectedCount = RECORDS_COUNT / 5;
    EXPECT_EQ(result.size(), expectedCount) << "Oczekiwano oko³o " << expectedCount << " rekordów z group=2";

    // SprawdŸ, czy wszystkie znalezione rekordy maj¹ group=2
    for (const auto& record : result) {
        ASSERT_GE(record.size(), 2) << "Rekord ma za ma³o elementów";
        EXPECT_EQ(std::get<int32_t>(record[1]), 2) << "Znaleziony rekord ma nieprawid³ow¹ wartoœæ grupy";
    }

    delete wal;
}

TEST(TableTests, GetRowsByBtreeForInt64) {
    // Test z kluczem typu int64_t
    Wal* wal = new Wal();
    wal->createDirectoryAndFile();
    Table table("testInt64KeyTable", ".", wal);

    // Dodaj kolumny
    table.addColumn("id", int32_tId, false);
    table.addColumn("bigValue", int64_tId, false);

    // Dodaj rekordy z du¿ymi liczbami
    int64_t largeValue1 = 9223372036854775807LL; // maksymalna wartoœæ int64_t
    int64_t largeValue2 = 9223372036854775806LL;

    table.addRecord({ 1, largeValue1 });
    table.addRecord({ 2, largeValue2 });
    table.addRecord({ 3, largeValue1 }); // duplikat wartoœci

    // Dodaj indeks B-tree dla kolumny "bigValue"
    table.addBtree("bigValue");

    // Wyszukaj rekordy po maksymalnej wartoœci int64_t
    std::vector<std::vector<allVars>> result = table.getRowsByBtree("bigValue", largeValue1);

    // SprawdŸ czy znaleziono 2 rekordy
    ASSERT_EQ(result.size(), 2) << "Powinny zostaæ znalezione 2 rekordy z maksymaln¹ wartoœci¹ int64_t";

    // SprawdŸ czy znalezione rekordy maj¹ odpowiednie wartoœci
    bool foundRecord1 = false, foundRecord3 = false;

    for (const auto& record : result) {
        ASSERT_GE(record.size(), 2) << "Rekord ma za ma³o elementów";
        EXPECT_EQ(std::get<int64_t>(record[1]), largeValue1) << "Znaleziony rekord ma nieprawid³ow¹ wartoœæ int64_t";

        if (std::get<int32_t>(record[0]) == 1) foundRecord1 = true;
        if (std::get<int32_t>(record[0]) == 3) foundRecord3 = true;
    }

    EXPECT_TRUE(foundRecord1) << "Nie znaleziono rekordu z id=1";
    EXPECT_TRUE(foundRecord3) << "Nie znaleziono rekordu z id=3";

    delete wal;
}

TEST(TableTests, GetRowsByBtreePerformance) {
    // Test wydajnoœci dla du¿ej liczby rekordów
    Wal* wal = new Wal();
    wal->createDirectoryAndFile();
    Table table("testPerformanceTable", ".", wal);

    // Dodaj kolumny
    table.addColumn("id", int32_tId, false);
    table.addColumn("key", int32_tId, false);
    table.addColumn("data", stringId, false);

    // Dodaj du¿¹ liczbê rekordów
    const int RECORDS_COUNT = 1000;
    const int TARGET_KEY = 42;
    int targetCount = 0;

    for (int i = 0; i < RECORDS_COUNT; i++) {
        int key = i % 100; // 100 ró¿nych kluczy
        std::string data = "Data" + std::to_string(i);

        table.addRecord({ i, key, data });

        if (key == TARGET_KEY) targetCount++;
    }

    // Dodaj indeks B-tree dla kolumny "key"
    auto startTime = std::chrono::high_resolution_clock::now();
    table.addBtree("key");
    auto endBuildTime = std::chrono::high_resolution_clock::now();

    // Wyszukaj rekordy z key=TARGET_KEY
    auto startSearchTime = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<allVars>> result = table.getRowsByBtree("key", TARGET_KEY);
    auto endSearchTime = std::chrono::high_resolution_clock::now();

    // Oblicz czasy
    std::chrono::duration<double> buildTime = endBuildTime - startTime;
    std::chrono::duration<double> searchTime = endSearchTime - startSearchTime;

    std::cout << "Czas budowania B-tree dla " << RECORDS_COUNT << " rekordów: "
        << buildTime.count() << " sekund" << std::endl;
    std::cout << "Czas wyszukiwania " << targetCount << " rekordów: "
        << searchTime.count() << " sekund" << std::endl;

    // SprawdŸ wyniki
    ASSERT_EQ(result.size(), targetCount) << "Znaleziono nieprawid³ow¹ liczbê rekordów";

    // SprawdŸ czy wszystkie znalezione rekordy maj¹ key=TARGET_KEY
    for (const auto& record : result) {
        ASSERT_GE(record.size(), 2) << "Rekord ma za ma³o elementów";
        EXPECT_EQ(std::get<int32_t>(record[1]), TARGET_KEY) << "Znaleziony rekord ma nieprawid³ow¹ wartoœæ klucza";
    }

    // Oczekujemy, ¿e wyszukiwanie z u¿yciem B-tree bêdzie szybkie
    EXPECT_LT(searchTime.count(), 0.1) << "Wyszukiwanie z B-tree powinno byæ szybkie (< 100ms)";

    delete wal;
}

TEST(TableTests, ConstructorWithValidParameters) {
    Wal* wal = nullptr; // Inicjalizacja wskaŸnika jako nullptr
    Table table("testTable", ".", wal);
    EXPECT_EQ(table.getTableName(), "testTable");
}

TEST(TableTests, ConstructorWithEmptyName) {
    Wal* wal = nullptr; // Inicjalizacja wskaŸnika jako nullptr
    Table table("", ".", wal);
    EXPECT_EQ(table.getTableName(), "");
}

TEST(TableTests, ConstructorWithLongName) {
    std::string longName(100, 'a');
    Wal* wal = nullptr; // Inicjalizacja wskaŸnika jako nullptr
    Table table(longName, ".", wal);
    EXPECT_EQ(table.getTableName(), longName);
}

TEST(TableTests, ConstructorWithSpecialChars) {
    std::string specialName = "test_special";
    Wal* wal = nullptr; // Inicjalizacja wskaŸnika jako nullptr
    Table table(specialName, ".", wal);
    EXPECT_EQ(table.getTableName(), specialName);
}

TEST(TableTests, GetTableNameTest) {
    Wal* wal = nullptr; // Inicjalizacja wskaŸnika jako nullptr
    {
        Table table("normalName", ".", wal);
        EXPECT_EQ(table.getTableName(), "normalName");
    }

    {
        Table table("", ".", wal);
        EXPECT_EQ(table.getTableName(), "");
    }

    {
        Table table("name with spaces", ".", wal);
        EXPECT_EQ(table.getTableName(), "name with spaces");
    }
}

TEST(TableTests, AddColumnBasicTypes) {
    // Najpierw resetujemy stdout
    std::cout << "test1" << std::endl;
    //ResetStdoutCapture();
    std::cout << "test1" << std::endl;
    Wal* wal = nullptr; // Inicjalizacja wskaŸnika jako nullptr
    
    Table table("columnTypesTest", ".", wal);



    EXPECT_NO_THROW(table.addColumn("intColumn", 1, false));
    EXPECT_NO_THROW(table.addColumn("stringColumn", 3, false));
    EXPECT_NO_THROW(table.addColumn("nullableIntColumn", 1, true));

    testing::internal::CaptureStdout();
    table.showTable();
    std::string output = testing::internal::GetCapturedStdout();

	std::cout << output << std::endl;

    EXPECT_TRUE(output.find("intColumn") != std::string::npos);
    EXPECT_TRUE(output.find("stringColumn") != std::string::npos);
    EXPECT_TRUE(output.find("nullableIntColumn") != std::string::npos);
}

TEST(TableTests, AddColumnWithEmptyName) {
    std::cout << "Rozpoczynam test AddColumnWithEmptyName..." << std::endl;
    Wal* wal = nullptr;
    Table table("emptyColumnTest", ".", wal);

#ifdef NDEBUG
    // W trybie Release asercje s¹ wy³¹czone
    EXPECT_NO_THROW({
        table.addColumn("", 1, false);

        testing::internal::CaptureStdout();
        table.showTable();
        std::string output = testing::internal::GetCapturedStdout();

        // Nie sprawdzamy konkretnej nazwy, bo mo¿e byæ pusta lub domyœlna
        EXPECT_TRUE(output.find("Table Name: emptyColumnTest") != std::string::npos)
            << "Tabela powinna istnieæ";
        });
#else
    // W trybie Debug asercja zatrzyma wykonanie - u¿ywamy EXPECT_DEATH
    EXPECT_DEATH({
        table.addColumn("", 1, false);
        }, "column string name is empty");

    // Poni¿szy kod nie bêdzie wykonany, gdy¿ asercja przerywa wykonanie,
    // ale mo¿emy dodaæ tabelê z poprawn¹ nazw¹ kolumny dla ukazania, 
    // ¿e test mo¿e kontynuowaæ wykonanie
    std::cout << "Po teœcie EXPECT_DEATH test nadal siê wykonuje" << std::endl;
#endif

    std::cout << "Zakoñczy³em test AddColumnWithEmptyName" << std::endl;
}

TEST(TableTests, AddColumnWithLongName) {
    //ResetStdoutCapture();
    Wal* wal = nullptr; // Inicjalizacja wskaŸnika jako nullptr
    Table table("longColumnTest", ".", wal);
    std::string longColumnName(100, 'b');
    EXPECT_NO_THROW(table.addColumn(longColumnName, 1, false));

    testing::internal::CaptureStdout();
    table.showTable();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find(longColumnName) != std::string::npos);
}

TEST(TableTests, MultipleColumnsOrdering) {
    //ResetStdoutCapture();
    Wal* wal = nullptr; // Inicjalizacja wskaŸnika jako nullptr
    Table table("orderingTest", ".", wal);

    table.addColumn("first", 1, false);
    table.addColumn("second", 2, false);
    table.addColumn("third", 3, false);

    testing::internal::CaptureStdout();
    table.showTable();
    std::string output = testing::internal::GetCapturedStdout();

    size_t firstPos = output.find("first");
    size_t secondPos = output.find("second");
    size_t thirdPos = output.find("third");

    ASSERT_NE(firstPos, std::string::npos);
    ASSERT_NE(secondPos, std::string::npos);
    ASSERT_NE(thirdPos, std::string::npos);

    EXPECT_LT(firstPos, secondPos);
    EXPECT_LT(secondPos, thirdPos);
}

TEST(TableTests, AddManyColumns) {
    //ResetStdoutCapture();
    Wal* wal = nullptr; // Inicjalizacja wskaŸnika jako nullptr
    Table table("manyColumnsTest", ".", wal);

    for (int i = 0; i < 20; i++) {
        std::string colName = "col" + std::to_string(i);
        EXPECT_NO_THROW(table.addColumn(colName, i % 3 + 1, i % 2 == 0));
    }

    testing::internal::CaptureStdout();
    table.showTable();
    std::string output = testing::internal::GetCapturedStdout();

    for (int i = 0; i < 20; i++) {
        std::string colName = "col" + std::to_string(i);
        EXPECT_TRUE(output.find(colName) != std::string::npos);
    }
}

TEST(TableTests, ShowTableEmpty) {
//ResetStdoutCapture();
    Wal* wal = nullptr; // Inicjalizacja wskaŸnika jako nullptr
    Table table("emptyTable", ".", wal);

    testing::internal::CaptureStdout();
    table.showTable();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Table Name: emptyTable") != std::string::npos);
    EXPECT_TRUE(output.find("Columns:") != std::string::npos);
}

TEST(TableTests, ShowTableWithColumns) {
    //ResetStdoutCapture();
    Wal* wal = nullptr; // Inicjalizacja wskaŸnika jako nullptr
    Table table("populatedTable", ".", wal);

    table.addColumn("id", 1, false);
    table.addColumn("name", 3, true);

    testing::internal::CaptureStdout();
    table.showTable();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Table Name: populatedTable") != std::string::npos);
    EXPECT_TRUE(output.find("id") != std::string::npos);
    EXPECT_TRUE(output.find("name") != std::string::npos);
    EXPECT_TRUE(output.find("Allow NULL: No") != std::string::npos);
    EXPECT_TRUE(output.find("Allow NULL: Yes") != std::string::npos);
}

TEST(TableTests, ClearAllEmptyTable) {
    Wal* wal = nullptr; // Inicjalizacja wskaŸnika jako nullptr
    Table* table = new Table("emptyTable", ".", wal);
    EXPECT_NO_THROW(table->clearAll());
    EXPECT_NO_THROW(delete table);
}

TEST(TableTests, ClearAllMultipleTimes) {
    Wal* wal = nullptr; // Inicjalizacja wskaŸnika jako nullptr
    Table* table = new Table("multiClearTable", ".", wal);
    table->addColumn("col1", 1, false);
    EXPECT_NO_THROW(table->clearAll());
    EXPECT_NO_THROW(table->clearAll());
    EXPECT_NO_THROW(delete table);
}
/*
TEST(TableTests, AddColumnsAfterClear) {
    //ResetStdoutCapture();
    
    Wal* wal = nullptr; // Inicjalizacja wskaŸnika jako nullptr
    Table table("clearAndAdd", ".", wal);
	std::cout << "test1" << std::endl;
    table.addColumn("col1", 1, false);
    std::cout << "test2" << std::endl;
    table.clearAll();

    EXPECT_NO_THROW(table.addColumn("col2", 2, true));

    testing::internal::CaptureStdout();
    table.showTable();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_FALSE(output.find("col1") != std::string::npos);
    EXPECT_TRUE(output.find("col2") != std::string::npos);
    
}
*/
TEST(TableTests, SameTableName) {
    //ResetStdoutCapture();
    Wal* wal = nullptr; // Inicjalizacja wskaŸnika jako nullptr
    Table table1("duplicateNameTest", "./dir1", wal);
    Table table2("duplicateNameTest", "./dir2", wal);

    EXPECT_EQ(table1.getTableName(), table2.getTableName());

    table1.addColumn("col1", 1, false);
    table2.addColumn("col2", 2, true);

    testing::internal::CaptureStdout();
    table1.showTable();
    std::string output1 = testing::internal::GetCapturedStdout();

    //ResetStdoutCapture();

    testing::internal::CaptureStdout();
    table2.showTable();
    std::string output2 = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output1.find("col1") != std::string::npos);
    EXPECT_FALSE(output1.find("col2") != std::string::npos);

    EXPECT_FALSE(output2.find("col1") != std::string::npos);
    EXPECT_TRUE(output2.find("col2") != std::string::npos);
}

TEST(TableTests, ColumnTypesValues) {
    //ResetStdoutCapture();
    Wal* wal = nullptr; // Inicjalizacja wskaŸnika jako nullptr
    Table table("typesTest", ".", wal);

    table.addColumn("normalType", 1, false);
    table.addColumn("maxType", INT32_MAX, false);
    table.addColumn("negType", -1, false);

    testing::internal::CaptureStdout();
    table.showTable();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Data Type: 1") != std::string::npos);
    EXPECT_TRUE(output.find("Data Type: 2147483647") != std::string::npos);
    EXPECT_TRUE(output.find("Data Type: -1") != std::string::npos);
}

TEST(TableTests, DifferentPaths) {
    // Wszystkie œcie¿ki bêd¹ podkatalogami "."
    Wal* wal = nullptr; // Inicjalizacja wskaŸnika jako nullptr
    Table table1("pathTest1", "./subdir1", wal);
    EXPECT_EQ(table1.getTableName(), "pathTest1");

    Table table2("pathTest2", "./subdir2", wal);
    EXPECT_EQ(table2.getTableName(), "pathTest2");

    Table table3("pathTest3", ".", wal);
    EXPECT_EQ(table3.getTableName(), "pathTest3");
}

//-------- bTree module tests

TEST(TableTests, AddBTreeAndGetBlockNum) {
    // Utworzenie obiektu Wal
    Wal* wal = new Wal();
    wal->createDirectoryAndFile();

    // Utworzenie tabeli
    Table table("testBTreeTable", ".", wal);

    // Dodanie kolumn - zmieniono float64Id na int64_tId
    table.addColumn("id", int32_tId, false);
    table.addColumn("name", stringId, false);
    table.addColumn("value", int64_tId, false);

    const int NUM_COLUMNS = 100;
    for (int i = 0; i < NUM_COLUMNS; i++) {
        std::string longColName = "column" + std::to_string(i) + std::string(500, 'X');
        table.addColumn(longColName, i % 3 + 1, i % 2 == 0);
		std::cout << "Dodano kolumnê: " << longColName << std::endl;
        //table.addColumn(tableName, longColName, i % 3 + 1, i % 2 == 0);
    }
    table.addBtree("name");
	table.getBlockNum("name", std::string("TestName"));
}


/*
// Test sprawdzaj¹cy dzia³anie addBtree dla nieistniej¹cej kolumny
TEST(TableTests, AddBTreeForNonExistentColumn) {
    Wal* wal = new Wal();
    wal->createDirectoryAndFile();

    Table table("testNonExistentColumn", ".", wal);

    // Dodajemy kolumnê "id"
    table.addColumn("id", int32_tId, false);

    // Próba dodania B-tree dla nieistniej¹cej kolumny
    testing::internal::CaptureStdout(); // Przechwytujemy wyjœcie konsoli
    table.addBtree("nonexistent");
    std::string output = testing::internal::GetCapturedStdout();

    // Sprawdzamy czy funkcja getBlockNum zwróci sensown¹ wartoœæ dla nieistniej¹cego indeksu
    // W zale¿noœci od implementacji mo¿e zwróciæ 0, -1 lub rzuciæ wyj¹tek
    try {
        int blockNum = table.getBlockNum("nonexistent", "someValue");
        // Jeœli nie rzuci wyj¹tku, oczekujemy jakiejœ domyœlnej wartoœci
        EXPECT_TRUE(blockNum == 0 || blockNum == -1) << "Dla nieistniej¹cej kolumny getBlockNum powinien zwróciæ domyœln¹ wartoœæ";
    }
    catch (const std::exception& e) {
        // Jeœli rzuci wyj¹tek, to równie¿ akceptujemy
        SUCCEED() << "Funkcja getBlockNum rzuci³a wyj¹tek dla nieistniej¹cej kolumny: " << e.what();
    }

    delete wal;
}

// Test sprawdzaj¹cy dzia³anie getBlockNum dla ró¿nych typów danych
TEST(TableTests, GetBlockNumForDifferentTypes) {
    Wal* wal = new Wal();
    wal->createDirectoryAndFile();

    Table table("testMultipleTypes", ".", wal);

    // Dodanie kolumn ró¿nych typów - zmieniono float64Id na int64_tId
    table.addColumn("id", int32_tId, false);
    table.addColumn("name", stringId, false);
    table.addColumn("value", int64_tId, false);

    // Dodanie rekordów
    for (int i = 1; i <= 30; i++) {
        std::string name = "Record-" + std::to_string(i);
        table.addRecord({ i, name, (int64_t)(i * 100) });  // Zmieniono typ wartoœci na int64_t
    }

    // Dodanie indeksu B-tree dla wszystkich kolumn
    table.addBtree("id");
    table.addBtree("name");
    table.addBtree("value");

    // Sprawdzenie getBlockNum dla wartoœci ró¿nych typów
    int intBlockNum = table.getBlockNum("id", 15);
    int stringBlockNum = table.getBlockNum("name", std::string("Record-15"));
    int doubleBlockNum = table.getBlockNum("value", (int64_t)(15 * 100));  // Zmieniono typ wartoœci na int64_t

    // Powinny wszystkie wskazywaæ na ten sam blok, poniewa¿ rekord jest tylko jeden
    EXPECT_EQ(intBlockNum, stringBlockNum) << "getBlockNum powinien zwróciæ ten sam numer bloku dla ró¿nych kolumn tego samego rekordu";
    EXPECT_EQ(stringBlockNum, doubleBlockNum) << "getBlockNum powinien zwróciæ ten sam numer bloku dla ró¿nych kolumn tego samego rekordu";

    delete wal;
}

// Test sprawdzaj¹cy odpornoœæ na ekstremalne przypadki
TEST(TableTests, BTreeEdgeCases) {
    Wal* wal = new Wal();
    wal->createDirectoryAndFile();

    Table table("testEdgeCases", ".", wal);

    // Dodanie kolumn
    table.addColumn("id", int32_tId, false);
    table.addColumn("name", stringId, false);

    // Test pustej tabeli
    ASSERT_NO_THROW(table.addBtree("id")) << "addBtree powinien obs³ugiwaæ pust¹ tabelê";

    // Dodanie pojedynczego rekordu
    table.addRecord({ 1, "SingleRecord" });

    // Dodanie B-tree dla kolumny z jednym rekordem
    ASSERT_NO_THROW(table.addBtree("name")) << "addBtree powinien obs³ugiwaæ tabelê z pojedynczym rekordem";

    // Sprawdzenie getBlockNum dla istniej¹cego elementu
    int blockNum = table.getBlockNum("name", std::string("SingleRecord"));
    EXPECT_GE(blockNum, 0) << "getBlockNum powinien zwróciæ prawid³owy numer bloku dla istniej¹cego elementu";

    // Sprawdzenie getBlockNum dla nieistniej¹cego elementu
    int nonExistentBlockNum = table.getBlockNum("name", std::string("NonExistent"));
    // W zale¿noœci od implementacji mo¿e zwróciæ 0, -1 lub inn¹ wartoœæ wskazuj¹c¹ na brak elementu

    delete wal;
}

// Test sprawdzaj¹cy wydajnoœæ dla du¿ych iloœci danych
TEST(TableTests, BTreePerformance) {
    Wal* wal = new Wal();
    wal->createDirectoryAndFile();

    Table table("testPerformance", ".", wal);

    // Dodanie kolumn
    table.addColumn("id", int32_tId, false);
    table.addColumn("value", stringId, false);

    const int RECORD_COUNT = 500; // Du¿a liczba rekordów

    // Dodanie rekordów
    for (int i = 0; i < RECORD_COUNT; i++) {
        std::string value = "Value-" + std::to_string(i) + "-" + std::string(20, 'X');
        table.addRecord({ i, value });
    }

    // Zmierzenie czasu tworzenia B-tree
    auto startCreate = std::chrono::high_resolution_clock::now();
    table.addBtree("value");
    auto endCreate = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> createTime = endCreate - startCreate;

    std::cout << "Czas tworzenia B-tree dla " << RECORD_COUNT << " rekordów: "
        << createTime.count() << " sekund" << std::endl;

    // Zmierzenie czasu wyszukiwania
    auto startSearch = std::chrono::high_resolution_clock::now();
    int blockNum = table.getBlockNum("value", std::string("Value-250-XXXXXXXXXXXXXXXXXXXX"));
    auto endSearch = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> searchTime = endSearch - startSearch;

    std::cout << "Czas wyszukiwania w B-tree: " << searchTime.count() << " sekund" << std::endl;

    // Sprawdzamy czy wyszukiwanie by³o szybkie (poni¿ej 1ms)
    EXPECT_LT(searchTime.count(), 0.001) << "Wyszukiwanie w B-tree powinno byæ szybkie";

    delete wal;
}
*/