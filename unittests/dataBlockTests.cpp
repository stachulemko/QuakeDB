#include "pch.h"
#include "dataBase.h"

TEST(DataBlockTests, EmptyTableSingleBlock) {
    Database db;
    std::string tableName = "emptyBlockTable";

    db.addTable(tableName);
    db.commit();

    Database loadDb;
    loadDb.loadDataBase();

    std::vector<int64_t> blockNums = loadDb.getTableBlockNums(tableName);
    EXPECT_EQ(blockNums.size(), 1);

    deleteFile(db.getPath() + "/" + tableName + ".bin");
}
/*
TEST(DataBlockTests, MultipleBlocksFor12KB) {
    Database db;
    std::string tableName = "blocks12KBTable";
    db.addTable(tableName);

    // Dodaj kolumny do przechowywania danych
    db.addColumn(tableName, "id", int32_tId, false);
    db.addColumn(tableName, "data", stringId, false);

    // Obliczamy rozmiar rekordu w bajtach
    // MaxBlockSize = 4108 bajtów
    // Ka¿dy rekord bêdzie mia³ oko³o 3 KB danych (3072 bajtów)
    // 4 takie rekordy dadz¹ nam oko³o 12 KB danych (12288 bajtów)
    const int dataSizeBytes = 3072;  // 3 * 1024 bajtów = 3 KB
    const int numRecords = 4;        // 4 rekordy po 3 KB = 12 KB ³¹cznie

    std::cout << "Rozpoczynam dodawanie " << numRecords << " rekordów po "
        << dataSizeBytes << " bajtów (" << (dataSizeBytes / 1024)
        << " KB) ka¿dy" << std::endl;

    for (int i = 0; i < numRecords; i++) {
        // Tworzymy unikaln¹ zawartoœæ dla ka¿dego rekordu
        std::string data = "Record_" + std::to_string(i) + "_" +
            std::string(dataSizeBytes - 10, 'A' + (i % 26));

        // Weryfikujemy faktyczny rozmiar stringa w bajtach
        std::cout << "Rekord #" << i << " - faktyczny rozmiar: "
            << data.size() << " bajtów" << std::endl;

        db.addRecord(tableName, { i, data });
    }

    db.commit();
    std::cout << "Zapisano dane do pliku" << std::endl;

    // Wczytaj bazê i sprawdŸ liczbê bloków
    Database loadDb;
    loadDb.loadDataBase();

    std::vector<int64_t> blockNums = loadDb.getTableBlockNums(tableName);
    std::cout << "Liczba bloków po dodaniu " << numRecords << " rekordów po ~"
        << dataSizeBytes << " bajtów: " << blockNums.size() << std::endl;

    // Dla 4 rekordów po 3072 bajtów (ok. 3 KB) oczekujemy 4 bloków,
    // poniewa¿ ka¿dy blok ma MaxBlockSize = 4108 bajtów (oko³o 4 KB)
    ASSERT_EQ(blockNums.size(), numRecords) << "Oczekiwano " << numRecords
        << " bloków dla " << (numRecords * dataSizeBytes) << " bajtów danych";

    // SprawdŸ czy numery bloków s¹ kolejne
    for (size_t i = 1; i < blockNums.size(); i++) {
        EXPECT_EQ(blockNums[i], blockNums[i - 1] + 1)
            << "Numery bloków nie s¹ kolejne miêdzy blokiem " << i - 1 << " a " << i;
    }

    // SprawdŸ, czy wszystkie rekordy zosta³y poprawnie zachowane
    loadDb.select(tableName, { "id", "data" });
    std::vector<std::vector<allVars>> result = loadDb.getSqlQueryBytes();

    // SprawdŸ czy liczba wyników to numRecords + 1 (nag³ówek)
    ASSERT_EQ(result.size(), numRecords + 1)
        << "Nieprawid³owa liczba wierszy w wynikach zapytania";

    // SprawdŸ zawartoœæ ka¿dego rekordu
    for (int i = 0; i < numRecords; i++) {
        EXPECT_EQ(std::get<int32_t>(result[i + 1][0]), i)
            << "Nieprawid³owe id dla rekordu " << i;

        std::string expectedData = "Record_" + std::to_string(i) + "_" +
            std::string(dataSizeBytes - 10, 'A' + (i % 26));
        EXPECT_EQ(std::get<std::string>(result[i + 1][1]), expectedData)
            << "Nieprawid³owe dane dla rekordu " << i;
    }

    // Usuñ plik po teœcie
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}
*/
/*
TEST(DataBlockTests, MultipleColumnsBlockDistribution) {
    Database db;
    std::string tableName = "multiColumnBlockTable";
    db.addTable(tableName);

    const int NUM_COLUMNS = 100;
    for (int i = 0; i < NUM_COLUMNS; i++) {
        std::string longColName = "column" + std::to_string(i) + std::string(500, 'X');
        db.addColumn(tableName, longColName, i % 3 + 1, i % 2 == 0);
    }
    db.commit();

    // Wczytaj bazê i sprawdŸ czy utworzono wiêcej ni¿ jeden blok
    Database loadDb;
    loadDb.loadDataBase();

    std::vector<int64_t> blockNums = loadDb.getTableBlockNums(tableName);
    std::cout << "Number of blocks: " << blockNums.size() << std::endl;

    // Z powodu du¿ych nazw kolumn, powinniœmy mieæ wiêcej ni¿ jeden blok
    EXPECT_GT(blockNums.size(), 1);

    // SprawdŸ czy numery bloków s¹ kolejne
    for (size_t i = 1; i < blockNums.size(); i++) {
        EXPECT_EQ(blockNums[i], blockNums[i - 1] + 1);
    }

    // Usuñ plik po teœcie
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

TEST(DataBlockTests, AddRecordsToMultipleBlocks) {
    Database db;
    std::string tableName = "multiBlockRecordTable";
    db.addTable(tableName);

    // Dodaj kolumny
    db.addColumn(tableName, "id", int32_tId, false);
    db.addColumn(tableName, "data", stringId, false);

    // Dodaj rekordy z du¿ymi ci¹gami znaków, aby wymusiæ tworzenie nowych bloków
    const int NUM_RECORDS = 20;
    for (int i = 0; i < NUM_RECORDS; i++) {
        std::string largeString(3000, static_cast<char>('A' + (i % 26)));
        db.addRecord(tableName, { i, largeString });
    }
    db.commit();

    // Wczytaj bazê i sprawdŸ liczbê bloków
    Database loadDb;
    loadDb.loadDataBase();

    std::vector<int64_t> blockNums = loadDb.getTableBlockNums(tableName);
    std::cout << "Number of blocks with records: " << blockNums.size() << std::endl;

    // Spodziewamy siê wielu bloków ze wzglêdu na du¿e rekordy
    EXPECT_GT(blockNums.size(), 1);

    // SprawdŸ czy wszystkie rekordy zosta³y zachowane
    loadDb.select(tableName, { "id", "data" });
    std::vector<std::vector<allVars>> result = loadDb.getSqlQueryBytes();

    // Nag³ówek + NUM_RECORDS wierszy
    EXPECT_EQ(result.size(), NUM_RECORDS + 1);

    // Usuñ plik po teœcie
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test sprawdzaj¹cy zachowanie bloków po operacjach commit i odczyt
TEST(DataBlockTests, PersistenceOfBlocks) {
    std::string tableName = "persistenceBlockTable";

    // Utwórz bazê, dodaj tabelê z kolumnami i rekordami
    {
        Database db;
        db.addTable(tableName);
        db.addColumn(tableName, "id", int32_tId, false);
        db.addColumn(tableName, "name", stringId, false);

        // Dodaj kilka rekordów
        for (int i = 0; i < 5; i++) {
            db.addRecord(tableName, { i, "Record" + std::to_string(i) });
        }
        db.commit();

        // SprawdŸ liczbê bloków przed zamkniêciem
        std::vector<int64_t> blockNumsBefore = db.getTableBlockNums(tableName);
        std::cout << "Blocks before close: " << blockNumsBefore.size() << std::endl;
    }

    // Wczytaj bazê ponownie i sprawdŸ czy liczba bloków i dane siê zgadzaj¹
    {
        Database loadDb;
        loadDb.loadDataBase();

        std::vector<int64_t> blockNumsAfter = loadDb.getTableBlockNums(tableName);
        std::cout << "Blocks after reload: " << blockNumsAfter.size() << std::endl;

        // SprawdŸ czy dane zosta³y poprawnie wczytane
        loadDb.select(tableName, { "id", "name" });
        std::vector<std::vector<allVars>> result = loadDb.getSqlQueryBytes();

        // Powinniœmy mieæ 6 wierszy (nag³ówek + 5 rekordów)
        EXPECT_EQ(result.size(), 6);

        // SprawdŸ wartoœci dla kilku rekordów
        EXPECT_EQ(std::get<int32_t>(result[1][0]), 0);
        EXPECT_EQ(std::get<std::string>(result[1][1]), "Record0");
        EXPECT_EQ(std::get<int32_t>(result[5][0]), 4);
        EXPECT_EQ(std::get<std::string>(result[5][1]), "Record4");
    }

    // Usuñ plik po teœcie
    deleteFile(Database().getPath() + "/" + tableName + ".bin");
}

// Test sprawdzaj¹cy zachowanie integralnoœci danych przy wielu blokach
TEST(DataBlockTests, DataIntegrityAcrossBlocks) {
    Database db;
    std::string tableName = "integrityBlockTable";
    db.addTable(tableName);

    // Dodaj kolumny
    db.addColumn(tableName, "id", int32_tId, false);
    db.addColumn(tableName, "bigText", stringId, false);

    // Dodaj rekordy z unikalnymi danymi du¿ego rozmiaru
    const int NUM_RECORDS = 10;
    for (int i = 0; i < NUM_RECORDS; i++) {
        // Generuj unikalne dane dla ka¿dego rekordu
        std::string uniqueData = "DATA_" + std::to_string(i) + "_" + std::string(2000, static_cast<char>('A' + i));
        db.addRecord(tableName, { i, uniqueData });
    }
    db.commit();

    // Wczytaj bazê i sprawdŸ dane
    Database loadDb;
    loadDb.loadDataBase();

    // U¿yj where, aby przetestowaæ równie¿ indeksowanie i wyszukiwanie w blokach
    for (int i = 0; i < NUM_RECORDS; i++) {
        loadDb.select(tableName, { "id", "bigText" })
            .where("id", "=", i);

        std::vector<std::vector<allVars>> result = loadDb.getSqlQueryBytes();

        // SprawdŸ czy mamy dok³adnie jeden wynik (plus nag³ówek)
        ASSERT_EQ(result.size(), 2);

        // SprawdŸ id
        EXPECT_EQ(std::get<int32_t>(result[1][0]), i);

        // SprawdŸ czy dane tekstowe s¹ poprawne
        std::string expectedData = "DATA_" + std::to_string(i) + "_" + std::string(2000, static_cast<char>('A' + i));
        EXPECT_EQ(std::get<std::string>(result[1][1]), expectedData);
    }

    // Usuñ plik po teœcie
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test sprawdzaj¹cy maksymaln¹ pojemnoœæ bloku danych
TEST(DataBlockTests, MaxBlockCapacity) {
    Database db;
    std::string tableName = "maxBlockCapacityTable";
    db.addTable(tableName);

    // Dodaj jedn¹ kolumnê do przechowywania danych
    db.addColumn(tableName, "id", int32_tId, false);
    db.addColumn(tableName, "largeData", stringId, false);

    // Najpierw dodaj jeden rekord z du¿¹ iloœci¹ danych, ale poni¿ej limitu bloku
    std::string data1(3000, 'A');  // Poni¿ej MaxBlockSize (4108 bajtów)
    db.addRecord(tableName, { 1, data1 });

    // Dodaj drugi rekord, który powinien byæ w nowym bloku
    std::string data2(3000, 'B');
    db.addRecord(tableName, { 2, data2 });

    db.commit();

    // Wczytaj bazê i sprawdŸ liczbê bloków
    Database loadDb;
    loadDb.loadDataBase();

    std::vector<int64_t> blockNums = loadDb.getTableBlockNums(tableName);
    std::cout << "Number of blocks with large records: " << blockNums.size() << std::endl;

    // Oczekujemy co najmniej 2 bloków ze wzglêdu na rozmiar rekordów
    EXPECT_GE(blockNums.size(), 2);

    // SprawdŸ czy oba rekordy zosta³y zachowane poprawnie
    loadDb.select(tableName, { "id", "largeData" });
    std::vector<std::vector<allVars>> result = loadDb.getSqlQueryBytes();

    ASSERT_EQ(result.size(), 3);  // nag³ówek + 2 rekordy

    EXPECT_EQ(std::get<int32_t>(result[1][0]), 1);
    EXPECT_EQ(std::get<std::string>(result[1][1]), data1);

    EXPECT_EQ(std::get<int32_t>(result[2][0]), 2);
    EXPECT_EQ(std::get<std::string>(result[2][1]), data2);

    // Usuñ plik po teœcie
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

// Test sprawdzaj¹cy operacje na wielu tabelach jednoczeœnie
TEST(DataBlockTests, MultipleTablesWithBlocks) {
    Database db;

    // Utwórz dwie tabele
    std::string table1 = "multiBlockTable1";
    std::string table2 = "multiBlockTable2";

    db.addTable(table1);
    db.addTable(table2);

    // Dodaj kolumny do pierwszej tabeli
    db.addColumn(table1, "id", int32_tId, false);
    db.addColumn(table1, "data", stringId, false);

    // Dodaj kolumny do drugiej tabeli
    db.addColumn(table2, "code", int32_tId, false);
    db.addColumn(table2, "description", stringId, false);

    // Dodaj rekordy do pierwszej tabeli, aby utworzyæ wiele bloków
    for (int i = 0; i < 10; i++) {
        std::string largeString(2000, 'X');
        db.addRecord(table1, { i, largeString });
    }

    // Dodaj rekordy do drugiej tabeli, równie¿ tworz¹c wiele bloków
    for (int i = 0; i < 10; i++) {
        std::string largeString(2000, 'Y');
        db.addRecord(table2, { i * 100, largeString });
    }

    db.commit();

    // Wczytaj bazê i sprawdŸ bloków w obu tabelach
    Database loadDb;
    loadDb.loadDataBase();

    std::vector<int64_t> blockNums1 = loadDb.getTableBlockNums(table1);
    std::vector<int64_t> blockNums2 = loadDb.getTableBlockNums(table2);

    std::cout << "Table1 blocks: " << blockNums1.size() << std::endl;
    std::cout << "Table2 blocks: " << blockNums2.size() << std::endl;

    // Obie tabele powinny mieæ wiele bloków
    EXPECT_GT(blockNums1.size(), 1);
    EXPECT_GT(blockNums2.size(), 1);

    // SprawdŸ dane w pierwszej tabeli
    loadDb.select(table1, { "id" })
        .where("id", "=", 5);
    std::vector<std::vector<allVars>> result1 = loadDb.getSqlQueryBytes();

    ASSERT_EQ(result1.size(), 2);  // nag³ówek + 1 rekord
    EXPECT_EQ(std::get<int32_t>(result1[1][0]), 5);

    // SprawdŸ dane w drugiej tabeli
    loadDb.select(table2, { "code" })
        .where("code", "=", 500);
    std::vector<std::vector<allVars>> result2 = loadDb.getSqlQueryBytes();

    ASSERT_EQ(result2.size(), 2);  // nag³ówek + 1 rekord
    EXPECT_EQ(std::get<int32_t>(result2[1][0]), 500);

    // Usuñ pliki po teœcie
    deleteFile(loadDb.getPath() + "/" + table1 + ".bin");
    deleteFile(loadDb.getPath() + "/" + table2 + ".bin");
}
*/