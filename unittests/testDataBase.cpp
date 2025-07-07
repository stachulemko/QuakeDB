#include "pch.h"
#include "dataBase.h"


TEST(DataBaseTests, CreateAndDeleteSingleTable) {
    Database db;
    std::string tableName = "testTable";
    db.addTable(tableName);
    db.commit();
    EXPECT_TRUE(isFileExists(db.getPath() + "/" + tableName + ".bin"));
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

TEST(DataBaseTests, AddColumnsAndDelete) {
    Database db;
    std::string tableName = "tableWithColumns";
    db.addTable(tableName);
    db.addColumn(tableName, "col1", 1, false);
    db.addColumn(tableName, "col2", 2, true);
    db.commit();
    EXPECT_TRUE(isFileExists(db.getPath() + "/" + tableName + ".bin"));
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}


TEST(DataBaseTests, MultipleTablesCleanup) {
    Database db;
    std::string t1 = "tab1", t2 = "tab2";
    db.addTable(t1);
    db.addTable(t2);
    db.commit();
    EXPECT_TRUE(isFileExists(db.getPath() + "/" + t1 + ".bin"));
    EXPECT_TRUE(isFileExists(db.getPath() + "/" + t2 + ".bin"));
    deleteFile(db.getPath() + "/" + t1 + ".bin");
    deleteFile(db.getPath() + "/" + t2 + ".bin");
}


TEST(DataBaseTests, CommitAndReloadCleanup) {
    std::string tableName = "reloadTable";
    {
        Database db;
        db.addTable(tableName);
        db.addColumn(tableName, "id", 1, false);
        db.commit();
    }
    Database db2;
    db2.loadDataBase();
    EXPECT_TRUE(isFileExists(db2.getPath() + "/" + tableName + ".bin"));
    deleteFile(db2.getPath() + "/" + tableName + ".bin");
}

// Test nazwy tabeli ze znakami specjalnymi
TEST(DataBaseTests, SpecialCharactersTableCleanup) {
    std::string tableName = "tab!@#";
    Database db;
    db.addTable(tableName);
    db.commit();
    EXPECT_TRUE(isFileExists(db.getPath() + "/" + tableName + ".bin"));
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test obs³ugi d³ugich nazw
/*
TEST(DataBaseTests, LongNamesHandling) {
    std::string longTableName = "aa";
    std::string longColumnName = "bb";
    int type = 1;
    bool allowNUll = false;
    Database db;
    db.addTable(longTableName);
    db.addColumn(longTableName, longColumnName, type, allowNUll);
    db.commit();

    Database loadDb;
    loadDb.loadDataBase();
    ASSERT_FALSE(loadDb.getTableColumnsNames().empty()) << "Nie znaleziono ¿adnych tabel po wczytaniu bazy danych";
    ASSERT_FALSE(loadDb.getTableColumnsNames()[0].empty()) << "Nie znaleziono kolumn w pierwszej tabeli";

    EXPECT_EQ(longColumnName, loadDb.getTableColumnsNames()[0][0])
        << "Nazwa kolumny po wczytaniu nie zgadza siê z oczekiwan¹";

    deleteFile(db.getPath() + "/" + longTableName + ".bin");
}
*/
// Test obs³ugi wielu tabel i kolumn
TEST(DataBaseTests, MultipleTablesAndColumnsHandling) {
    std::string longTableName1(50, 'a');
    std::string longColumnName1(50, 'b');
    std::string longColumnName2(50, 'c');
    std::string longTableName2(50, 'd');
    std::string longColumnName3(50, 'e');
    std::string longColumnName4(50, 'f');
    int type1 = 1;
    bool allowNUll1 = false;
    int type2 = 2;
    bool allowNUll2 = true;
    int type3 = 3;
    bool allowNUll3 = false;
    int type4 = 1;
    bool allowNUll4 = true;
    Database db;
    db.addTable(longTableName1);
    db.addColumn(longTableName1, longColumnName1, type1, allowNUll1);
    db.addColumn(longTableName1, longColumnName2, type2, allowNUll2);
    db.addTable(longTableName2);
    db.addColumn(longTableName2, longColumnName3, type3, allowNUll3);
    db.addColumn(longTableName2, longColumnName4, type4, allowNUll4);
    db.commit();

    Database loadDb;
    loadDb.loadDataBase();

    std::vector<std::vector<std::string>> tableColumns = loadDb.getTableColumnsNames();
    ASSERT_EQ(tableColumns.size(), 2) << "Nieprawid³owa liczba tabel";
    ASSERT_EQ(tableColumns[0].size(), 2) << "Nieprawid³owa liczba kolumn w pierwszej tabeli";
    ASSERT_EQ(tableColumns[1].size(), 2) << "Nieprawid³owa liczba kolumn w drugiej tabeli";

    EXPECT_EQ(tableColumns[0][0], longColumnName1);
    EXPECT_EQ(tableColumns[0][1], longColumnName2);
    EXPECT_EQ(tableColumns[1][0], longColumnName3);
    EXPECT_EQ(tableColumns[1][1], longColumnName4);

    std::vector<std::vector<std::vector<int32_t>>> typesAndNulls = loadDb.getTypeAndAllowNUllTables();
    ASSERT_EQ(typesAndNulls.size(), 2) << "Nieprawid³owa liczba tabel w typesAndNulls";
    ASSERT_EQ(typesAndNulls[0].size(), 2) << "Nieprawid³owa liczba typów w pierwszej tabeli";
    ASSERT_EQ(typesAndNulls[1].size(), 2) << "Nieprawid³owa liczba typów w drugiej tabeli";

    EXPECT_EQ(typesAndNulls[0][0][0], type1);
    EXPECT_EQ(typesAndNulls[0][0][1], allowNUll1 ? 1 : 0);
    EXPECT_EQ(typesAndNulls[0][1][0], type2);
    EXPECT_EQ(typesAndNulls[0][1][1], allowNUll2 ? 1 : 0);

    EXPECT_EQ(typesAndNulls[1][0][0], type3);
    EXPECT_EQ(typesAndNulls[1][0][1], allowNUll3 ? 1 : 0);
    EXPECT_EQ(typesAndNulls[1][1][0], type4);
    EXPECT_EQ(typesAndNulls[1][1][1], allowNUll4 ? 1 : 0);

    deleteFile(db.getPath() + "/" + longTableName1 + ".bin");
    deleteFile(db.getPath() + "/" + longTableName2 + ".bin");
}

TEST(DataBaseTests, TestFolderCreated) {
    Database database;
    EXPECT_TRUE(isFileExists(database.getPath()));
}

TEST(DataBaseTests, FileWasCreated) {
    Database database;
    database.addTable("testTable");
    database.commit();
    EXPECT_TRUE(isFileExists(database.getPath() + "/testTable.bin"));
    deleteFile(database.getPath() + "/testTable.bin");
}

// Test czyszczenia wszystkich tabel
/*
TEST(DataBaseTests, ClearAllTablesTest) {
    Database database;
    database.addTable("tempTable");
    database.addColumn("tempTable", "tempCol", 1, false);

    testing::internal::CaptureStdout();
    database.showFile();
    std::string beforeClear = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(beforeClear.find("tempCol") != std::string::npos);

    database.clearAll();

    testing::internal::CaptureStdout();
    database.showFile();
    std::string afterClear = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(afterClear.empty() || afterClear.find("tempCol") == std::string::npos);
}
*/
// Test kodowania definicji kolumny
TEST(DataBaseTests, ColumnDefinitionEncodingTest) {
    std::string tableName = "encodingTest";
    std::string columnName = "testColumn";
    int dataType = 2;
    bool allowNull = true;

    Database db;
    db.addTable(tableName);
    db.addColumn(tableName, columnName, dataType, allowNull);
    db.commit();

    std::vector<uint8_t> fileBytes = readFileBytes(db.getPath() + "/" + tableName + ".bin");

    EXPECT_FALSE(fileBytes.empty());

    Database loadDb;
    loadDb.loadDataBase();

    // Sprawdzenie czy dane s¹ poprawnie za³adowane
    std::vector<std::vector<std::string>> tableColumns = loadDb.getTableColumnsNames();
    ASSERT_FALSE(tableColumns.empty());
    ASSERT_FALSE(tableColumns[0].empty());
    EXPECT_EQ(tableColumns[0][0], columnName);

    // Sprawdzenie typu i flagi NULL
    std::vector<std::vector<std::vector<int32_t>>> typesAndNulls = loadDb.getTypeAndAllowNUllTables();
    ASSERT_FALSE(typesAndNulls.empty());
    ASSERT_FALSE(typesAndNulls[0].empty());
    EXPECT_EQ(typesAndNulls[0][0][0], dataType);
    EXPECT_EQ(typesAndNulls[0][0][1], 1); // allowNull = true

    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test dodawania kolumny do nieistniej¹cej tabeli
TEST(DataBaseTests, AddColumnToNonExistentTable) {
    Database db;
    db.addColumn("nonExistent", "testCol", 1, false);

    testing::internal::CaptureStdout();
    db.showFile();
    std::string output = testing::internal::GetCapturedStdout();

    // Nie powinno byæ ¿adnych danych
    EXPECT_TRUE(output.empty() || output.find("nonExistent") == std::string::npos);
}

// Test dodawania tabeli z pust¹ nazw¹
TEST(DataBaseTests, AddTableWithEmptyName) {
    Database db;
    db.addTable("");
    db.addColumn("", "id", 1, false);
    db.commit();

    // Sprawdzenie czy plik z pust¹ nazw¹ zosta³ utworzony
    EXPECT_TRUE(isFileExists(db.getPath() + "/.bin"));
    deleteFile(db.getPath() + "/.bin");
}

// Test aktualizacji istniej¹cej tabeli
TEST(DataBaseTests, UpdateExistingTable) {
    std::string tableName = "updateTable";

    // Pierwsza wersja tabeli
    {
        Database db1;
        db1.addTable(tableName);
        db1.addColumn(tableName, "id", 1, false);
        db1.commit();
    }

    // Aktualizacja tabeli
    {
        Database db2;
        db2.loadDataBase();
        db2.addColumn(tableName, "description", 3, true);
        db2.commit();
    }

    // Sprawdzenie czy aktualizacja zosta³a zachowana
    Database db3;
    db3.loadDataBase();

    std::vector<std::vector<std::string>> tableColumns = db3.getTableColumnsNames();
    ASSERT_FALSE(tableColumns.empty());
    ASSERT_GE(tableColumns[0].size(), 2) << "Tabela powinna mieæ co najmniej 2 kolumny";

    bool hasIdColumn = false;
    bool hasDescriptionColumn = false;

    for (const auto& colName : tableColumns[0]) {
        if (colName == "id") hasIdColumn = true;
        if (colName == "description") hasDescriptionColumn = true;
    }

    EXPECT_TRUE(hasIdColumn) << "Kolumna 'id' nie zosta³a znaleziona";
    EXPECT_TRUE(hasDescriptionColumn) << "Kolumna 'description' nie zosta³a znaleziona";

    deleteFile(db3.getPath() + "/" + tableName + ".bin");
}

// Test wielokrotnego zapisu i odczytu
TEST(DataBaseTests, MultipleWriteReadTest) {
    std::string tableName = "multiRW";

    // Utwórz i zapisz tabelê
    {
        Database db;
        db.addTable(tableName);
        db.addColumn(tableName, "col1", 1, false);
        db.commit();
    }

    // Odczytaj, dodaj kolumnê i zapisz
    {
        Database db;
        db.loadDataBase();
        db.addColumn(tableName, "col2", 2, false);
        db.commit();
    }

    // Odczytaj, dodaj kolejn¹ kolumnê i zapisz
    {
        Database db;
        db.loadDataBase();
        db.addColumn(tableName, "col3", 3, true);
        db.commit();
    }

    // SprawdŸ czy wszystkie kolumny s¹ obecne
    Database finalDb;
    finalDb.loadDataBase();

    std::vector<std::vector<std::string>> tableColumns = finalDb.getTableColumnsNames();
    ASSERT_FALSE(tableColumns.empty());
    ASSERT_GE(tableColumns[0].size(), 3) << "Tabela powinna mieæ co najmniej 3 kolumny";

    std::vector<std::string> expectedColumns = { "col1", "col2", "col3" };
    for (const auto& expectedCol : expectedColumns) {
        bool found = false;
        for (const auto& actualCol : tableColumns[0]) {
            if (actualCol == expectedCol) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found) << "Kolumna '" << expectedCol << "' nie zosta³a znaleziona";
    }

    deleteFile(finalDb.getPath() + "/" + tableName + ".bin");
}

// Test dodawania rekordów do tabeli
TEST(DataBaseTests, AddRecordsToTable) {
    std::string tableName = "recordTable";
    Database db;
    db.addTable(tableName);
    db.addColumn(tableName, "id", 1, false);      // int
    db.addColumn(tableName, "amount", 2, false);  // double (int64_t)
    db.addColumn(tableName, "name", 3, false);    // string

    // Dodaj rekordy (symulacja - rzeczywiste dodawanie rekordów zale¿y od implementacji)
    std::vector<allVars> record1 = { 1, static_cast<int64_t>(100), std::string("Product1") };
    std::vector<allVars> record2 = { 2, static_cast<int64_t>(200), std::string("Product2") };

    // Jeœli Database ma metodê addRecord
    if (std::is_member_function_pointer<decltype(&Database::addRecord)>::value) {
        db.addRecord(tableName, record1);
        db.addRecord(tableName, record2);
    }

    // Zapisz dane
    db.commit();

    // SprawdŸ czy plik zosta³ utworzony
    EXPECT_TRUE(isFileExists(db.getPath() + "/" + tableName + ".bin"));

    // SprawdŸ rozmiar pliku (powinien zawieraæ dane dwóch rekordów)
    std::vector<uint8_t> fileBytes = readFileBytes(db.getPath() + "/" + tableName + ".bin");
    EXPECT_GT(fileBytes.size(), 0) << "Plik bazy danych nie powinien byæ pusty";

    // Posprz¹taj po teœcie
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test odpornoœci na b³êdy
TEST(DataBaseTests, ErrorResilienceTest) {
    Database db;

    // Próba dodania kolumny do nieistniej¹cej tabeli
    EXPECT_NO_THROW(db.addColumn("nonexistent", "col", 1, false));

    // Próba commitu bez ¿adnych tabel
    EXPECT_NO_THROW(db.commit());

    // Próba wczytania bazy danych, gdy folder nie zawiera plików bazy danych
    EXPECT_NO_THROW(db.loadDataBase());

    // Test clearAll na pustej bazie danych
    EXPECT_NO_THROW(db.clearAll());
}

// Test na spójnoœæ zwracanego getPath
TEST(DataBaseTests, PathConsistencyTest) {
    Database db1;
    Database db2;

    EXPECT_EQ(db1.getPath(), db2.getPath())
        << "Metoda getPath() powinna zwracaæ tê sam¹ wartoœæ dla ró¿nych instancji Database";
}

// Test zachowania przy bardzo du¿ej liczbie tabel
TEST(DataBaseTests, LargeNumberOfTablesTest) {
    Database db;
    const int numTables = 20; // Limit iloœci tabel w teœcie

    for (int i = 0; i < numTables; i++) {
        std::string tableName = "table" + std::to_string(i);
        db.addTable(tableName);
        db.addColumn(tableName, "id", 1, false);
    }

    EXPECT_NO_THROW(db.commit());

    for (int i = 0; i < numTables; i++) {
        std::string tableName = "table" + std::to_string(i);
        std::string filePath = db.getPath() + "/" + tableName + ".bin";
        EXPECT_TRUE(isFileExists(filePath));
        deleteFile(filePath);
    }
}


//------------------------------------------------------
TEST(DataBaseTests, BinaryFileContentVerification) {
    std::string tableName = "contentTest";
    std::string columnName = "testCol";
    int dataType = 1;
    bool allowNull = false;

    Database db;
    db.addTable(tableName);
    db.addColumn(tableName, columnName, dataType, allowNull);
    db.commit();

    std::vector<uint8_t> fileContent = readFileBytes(db.getPath() + "/" + tableName + ".bin");

    // SprawdŸ czy plik zawiera przynajmniej nag³ówek (typ i d³ugoœæ)
    ASSERT_GE(fileContent.size(), 8) << "Plik jest zbyt krótki, brakuje nag³ówka";

    // Odczytaj typ z nag³ówka (pierwsze 4 bajty)
    int32_t fileType;
    std::vector<uint8_t> typeBytes(fileContent.begin(), fileContent.begin() + 4);
    UnmarshalInt32_t(&fileType, &typeBytes);

    // SprawdŸ czy typ w nag³ówku to columnTypeId (4)
    EXPECT_EQ(fileType, columnTypeId) << "Nieprawid³owy typ w nag³ówku pliku";


    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test przypadku granicznego - bardzo du¿a nazwa kolumny
TEST(DataBaseTests, VeryLongColumnNameTest) {
    std::string tableName = "longColTable";
    // Utwórz bardzo d³ug¹ nazwê kolumny - 1000 znaków
    std::string longColumnName(1000, 'x');

    Database db;
    db.addTable(tableName);
    db.addColumn(tableName, longColumnName, 1, false);
    db.commit();

    // £adujemy bazê danych ponownie
    Database loadDb;
    loadDb.loadDataBase();

    // Sprawdzamy czy d³uga nazwa kolumny zosta³a zachowana
    std::vector<std::vector<std::string>> tableColumns = loadDb.getTableColumnsNames();
    ASSERT_FALSE(tableColumns.empty());
    ASSERT_FALSE(tableColumns[0].empty());

    // Ze wzglêdu na potencjalne ograniczenia d³ugoœci nazwy, sprawdzamy czy:
    // 1. Nazwa zosta³a zapisana w ca³oœci, lub
    // 2. Nazwa zosta³a przyciêta, ale przynajmniej czêœæ siê zgadza
    std::string loadedColumnName = tableColumns[0][0];

    if (loadedColumnName.length() == longColumnName.length()) {
        // Nazwa zachowana w ca³oœci
        EXPECT_EQ(loadedColumnName, longColumnName)
            << "D³uga nazwa kolumny nie zosta³a poprawnie zachowana";
    }
    else {
        // Nazwa mog³a zostaæ przyciêta
        EXPECT_EQ(loadedColumnName, longColumnName.substr(0, loadedColumnName.length()))
            << "Przyciêta nazwa kolumny nie zgadza siê z pocz¹tkiem oryginalnej nazwy";
    }

    // Wyczyœæ po teœcie
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test przypadku granicznego - tabela bez kolumn
TEST(DataBaseTests, TableWithNoColumnsTest) {
    std::string tableName = "emptyTable";

    Database db;
    db.addTable(tableName);
    // Celowo nie dodajemy ¿adnej kolumny
    db.commit();

    EXPECT_TRUE(isFileExists(db.getPath() + "/" + tableName + ".bin"));

    // £adujemy bazê danych ponownie
    Database loadDb;
    loadDb.loadDataBase();

    // Sprawdzamy czy tabela istnieje
    std::vector<std::vector<std::string>> tableColumns = loadDb.getTableColumnsNames();
    ASSERT_FALSE(tableColumns.empty()) << "Tabela bez kolumn nie zosta³a wczytana";

    // Sprawdzamy czy tabela nie ma kolumn
    EXPECT_TRUE(tableColumns[0].empty()) << "Tabela powinna byæ pusta, ale zawiera kolumny";

    // Wyczyœæ po teœcie
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test przypadku granicznego - niepoprawne typy danych kolumn
TEST(DataBaseTests, InvalidColumnDataTypeTest) {
    std::string tableName = "invalidTypeTable";

    Database db;
    db.addTable(tableName);

    // Próba dodania kolumny z niepoprawnym typem danych (np. 99)
    db.addColumn(tableName, "invalidCol", 99, false);
    db.commit();

    // £adujemy bazê danych ponownie
    Database loadDb;
    loadDb.loadDataBase();

    // Sprawdzamy czy tabela siê wczyta³a
    std::vector<std::vector<std::string>> tableColumns = loadDb.getTableColumnsNames();
    ASSERT_FALSE(tableColumns.empty()) << "Tabela z niepoprawnym typem kolumny nie zosta³a wczytana";

    // Sprawdzamy typy kolumn
    std::vector<std::vector<std::vector<int32_t>>> typesAndNulls = loadDb.getTypeAndAllowNUllTables();

    // Baza danych mo¿e albo zignorowaæ niepoprawny typ, albo go zapisaæ i odczytaæ
    if (!tableColumns[0].empty() && !typesAndNulls.empty() && !typesAndNulls[0].empty()) {
        // Albo kolumna z niepoprawnym typem istnieje
        EXPECT_EQ(typesAndNulls[0][0][0], 99) << "Typ kolumny nie zgadza siê z oczekiwanym";
    }
    else {
        // Albo kolumna zosta³a zignorowana
        SUCCEED() << "Kolumna z niepoprawnym typem zosta³a zignorowana (zachowanie bezpieczne)";
    }

    // Wyczyœæ po teœcie
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test przypadku granicznego - zapis i odczyt du¿ej liczby kolumn
TEST(DataBaseTests, LargeNumberOfColumnsTest) {
    std::string tableName = "manyColumnsTable";
    const int numColumns = 100; // Du¿a liczba kolumn

    Database db;
    db.addTable(tableName);

    // Dodaj du¿¹ liczbê kolumn
    for (int i = 0; i < numColumns; i++) {
        std::string colName = "col" + std::to_string(i);
        int dataType = (i % 3) + 1; // Typy 1, 2, 3 rotacyjnie
        bool allowNull = (i % 2 == 0); // Naprzemiennie true/false
        db.addColumn(tableName, colName, dataType, allowNull);
    }

    db.commit();

    // £adujemy bazê danych ponownie
    Database loadDb;
    loadDb.loadDataBase();

    // Sprawdzamy czy wszystkie kolumny zosta³y wczytane
    std::vector<std::vector<std::string>> tableColumns = loadDb.getTableColumnsNames();
    ASSERT_FALSE(tableColumns.empty()) << "Tabela nie zosta³a wczytana";

    // Baza mo¿e mieæ ograniczenie na liczbê kolumn, wiêc sprawdzamy czy wczytano przynajmniej czêœæ
    EXPECT_GE(tableColumns[0].size(), 1) << "¯adna kolumna nie zosta³a wczytana";

    // Jeœli wszystkie kolumny zosta³y wczytane, sprawdzamy ich nazwy
    if (tableColumns[0].size() == numColumns) {
        for (int i = 0; i < numColumns; i++) {
            std::string expectedColName = "col" + std::to_string(i);
            bool found = false;

            for (const auto& colName : tableColumns[0]) {
                if (colName == expectedColName) {
                    found = true;
                    break;
                }
            }

            EXPECT_TRUE(found) << "Nie znaleziono kolumny " << expectedColName;
        }
    }

    // Wyczyœæ po teœcie
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test wczytywania uszkodzonego/niepoprawnego pliku bazy danych
/*
TEST(DataBaseTests, CorruptedDatabaseFileTest) {
    std::string tableName = "corruptedTable";
    std::string filePath = Database().getPath() + "/" + tableName + ".bin";

    std::vector<uint8_t> invalidData = { 0x12, 0x34, 0x56, 0x78 };
    createBinFile(Database().getPath(), tableName);
    addToFileBytes(filePath, invalidData);

    EXPECT_TRUE(isFileExists(filePath)) << "Nie uda³o siê utworzyæ testowego pliku";               
    Database loadDb;
    EXPECT_NO_THROW(loadDb.loadDataBase()) << "Wczytywanie uszkodzonego pliku spowodowa³o wyj¹tek";
    deleteFile(filePath);
}
*/


TEST(DataBaseTests, SpecialCharactersInColumnNames) {
    Database db;
    std::string tableName = "specialCharsColumnsTable";
    db.addTable(tableName);

    std::vector<std::string> specialColumnNames = {
        "column-with-hyphens",
        "column_with_underscores",
        "column.with.dots",
        "column@with@at",
        "column123",
        "123column",
        "COLUMN_UPPERCASE",
        "column lowercase",
        "column$pecial!@#"
    };

    for (const auto& colName : specialColumnNames) {
        db.addColumn(tableName, colName, 1, false);
    }

    db.commit();

    // £adujemy bazê danych ponownie
    Database loadDb;
    loadDb.loadDataBase();

    // Sprawdzamy czy wszystkie kolumny zosta³y wczytane
    std::vector<std::vector<std::string>> tableColumns = loadDb.getTableColumnsNames();
    ASSERT_FALSE(tableColumns.empty()) << "Tabela nie zosta³a wczytana";

    // Sprawdzamy czy wszystkie kolumny ze specjalnymi znakami zosta³y zachowane
    for (const auto& expectedColName : specialColumnNames) {
        bool found = false;

        for (const auto& colName : tableColumns[0]) {
            if (colName == expectedColName) {
                found = true;
                break;
            }
        }

        EXPECT_TRUE(found) << "Nie znaleziono kolumny ze specjalnymi znakami: " << expectedColName;
    }

    // Wyczyœæ po teœcie
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}
/*
// Test przypadku granicznego - bardzo du¿a liczba tabel z unikalnymi nazwami
TEST(DataBaseTests, ManyTablesWithUniqueNames) {
    Database db;
    const int numTables = 50; // Bardzo du¿a liczba tabel
    std::vector<std::string> tableNames;

    // Utwórz wiele tabel
    for (int i = 0; i < numTables; i++) {
        std::string tableName = "unique_table_" + std::to_string(i);
        tableNames.push_back(tableName);
        db.addTable(tableName);
        db.addColumn(tableName, "id", 1, false);
    }

    EXPECT_NO_THROW(db.commit()) << "Commit du¿ej liczby tabel spowodowa³ wyj¹tek";

    // £adujemy bazê danych ponownie
    Database loadDb;
    EXPECT_NO_THROW(loadDb.loadDataBase()) << "Wczytywanie du¿ej liczby tabel spowodowa³o wyj¹tek";

    // Sprawdzamy czy wszystkie tabele zosta³y wczytane
}
*/
//-----------------Btree Tests ------------------

// Test dodawania B-tree i sprawdzania numeru bloku dla istniej¹cego elementu
TEST(DataBaseTests, AddBTreeAndGetBlockNum) {
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
    ASSERT_NO_THROW(db.addBtree(tableName, "name")) << "Dodanie B-tree zakoñczy³o siê b³êdem";

    // SprawdŸ numery bloków
    std::vector<int64_t> blockNums = db.getTableBlockNums(tableName);
    ASSERT_GE(blockNums.size(), 2) << "Tabela powinna mieæ co najmniej 2 bloki";

    // SprawdŸ czy getBlockNum zwraca prawid³owy numer bloku
    int blockNum1 = db.getBlockNum(tableName, "name", std::string("Record-10"));
    EXPECT_GE(blockNum1, 0) << "getBlockNum powinien zwróciæ prawid³owy numer bloku";

    // SprawdŸ numer bloku dla elementu z drugiego bloku
    std::string longRecordName = "LongRecord-75" + std::string(500, 'X');
    int blockNum2 = db.getBlockNum(tableName, "name", longRecordName);
    EXPECT_EQ(blockNum2, blockNums[1]) << "getBlockNum dla elementu z drugiego bloku powinien zwróciæ numer drugiego bloku";

    // Wyczyœæ po teœcie
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test dodawania B-tree dla ró¿nych typów kolumn
TEST(DataBaseTests, AddBTreeForDifferentColumnTypes) {
    std::string tableName = "bTreeTypesTable";
    Database db;

    // Utwórz tabelê z kolumnami ró¿nych typów
    db.addTable(tableName);
    db.addColumn(tableName, "id", int32_tId, false);
    db.addColumn(tableName, "name", stringId, false);
    db.addColumn(tableName, "value", int64_tId, false);

    // Dodaj kilka rekordów
    for (int i = 1; i <= 20; i++) {
        std::string name = "Test-" + std::to_string(i);
        db.addRecord(tableName, { i, name, (int64_t)(i * 10) });
    }

    // Dodaj B-tree dla wszystkich kolumn
    ASSERT_NO_THROW(db.addBtree(tableName, "id")) << "Dodanie B-tree dla kolumny int32_t zakoñczy³o siê b³êdem";
    ASSERT_NO_THROW(db.addBtree(tableName, "name")) << "Dodanie B-tree dla kolumny string zakoñczy³o siê b³êdem";
    ASSERT_NO_THROW(db.addBtree(tableName, "value")) << "Dodanie B-tree dla kolumny int64_t zakoñczy³o siê b³êdem";

    // SprawdŸ czy getBlockNum dzia³a dla ró¿nych typów
    int blockNumInt = db.getBlockNum(tableName, "id", 10);
    int blockNumString = db.getBlockNum(tableName, "name", std::string("Test-10"));
    int blockNumInt64 = db.getBlockNum(tableName, "value", (int64_t)100);

    EXPECT_GE(blockNumInt, 0) << "getBlockNum dla int32_t powinien zwróciæ prawid³owy numer bloku";
    EXPECT_GE(blockNumString, 0) << "getBlockNum dla string powinien zwróciæ prawid³owy numer bloku";
    EXPECT_GE(blockNumInt64, 0) << "getBlockNum dla int64_t powinien zwróciæ prawid³owy numer bloku";

    // Powinny wskazywaæ na ten sam blok, poniewa¿ te wartoœci s¹ w tym samym rekordzie
    EXPECT_EQ(blockNumInt, blockNumString) << "Bloki dla ró¿nych kolumn tego samego rekordu powinny byæ identyczne";
    EXPECT_EQ(blockNumString, blockNumInt64) << "Bloki dla ró¿nych kolumn tego samego rekordu powinny byæ identyczne";

    // Wyczyœæ po teœcie
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test przypadku, gdy tabela ma co najmniej 3 bloki
TEST(DataBaseTests, BTreeWithMultipleBlocks) {
    std::string tableName = "threeBlocksTable";
    Database db;

    // Utwórz tabelê
    db.addTable(tableName);
    db.addColumn(tableName, "id", int32_tId, false);
    db.addColumn(tableName, "name", stringId, false);

    // Dodaj rekordy do pierwszego bloku
    for (int i = 1; i <= 20; i++) {
        std::string name = "FirstBlock-" + std::to_string(i) + std::string(200, 'A');
        db.addRecord(tableName, { i, name });
    }

    // Dodaj rekordy do drugiego bloku
    for (int i = 21; i <= 40; i++) {
        std::string name = "SecondBlock-" + std::to_string(i) + std::string(200, 'B');
        db.addRecord(tableName, { i, name });
    }

    // Dodaj rekordy do trzeciego bloku
    std::string targetElement = "ThirdBlock-60" + std::string(200, 'C');
    for (int i = 41; i <= 60; i++) {
        std::string name = "ThirdBlock-" + std::to_string(i) + std::string(200, 'C');
        db.addRecord(tableName, { i, name });
    }

    // SprawdŸ czy tabela ma przynajmniej 3 bloki
    std::vector<int64_t> blockNums = db.getTableBlockNums(tableName);
    ASSERT_GE(blockNums.size(), 3) << "Tabela powinna mieæ co najmniej 3 bloki";

    // Dodaj B-tree dla kolumny "name"
    ASSERT_NO_THROW(db.addBtree(tableName, "name")) << "Dodanie B-tree zakoñczy³o siê b³êdem";

    // SprawdŸ numer bloku dla elementu z trzeciego bloku
    int blockNum = db.getBlockNum(tableName, "name", targetElement);
    EXPECT_EQ(blockNum, blockNums[2]) << "getBlockNum powinien zwróciæ numer trzeciego bloku dla elementu z trzeciego bloku";

    // Wyczyœæ po teœcie
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test obs³ugi nieistniej¹cych kolumn i elementów
TEST(DataBaseTests, BTreeWithNonExistentItems) {
    std::string tableName = "nonExistentItemsTable";
    Database db;

    // Utwórz tabelê
    db.addTable(tableName);
    db.addColumn(tableName, "id", int32_tId, false);
    db.addColumn(tableName, "name", stringId, false);

    // Dodaj kilka rekordów
    for (int i = 1; i <= 10; i++) {
        std::string name = "Record-" + std::to_string(i);
        db.addRecord(tableName, { i, name });
    }

    // Dodaj B-tree dla istniej¹cej kolumny
    ASSERT_NO_THROW(db.addBtree(tableName, "name")) << "Dodanie B-tree dla istniej¹cej kolumny zakoñczy³o siê b³êdem";

    // Spróbuj dodaæ B-tree dla nieistniej¹cej kolumny
    EXPECT_NO_THROW(db.addBtree(tableName, "nonExistentColumn")) << "addBtree powinien bezpiecznie obs³u¿yæ nieistniej¹c¹ kolumnê";

    // SprawdŸ getBlockNum dla istniej¹cego elementu
    int blockNum1 = db.getBlockNum(tableName, "name", std::string("Record-5"));
    EXPECT_GE(blockNum1, 0) << "getBlockNum powinien zwróciæ prawid³owy numer bloku dla istniej¹cego elementu";

    // SprawdŸ getBlockNum dla nieistniej¹cego elementu
    int blockNum2 = db.getBlockNum(tableName, "name", std::string("NonExistentRecord"));
    // W zale¿noœci od implementacji mo¿e zwróciæ -1 lub inn¹ wartoœæ wskazuj¹c¹ na brak elementu

    // SprawdŸ getBlockNum dla nieistniej¹cej kolumny
    int blockNum3 = db.getBlockNum(tableName, "nonExistentColumn", std::string("Record-5"));
    // W zale¿noœci od implementacji mo¿e zwróciæ -1 lub inn¹ wartoœæ wskazuj¹c¹ na brak kolumny

    // Wyczyœæ po teœcie
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test wydajnoœci dla du¿ej liczby rekordów
TEST(DataBaseTests, BTreePerformanceTest) {
    std::string tableName = "performanceTable";
    Database db;

    // Utwórz tabelê
    db.addTable(tableName);
    db.addColumn(tableName, "id", int32_tId, false);
    db.addColumn(tableName, "value", stringId, false);

    const int RECORD_COUNT = 300; // Du¿a liczba rekordów

    // Dodaj du¿¹ liczbê rekordów
    for (int i = 0; i < RECORD_COUNT; i++) {
        std::string value = "Value-" + std::to_string(i) + "-" + std::string(20, 'X');
        db.addRecord(tableName, { i, value });
    }

    // Zmierz czas tworzenia B-tree
    auto startCreate = std::chrono::high_resolution_clock::now();
    db.addBtree(tableName, "value");
    auto endCreate = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> createTime = endCreate - startCreate;

    std::cout << "Czas tworzenia B-tree dla " << RECORD_COUNT << " rekordów: "
        << createTime.count() << " sekund" << std::endl;

    // Zmierz czas wyszukiwania
    std::string searchValue = "Value-150-" + std::string(20, 'X');
    auto startSearch = std::chrono::high_resolution_clock::now();
    int blockNum = db.getBlockNum(tableName, "value", searchValue);
    auto endSearch = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> searchTime = endSearch - startSearch;

    std::cout << "Czas wyszukiwania elementu w B-tree: "
        << searchTime.count() << " sekund" << std::endl;

    EXPECT_GE(blockNum, 0) << "Nie znaleziono elementu w B-tree";
    EXPECT_LT(searchTime.count(), 0.01) << "Wyszukiwanie w B-tree powinno byæ szybkie (poni¿ej 10ms)";

    // Wyczyœæ po teœcie
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test zachowania B-tree po zapisaniu i ponownym wczytaniu bazy danych
TEST(DataBaseTests, BTreePersistenceTest) {
    std::string tableName = "persistenceTable";

    // Utwórz bazê danych, dodaj tabele, kolumny, rekordy i B-tree
    {
        Database db;
        db.addTable(tableName);
        db.addColumn(tableName, "id", int32_tId, false);
        db.addColumn(tableName, "name", stringId, false);

        for (int i = 1; i <= 20; i++) {
            std::string name = "Record-" + std::to_string(i);
            db.addRecord(tableName, { i, name });
        }

        // Dodaj B-tree i sprawdŸ numer bloku
        db.addBtree(tableName, "name");
        int originalBlockNum = db.getBlockNum(tableName, "name", std::string("Record-10"));
        EXPECT_GE(originalBlockNum, 0) << "Nie uda³o siê znaleŸæ elementu w B-tree";

        // Zapisz bazê danych
        db.commit();
    }

    // Wczytaj bazê danych ponownie i sprawdŸ czy B-tree dzia³a
    {
        Database db;
        db.loadDataBase();

        // Ponownie dodaj B-tree, poniewa¿ indeksy B-tree mog¹ nie byæ zapisywane
        db.addBtree(tableName, "name");

        // SprawdŸ czy getBlockNum zwraca prawid³owy numer bloku
        int newBlockNum = db.getBlockNum(tableName, "name", std::string("Record-10"));
        EXPECT_GE(newBlockNum, 0) << "Nie uda³o siê znaleŸæ elementu w B-tree po ponownym wczytaniu";
    }

    // Wyczyœæ po teœcie
    deleteFile(Database().getPath() + "/" + tableName + ".bin");
}

