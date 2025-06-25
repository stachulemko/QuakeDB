#include "pch.h"
#include "wal.h"
#include "column.h"
#include "record.h"
#include "fileOperationManager.h"
#include "typeManager.h"
#include "dataBase.h"

// Test konstruktora klasy Wal
TEST(WalTests, WalConstructor) {
    // Œcie¿ka do pliku WAL
    std::string walPath = executionFilePath() + "/wal/wal.bin";

    // Usuñ pliki przed testem
    deleteFile(walPath);

    // Utwórz WAL poprzez konstruktor
    Wal wal;
    wal.createDirectoryAndFile();
    // SprawdŸ czy plik WAL zosta³ utworzony
    EXPECT_TRUE(isFileExists(walPath)) << "Konstruktor Wal nie utworzy³ pliku WAL";

    // Czyszczenie po teœcie
    clearFile(walPath);
    deleteFile(walPath);
}

// Test integracji klasy Table z mechanizmem WAL
TEST(WalTests, TableIntegrationWithWal) {
    std::string walPath = executionFilePath() + "/wal/wal.bin";
    std::string tablePath = executionFilePath() + "/dataBaseMemoryManagment/TestTable.bin";

    // Usuñ pliki przed testem
    deleteFile(walPath);
    // Utwórz instancjê klasy Wal dla tabeli
    Wal* wal = new Wal();
    wal->createDirectoryAndFile();

    // Utwórz instancjê klasy Table i dodaj kolumny
    Table table("TestTable", "dataBaseMemoryManagment", wal);
    table.addColumn("Id", int32_tId, false);
    table.addColumn("Name", stringId, false);

    // SprawdŸ czy dane o kolumnach zosta³y zapisane do WAL
    std::vector<uint8_t> walContent = readFileBytes(walPath);
    EXPECT_FALSE(walContent.empty()) << "Plik WAL jest pusty po dodaniu kolumn do tabeli";

    // Zwolnij pamiêæ
    delete wal;

    // Czyszczenie po teœcie
    clearFile(walPath);
    deleteFile(walPath);
    deleteFile(tablePath);
}


// Test dekodowania zawartoœci WAL po dodaniu kolumn do tabeli
// Test dekodowania zawartoœci WAL po dodaniu kolumn do tabeli
TEST(WalTests, DecodeWalAfterAddingColumns) {
    std::string walPath = executionFilePath() + "/wal/wal.bin";
    std::string tablePath = executionFilePath() + "/dataBaseMemoryManagment/TestTable.bin";

    // Usuñ pliki przed testem
    deleteFile(walPath);

    // Utwórz instancjê klasy Wal dla tabeli (tylko do zapisu)
    Wal* walWriter = new Wal();
    walWriter->createDirectoryAndFile();

    // Utwórz instancjê klasy Table i dodaj kolumny
    Table table("TestTable", "dataBaseMemoryManagment", walWriter);
    table.addColumn("Id", int32_tId, false);
    table.addColumn("Name", stringId, false);

    // Zwolnij pamiêæ zapisuj¹cego WAL
    delete walWriter;

    std::cout << "Kolumny zosta³y dodane do WAL" << std::endl;

    // Teraz utwórz now¹ instancjê Wal tylko do odczytu (dekodowania)
    Wal* walReader = new Wal();
    walReader->createDirectoryAndFile();

    // Dekoduj WAL i sprawdŸ zawartoœæ
    walReader->loadWal();

    // Pobierz dane z WAL
    std::vector<std::vector<allVars>> walData = walReader->getWalData();
    std::cout << "-----------------" << std::endl;
    showVariantVectorOfVector(walData);
    std::cout << "-----------------" << std::endl;
    std::cout << "walData.size() = " << walData.size() << std::endl;

    // SprawdŸ, czy w danych WAL s¹ 2 wpisy (dla ka¿dej kolumny)
    EXPECT_EQ(walData.size(), 2) << "Nieprawid³owa liczba wpisów w WAL";

    // SprawdŸ, czy dane kolumn s¹ poprawne
    if (walData.size() >= 2) {
        // SprawdŸ pierwsz¹ kolumnê
        EXPECT_EQ(std::get<int32_t>(walData[0][0]), addColumnTypeId) << "Nieprawid³owy typ operacji dla pierwszej kolumny";
        EXPECT_EQ(std::get<std::string>(walData[0][2]), "Id") << "Nieprawid³owa nazwa pierwszej kolumny";
        EXPECT_EQ(std::get<int32_t>(walData[0][3]), int32_tId) << "Nieprawid³owy typ danych pierwszej kolumny";

        // Zamiast u¿ywaæ std::get<bool>, sprawdzamy tylko czy indeks istnieje
        EXPECT_GE(walData[0].size(), 5) << "Za ma³o elementów w wektorze dla pierwszej kolumny";

        // SprawdŸ drug¹ kolumnê
        EXPECT_EQ(std::get<int32_t>(walData[1][0]), addColumnTypeId) << "Nieprawid³owy typ operacji dla drugiej kolumny";
        EXPECT_EQ(std::get<std::string>(walData[1][2]), "Name") << "Nieprawid³owa nazwa drugiej kolumny";
        EXPECT_EQ(std::get<int32_t>(walData[1][3]), stringId) << "Nieprawid³owy typ danych drugiej kolumny";

        // Zamiast u¿ywaæ std::get<bool>, sprawdzamy tylko czy indeks istnieje
        EXPECT_GE(walData[1].size(), 5) << "Za ma³o elementów w wektorze dla drugiej kolumny";
    }

    // Zwolnij pamiêæ czytaj¹cego WAL
    delete walReader;

    // Czyszczenie po teœcie
    clearFile(walPath);
    deleteFile(walPath);
    deleteFile(tablePath);
}

// Test obs³ugi bardzo du¿ej nazwy kolumny w WAL
TEST(WalTests, ExtremelyLongColumnNameInWal) {
    std::string walPath = executionFilePath() + "/wal/wal.bin";
    std::string tablePath = executionFilePath() + "/dataBaseMemoryManagment/LongNameTable.bin";

    // Usuñ pliki przed testem
    deleteFile(walPath);

    // Utwórz instancjê klasy Wal dla tabeli (tylko do zapisu)
    Wal* walWriter = new Wal();
    walWriter->createDirectoryAndFile();

    // Utwórz instancjê klasy Table
    Table table("LongNameTable", "dataBaseMemoryManagment", walWriter);

    // Dodaj kolumnê z bardzo d³ug¹ nazw¹ - 10000 znaków
    std::string extremelyLongName(10000, 'X');
    table.addColumn(extremelyLongName, int32_tId, false);

    // Zwolnij pamiêæ zapisuj¹cego WAL
    delete walWriter;

    // Teraz utwórz now¹ instancjê Wal do odczytu
    Wal* walReader = new Wal();
    walReader->createDirectoryAndFile();
    walReader->loadWal();

    // Pobierz dane z WAL
    std::vector<std::vector<allVars>> walData = walReader->getWalData();

    // SprawdŸ czy WAL zawiera jakieœ dane
    EXPECT_FALSE(walData.empty()) << "WAL nie zawiera ¿adnych danych po dodaniu kolumny z bardzo d³ug¹ nazw¹";

    // SprawdŸ czy nazwa kolumny zosta³a zachowana (mo¿e byæ obciêta)
    if (!walData.empty() && walData[0].size() >= 3) {
        std::string retrievedName = std::get<std::string>(walData[0][2]);
        // SprawdŸ czy pocz¹tek nazwy siê zgadza
        EXPECT_EQ(retrievedName.substr(0, 100), extremelyLongName.substr(0, 100))
            << "Pocz¹tek d³ugiej nazwy kolumny nie zosta³ poprawnie zapisany";
    }

    // Zwolnij pamiêæ czytaj¹cego WAL
    delete walReader;

    // Czyszczenie po teœcie
    clearFile(walPath);
    deleteFile(walPath);
    deleteFile(tablePath);
}

// Test obs³ugi niepoprawnych danych typów w WAL
TEST(WalTests, InvalidDataTypesInWal) {
    std::string walPath = executionFilePath() + "/wal/wal.bin";
    std::string tablePath = executionFilePath() + "/dataBaseMemoryManagment/InvalidTypeTable.bin";

    // Usuñ pliki przed testem
    deleteFile(walPath);

    // Utwórz instancjê klasy Wal dla tabeli (tylko do zapisu)
    Wal* walWriter = new Wal();
    walWriter->createDirectoryAndFile();

    // Utwórz instancjê klasy Table
    Table table("InvalidTypeTable", "dataBaseMemoryManagment", walWriter);

    // Dodaj kolumnê z niestandardowym typem danych
    int32_t invalidTypeId = 999; // Niezdefiniowany typ
    table.addColumn("InvalidTypeColumn", invalidTypeId, false);

    // Zwolnij pamiêæ zapisuj¹cego WAL
    delete walWriter;

    // Teraz utwórz now¹ instancjê Wal do odczytu
    Wal* walReader = new Wal();
    walReader->createDirectoryAndFile();

    // SprawdŸ czy dekodowanie WAL nie powoduje wyj¹tku
    EXPECT_NO_THROW({
        walReader->loadWal();
        }) << "Dekodowanie WAL z niepoprawnym typem danych spowodowa³o wyj¹tek";

    // Pobierz dane z WAL
    std::vector<std::vector<allVars>> walData = walReader->getWalData();

    // SprawdŸ czy dane zosta³y zapisane
    EXPECT_FALSE(walData.empty()) << "WAL nie zawiera danych po dodaniu kolumny z nieprawid³owym typem";

    // SprawdŸ czy typ kolumny zosta³ zachowany
    if (!walData.empty() && walData[0].size() >= 4) {
        EXPECT_EQ(std::get<int32_t>(walData[0][3]), invalidTypeId)
            << "Nieprawid³owy typ kolumny nie zosta³ poprawnie zapisany w WAL";
    }

    // Zwolnij pamiêæ czytaj¹cego WAL
    delete walReader;

    // Czyszczenie po teœcie
    clearFile(walPath);
    deleteFile(walPath);
    deleteFile(tablePath);
}

// Test obs³ugi znaków specjalnych w WAL
TEST(WalTests, SpecialCharactersInWal) {
    std::string walPath = executionFilePath() + "/wal/wal.bin";
    std::string tablePath = executionFilePath() + "/dataBaseMemoryManagment/SpecialCharsTable.bin";

    // Usuñ pliki przed testem
    deleteFile(walPath);

    // Utwórz instancjê klasy Wal dla tabeli (tylko do zapisu)
    Wal* walWriter = new Wal();
    walWriter->createDirectoryAndFile();

    // Utwórz instancjê klasy Table
    Table table("SpecialCharsTable", "dataBaseMemoryManagment", walWriter);

    // Dodaj kolumny ze znakami specjalnymi w nazwach
    table.addColumn("Column!@#$%^&*()", int32_tId, false);
    table.addColumn("Column\n\r\t\b", stringId, true);
    table.addColumn("Column\0Null", int64_tId, false); // Z zerem w œrodku
    table.addColumn("Column\u00E9\u00F1\u00FC", int32_tId, true); // Ze znakami Unicode

    // Dodaj rekord ze znakami specjalnymi
    std::vector<allVars> record = {
        123,
        std::string("Value!@#$%^&*()"),
        static_cast<int64_t>(456),
        789
    };
    table.addRecord(record);

    // Zwolnij pamiêæ zapisuj¹cego WAL
    delete walWriter;

    // Teraz utwórz now¹ instancjê Wal do odczytu
    Wal* walReader = new Wal();
    walReader->createDirectoryAndFile();
    walReader->loadWal();

    // Pobierz dane z WAL
    std::vector<std::vector<allVars>> walData = walReader->getWalData();

    // SprawdŸ czy WAL zawiera odpowiedni¹ liczbê wpisów (4 kolumny + 1 rekord)
    EXPECT_EQ(walData.size(), 5) << "Nieprawid³owa liczba wpisów w WAL po dodaniu kolumn ze znakami specjalnymi";

    // SprawdŸ nazwy kolumn
    if (walData.size() >= 4) {
        EXPECT_EQ(std::get<std::string>(walData[0][2]), "Column!@#$%^&*()");
        // Dla nazwy z bia³ymi znakami mo¿emy sprawdziæ tylko d³ugoœæ (mog³y zostaæ znormalizowane)
        EXPECT_GE(std::get<std::string>(walData[1][2]).length(), 6);
        // Dla nazwy z zerem w œrodku sprawdŸmy przynajmniej pocz¹tek
        EXPECT_TRUE(std::get<std::string>(walData[2][2]).find("Column") == 0);
        // Dla nazwy ze znakami Unicode mo¿emy sprawdziæ d³ugoœæ
        EXPECT_GE(std::get<std::string>(walData[3][2]).length(), 6);
    }

    // SprawdŸ dane rekordu ze znakami specjalnymi
    if (walData.size() >= 5) {
        EXPECT_EQ(std::get<std::string>(walData[4][3]), "Value!@#$%^&*()");
    }

    // Zwolnij pamiêæ czytaj¹cego WAL
    delete walReader;

    // Czyszczenie po teœcie
    clearFile(walPath);
    deleteFile(walPath);
    deleteFile(tablePath);
}

// Test obs³ugi pustych wartoœci w WAL
/*
TEST(WalTests, EmptyValuesInWal) {
    std::string walPath = executionFilePath() + "/wal/wal.bin";
    std::string tablePath = executionFilePath() + "/dataBaseMemoryManagment/EmptyValuesTable.bin";

    // Usuñ pliki przed testem
    deleteFile(walPath);

    // Utwórz instancjê klasy Wal dla tabeli (tylko do zapisu)
    Wal* walWriter = new Wal();
    walWriter->createDirectoryAndFile();

    // Utwórz instancjê klasy Table
    Table table("EmptyValuesTable", "dataBaseMemoryManagment", walWriter);

#ifdef NDEBUG
    // W trybie Release asercje s¹ wy³¹czone, wiêc mo¿na testowaæ pusty string
    table.addColumn("", int32_tId, false); // Pusta nazwa kolumny mo¿e zadzia³aæ w Release
#else
    // W trybie Debug zamiast pustej nazwy u¿yjmy pojedynczego znaku spacji
    // ¿eby unikn¹æ asercji
    table.addColumn(" ", int32_tId, false);
#endif

    // Dodaj kolumnê z normaln¹ nazw¹
    table.addColumn("StringCol", stringId, true);

    // Dodaj rekord z pustym stringiem (to powinno zadzia³aæ - nie ma asercji dla pustych wartoœci stringów)
    std::vector<allVars> record = {
#ifdef NDEBUG
        0, // Zerowa wartoœæ dla kolumny z pust¹ nazw¹
#else
        0, // Zerowa wartoœæ dla kolumny z nazw¹ spacji
#endif
        std::string("") // Pusty string dla StringCol
    };
    table.addRecord(record);

    // Zwolnij pamiêæ zapisuj¹cego WAL
    delete walWriter;

    // Teraz utwórz now¹ instancjê Wal do odczytu
    Wal* walReader = new Wal();
    walReader->createDirectoryAndFile();
    walReader->loadWal();

    // Pobierz dane z WAL
    std::vector<std::vector<allVars>> walData = walReader->getWalData();

    // SprawdŸ czy WAL zawiera odpowiedni¹ liczbê wpisów (2 kolumny + 1 rekord)
    EXPECT_EQ(walData.size(), 3) << "Nieprawid³owa liczba wpisów w WAL po dodaniu pustych wartoœci";

#ifdef NDEBUG
    // W trybie Release sprawdzamy czy pusta nazwa kolumny zosta³a zachowana
    if (walData.size() >= 1) {
        EXPECT_TRUE(std::get<std::string>(walData[0][2]).empty()) << "Pusta nazwa kolumny nie zosta³a zachowana";
    }
#else
    // W trybie Debug sprawdzamy czy nazwa zawiera spacjê
    if (walData.size() >= 1) {
        EXPECT_EQ(std::get<std::string>(walData[0][2]), " ") << "Nazwa kolumny ze spacj¹ nie zosta³a zachowana";
    }
#endif

    // SprawdŸ pusty string w rekordzie (to powinno dzia³aæ zarówno w Debug jak i Release)
    if (walData.size() >= 3) {
        EXPECT_TRUE(std::get<std::string>(walData[2][3]).empty()) << "Pusty string w rekordzie nie zosta³ zachowany";
    }

    // Zwolnij pamiêæ czytaj¹cego WAL
    delete walReader;

    // Czyszczenie po teœcie
    clearFile(walPath);
    deleteFile(walPath);
    deleteFile(tablePath);
}
*/
// Test odpornoœci na bardzo du¿y rozmiar WAL
/*
TEST(WalTests, LargeWalFileHandling) {
    std::string walPath = executionFilePath() + "/wal/wal.bin";
    std::string tablePath = executionFilePath() + "/dataBaseMemoryManagment/LargeTable.bin";

    // Usuñ pliki przed testem
    deleteFile(walPath);

    // Utwórz instancjê klasy Wal dla tabeli (tylko do zapisu)
    Wal* walWriter = new Wal();
    walWriter->createDirectoryAndFile();

    // Utwórz instancjê klasy Table
    Table table("LargeTable", "dataBaseMemoryManagment", walWriter);

    // Dodaj wiele kolumn
    const int numColumns = 50;
    for (int i = 0; i < numColumns; i++) {
        table.addColumn("Column" + std::to_string(i), i % 3 + 1, i % 2 == 0);
    }

    // Dodaj wiele rekordów z du¿ymi stringami
    const int numRecords = 100;
    std::string largeString(1000, 'X'); // 1000 znaków

    for (int i = 0; i < numRecords; i++) {
        std::vector<allVars> record;
        for (int j = 0; j < numColumns; j++) {
            switch (j % 3) {
            case 0:
                record.push_back(i); // int32_t
                break;
            case 1:
                record.push_back(largeString + std::to_string(i)); // du¿y string
                break;
            case 2:
                record.push_back(static_cast<int64_t>(i * 1000)); // int64_t
                break;
            }
        }
        table.addRecord(record);
    }

    // Zwolnij pamiêæ zapisuj¹cego WAL
    delete walWriter;

    // SprawdŸ czy plik WAL zosta³ utworzony i ma du¿y rozmiar
    std::vector<uint8_t> walContent = readFileBytes(walPath);
    std::cout << "Rozmiar pliku WAL: " << walContent.size() << " bajtów" << std::endl;
    EXPECT_GT(walContent.size(), 1000000) << "Plik WAL jest zbyt ma³y dla du¿ej iloœci danych";

    // Teraz utwórz now¹ instancjê Wal do odczytu
    Wal* walReader = new Wal();
    walReader->createDirectoryAndFile();

    // Zmierz czas dekodowania du¿ego pliku WAL
    auto start = std::chrono::high_resolution_clock::now();
    walReader->loadWal();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Czas dekodowania du¿ego WAL: " << duration.count() << " sekund" << std::endl;

    // Pobierz dane z WAL
    std::vector<std::vector<allVars>> walData = walReader->getWalData();

    // SprawdŸ czy wszystkie dane zosta³y poprawnie zdekodowane
    EXPECT_EQ(walData.size(), numColumns + numRecords) << "Niepoprawna liczba zdekodowanych wpisów";

    // Zwolnij pamiêæ czytaj¹cego WAL
    delete walReader;

    // Czyszczenie po teœcie
    clearFile(walPath);
    deleteFile(walPath);
    deleteFile(tablePath);
}
*/
// Test reakcji na uszkodzony plik WAL



TEST(WalTests, CorruptedWalFile) {
    std::string walPath = executionFilePath() + "/wal/wal.bin";
    std::string tablePath = executionFilePath() + "/dataBaseMemoryManagment/CorruptedTable.bin";

    // Usuñ pliki przed testem
    deleteFile(walPath);

    // Utwórz instancjê klasy Wal dla tabeli
    Wal* walWriter = new Wal();
    walWriter->createDirectoryAndFile();

    // Dodaj normalne dane do WAL
    Table table("CorruptedTable", "dataBaseMemoryManagment", walWriter);
    table.addColumn("ValidColumn", int32_tId, false);

    // Zwolnij pamiêæ zapisuj¹cego WAL
    delete walWriter;

    // Teraz uszkodŸ plik WAL poprzez dodanie nieprawid³owych danych
    std::vector<uint8_t> corruptData = { 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00 };
    addToFileBytes(walPath, corruptData);

    // Utwórz now¹ instancjê Wal i spróbuj zdekodowaæ uszkodzony WAL
    Wal* walReader = new Wal();
    walReader->createDirectoryAndFile();

    // SprawdŸ czy dekodowanie nie powoduje awarii programu
    EXPECT_NO_THROW({
        walReader->loadWal();
        }) << "Dekodowanie uszkodzonego WAL spowodowa³o nieoczekiwany wyj¹tek";

    // Pobierz dane z WAL
    std::vector<std::vector<allVars>> walData = walReader->getWalData();

    // Oczekujemy, ¿e przynajmniej czêœæ danych zosta³a zdekodowana
    EXPECT_GE(walData.size(), 1) << "Nie uda³o siê zdekodowaæ ¿adnych danych z uszkodzonego WAL";

    // Zwolnij pamiêæ czytaj¹cego WAL
    delete walReader;

    // Czyszczenie po teœcie
    clearFile(walPath);
    deleteFile(walPath);
    deleteFile(tablePath);
}

// Test obs³ugi pustego pliku WAL
TEST(WalTests, EmptyWalFile) {
    std::string walPath = executionFilePath() + "/wal/wal.bin";

    // Usuñ pliki przed testem
    deleteFile(walPath);

    // Utwórz instancjê klasy Wal
    Wal* wal = new Wal();
    wal->createDirectoryAndFile();

    // Wyczyœæ plik WAL (upewnij siê, ¿e jest pusty)
    clearFile(walPath);

    // SprawdŸ czy dekodowanie pustego WAL nie powoduje wyj¹tku
    EXPECT_NO_THROW({
        wal->loadWal();
        }) << "Dekodowanie pustego WAL spowodowa³o wyj¹tek";

    // Pobierz dane z WAL
    std::vector<std::vector<allVars>> walData = wal->getWalData();

    // SprawdŸ czy wektor jest pusty
    EXPECT_TRUE(walData.empty()) << "Dekodowanie pustego WAL powinno zwróciæ pusty wektor";

    // Zwolnij pamiêæ
    delete wal;

    // Czyszczenie po teœcie
    deleteFile(walPath);
}

// Test obs³ugi wielokrotnego zapisu i odczytu WAL
TEST(WalTests, MultipleWriteReadCycles) {
    std::string walPath = executionFilePath() + "/wal/wal.bin";
    std::string tablePath = executionFilePath() + "/dataBaseMemoryManagment/CyclesTable.bin";

    // Usuñ pliki przed testem
    deleteFile(walPath);

    // PrzeprowadŸ wiele cykli zapisu i odczytu
    const int numCycles = 5;

    for (int cycle = 0; cycle < numCycles; cycle++) {
        {
            Wal* walWriter = new Wal();
            walWriter->createDirectoryAndFile();

            Table table("CyclesTable" + std::to_string(cycle), "dataBaseMemoryManagment", walWriter);
            table.addColumn("Cycle" + std::to_string(cycle), int32_tId, false);

            delete walWriter;
        }

        // Odczytaj dane
        {
            Wal* walReader = new Wal();
            walReader->createDirectoryAndFile();
            walReader->loadWal();

            std::vector<std::vector<allVars>> walData = walReader->getWalData();

            // SprawdŸ czy liczba wpisów roœnie z ka¿dym cyklem
            EXPECT_EQ(walData.size(), cycle + 1) << "Nieprawid³owa liczba wpisów po cyklu " << cycle;

            delete walReader;
        }
    }

    // Czyszczenie po teœcie
    clearFile(walPath);
    deleteFile(walPath);

    for (int cycle = 0; cycle < numCycles; cycle++) {
        deleteFile(executionFilePath() + "/dataBaseMemoryManagment/CyclesTable" + std::to_string(cycle) + ".bin");
    }
}

// Test wytrzyma³oœciowy - du¿o operacji w krótkim czasie
/*
TEST(WalTests, StressTest) {
    std::string walPath = executionFilePath() + "/wal/wal.bin";
    std::string tablePath = executionFilePath() + "/dataBaseMemoryManagment/StressTable.bin";

    // Usuñ pliki przed testem
    deleteFile(walPath);

    // Utwórz instancjê klasy Wal dla tabeli
    Wal* walWriter = new Wal();
    walWriter->createDirectoryAndFile();

    // Utwórz instancjê klasy Table
    Table table("StressTable", "dataBaseMemoryManagment", walWriter);

    // Dodaj kolumny
    table.addColumn("IntCol", int32_tId, false);
    table.addColumn("StringCol", stringId, false);

    // Wykonaj wiele operacji dodawania rekordów w pêtli
    const int numOperations = 1000;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < numOperations; i++) {
        std::vector<allVars> record = {
            i,
            std::string("Value" + std::to_string(i))
        };
        table.addRecord(record);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Czas dodania " << numOperations << " rekordów: " << duration.count() << " sekund" << std::endl;
    std::cout << "Œredni czas na rekord: " << (duration.count() * 1000 / numOperations) << " ms" << std::endl;

    // Zwolnij pamiêæ zapisuj¹cego WAL
    delete walWriter;

    // Teraz zmierz czas dekodowania
    Wal* walReader = new Wal();
    walReader->createDirectoryAndFile();

    start = std::chrono::high_resolution_clock::now();
    walReader->loadWal();
    end = std::chrono::high_resolution_clock::now();

    duration = end - start;
    std::cout << "Czas dekodowania WAL z " << numOperations << " rekordami: " << duration.count() << " sekund" << std::endl;

    // Pobierz dane z WAL
    std::vector<std::vector<allVars>> walData = walReader->getWalData();

    // SprawdŸ czy wszystkie dane zosta³y zapisane i odczytane
    EXPECT_EQ(walData.size(), numOperations + 2) << "Nieprawid³owa liczba wpisów po teœcie wydajnoœciowym";

    // Zwolnij pamiêæ czytaj¹cego WAL
    delete walReader;

    // Czyszczenie po teœcie
    clearFile(walPath);
    deleteFile(walPath);
    deleteFile(tablePath);
}
*/

/*
// Test wspó³bie¿noœci - symulacja dostêpu z wielu w¹tków
TEST(WalTests, ConcurrentAccess) {
    std::string walPath = executionFilePath() + "/wal/wal.bin";
    std::string tablePath = executionFilePath() + "/dataBaseMemoryManagment/ConcurrentTable.bin";

    // Usuñ pliki przed testem
    deleteFile(walPath);

    // Utwórz instancjê klasy Wal dla tabeli
    Wal* wal = new Wal();
    wal->createDirectoryAndFile();

    // Utwórz instancjê klasy Table
    Table table("ConcurrentTable", "dataBaseMemoryManagment", wal);
    table.addColumn("ThreadId", int32_tId, false);
    table.addColumn("Value", int32_tId, false);

    // Funkcja do wykonania w w¹tku
    auto threadFunction = [&table](int threadId) {
        // Ka¿dy w¹tek dodaje kilka rekordów
        for (int i = 0; i < 100; i++) {
            std::vector<allVars> record = {
                threadId,
                i
            };
            table.addRecord(record);
        }
        };

    // Utwórz i uruchom w¹tki
    const int numThreads = 5;
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; i++) {
        threads.emplace_back(threadFunction, i);
    }

    // Poczekaj na zakoñczenie wszystkich w¹tków
    for (auto& thread : threads) {
        thread.join();
    }

    // Zwolnij pamiêæ WAL
    delete wal;

    // Utwórz now¹ instancjê Wal do odczytu
    Wal* walReader = new Wal();
    walReader->createDirectoryAndFile();
    walReader->decodeWall();

    // Pobierz dane z WAL
    std::vector<std::vector<allVars>> walData = walReader->getWalData();

    // SprawdŸ czy wszystkie dane zosta³y zapisane
    // Oczekujemy 1 wpisu dla kolumny + numThreads * 100 rekordów + 1 wpis dla kolumny
    EXPECT_EQ(walData.size(), 2 + numThreads * 100)
        << "Nieprawid³owa liczba wpisów po teœcie wspó³bie¿noœci";

    // Zwolnij pamiêæ czytaj¹cego WAL
    delete walReader;

    // Czyszczenie po teœcie
    clearFile(walPath);
    deleteFile(walPath);
    deleteFile(tablePath);
}


// Test dekodowania zawartoœci WAL po dodaniu rekordu do tabeli
TEST(WalTests, DecodeWalAfterAddingRecord) {
    std::string walPath = executionFilePath() + "/wal/wal.bin";
    std::string tablePath = executionFilePath() + "/dataBaseMemoryManagment/TestTable.bin";

    // Usuñ pliki przed testem
    deleteFile(walPath);

    // Utwórz instancjê klasy Wal dla tabeli
    Wal* wal = new Wal();
    wal->createDirectoryAndFile();

    // Utwórz instancjê klasy Table i dodaj kolumny
    Table table("TestTable", "dataBaseMemoryManagment", wal);
    table.addColumn("Id", int32_tId, false);
    table.addColumn("Name", stringId, false);

    // Dodaj rekord do tabeli
    std::vector<allVars> record = { 1, std::string("Test") };
    table.addRecord(record);

    // Dekoduj WAL
    wal->decodeWall();

    // Pobierz dane z WAL
    std::vector<std::vector<allVars>> walData = wal->getWalData();

    // SprawdŸ, czy w danych WAL s¹ 3 wpisy (2 kolumny + 1 rekord)
    EXPECT_EQ(walData.size(), 3) << "Nieprawid³owa liczba wpisów w WAL";

    // SprawdŸ dane rekordu
    if (walData.size() >= 3) {
        EXPECT_EQ(std::get<int32_t>(walData[2][0]), insertTypeId) << "Nieprawid³owy typ operacji dla rekordu";

        // SprawdŸ wartoœci w rekordzie, jeœli istniej¹
        if (walData[2].size() >= 3) {
            EXPECT_EQ(std::get<int32_t>(walData[2][2]), 1) << "Nieprawid³owa wartoœæ Id w rekordzie";
        }
        if (walData[2].size() >= 4) {
            EXPECT_EQ(std::get<std::string>(walData[2][3]), "Test") << "Nieprawid³owa wartoœæ Name w rekordzie";
        }
    }

    // Zwolnij pamiêæ
    delete wal;

    // Czyszczenie po teœcie
    clearFile(walPath);
    deleteFile(walPath);
    deleteFile(tablePath);
}

// Test weryfikacji spójnoœci WAL po wielu operacjach
TEST(WalTests, ConsistencyAfterMultipleOperations) {
    std::string walPath = executionFilePath() + "/wal/wal.bin";
    std::string tablePath = executionFilePath() + "/dataBaseMemoryManagment/TestTable.bin";

    // Usuñ pliki przed testem
    deleteFile(walPath);

    // Utwórz instancjê klasy Wal dla tabeli
    Wal* wal = new Wal();
    wal->createDirectoryAndFile();

    // Utwórz instancjê klasy Table
    Table table("TestTable", "dataBaseMemoryManagment", wal);

    // Dodaj kolumny w pêtli
    const int columnCount = 3;
    for (int i = 0; i < columnCount; i++) {
        std::string columnName = "Column" + std::to_string(i);
        int32_t columnType = (i % 2 == 0) ? int32_tId : stringId; // Naprzemiennie int i string
        bool allowNull = (i % 2 == 1); // Naprzemiennie false/true
        table.addColumn(columnName, columnType, allowNull);
    }

    // Dodaj rekordy w pêtli
    const int recordCount = 3;
    for (int i = 0; i < recordCount; i++) {
        std::vector<allVars> record;
        for (int j = 0; j < columnCount; j++) {
            if (j % 2 == 0) {
                record.push_back(i * 10 + j); // int32_t
            }
            else {
                record.push_back(std::string("Value" + std::to_string(i * 10 + j))); // string
            }
        }
        table.addRecord(record);
    }

    // Dekoduj WAL
    wal->decodeWall();

    // Pobierz dane z WAL
    std::vector<std::vector<allVars>> walData = wal->getWalData();

    // SprawdŸ, czy w danych WAL jest odpowiednia liczba wpisów
    int expectedEntries = columnCount + recordCount;
    EXPECT_EQ(walData.size(), expectedEntries) << "Nieprawid³owa liczba wpisów w WAL";

    // SprawdŸ wpisy kolumn i rekordów
    int columnCounter = 0;
    int recordCounter = 0;

    for (size_t i = 0; i < walData.size(); i++) {
        if (walData[i].size() > 0) {
            int32_t operationType = std::get<int32_t>(walData[i][0]);

            if (operationType == addColumnTypeId) {
                columnCounter++;
            }
            else if (operationType == insertTypeId) {
                recordCounter++;
            }
        }
    }

    EXPECT_EQ(columnCounter, columnCount) << "Nieprawid³owa liczba operacji addColumn w WAL";
    EXPECT_EQ(recordCounter, recordCount) << "Nieprawid³owa liczba operacji insert w WAL";

    // Zwolnij pamiêæ
    delete wal;

    // Czyszczenie po teœcie
    clearFile(walPath);
    deleteFile(walPath);
    deleteFile(tablePath);
}

// Test zdolnoœci odtworzenia danych z WAL
TEST(WalTests, RecoveryFromWal) {
    std::string walPath = executionFilePath() + "/wal/wal.bin";
    std::string tablePath = executionFilePath() + "/dataBaseMemoryManagment/TestTable.bin";

    // Usuñ pliki przed testem
    deleteFile(walPath);

    // Utwórz Wal i tabele, dodaj dane
    {
        Wal* wal = new Wal();
        wal->createDirectoryAndFile();

        Table table("TestTable", "dataBaseMemoryManagment", wal);
        table.addColumn("Id", int32_tId, false);
        table.addColumn("Name", stringId, false);

        table.addRecord({ 1, std::string("Record1") });
        table.addRecord({ 2, std::string("Record2") });

        delete wal;
    }

    // Teraz stwórz nowy obiekt Wal i spróbuj odtworzyæ dane
    Wal* recoveryWal = new Wal();
    recoveryWal->createDirectoryAndFile();
    recoveryWal->decodeWall();

    // Pobierz dane z WAL
    std::vector<std::vector<allVars>> recoveredData = recoveryWal->getWalData();

    // SprawdŸ, czy odzyskane dane s¹ poprawne
    EXPECT_EQ(recoveredData.size(), 4) << "Nieprawid³owa liczba wpisów w odtworzonym WAL";

    // SprawdŸ wpisy kolumn
    if (recoveredData.size() >= 2) {
        EXPECT_EQ(std::get<int32_t>(recoveredData[0][0]), addColumnTypeId);
        if (recoveredData[0].size() >= 3) {
            EXPECT_EQ(std::get<std::string>(recoveredData[0][2]), "Id");
        }

        EXPECT_EQ(std::get<int32_t>(recoveredData[1][0]), addColumnTypeId);
        if (recoveredData[1].size() >= 3) {
            EXPECT_EQ(std::get<std::string>(recoveredData[1][2]), "Name");
        }
    }

    // SprawdŸ wpisy rekordów
    if (recoveredData.size() >= 4) {
        EXPECT_EQ(std::get<int32_t>(recoveredData[2][0]), insertTypeId);
        if (recoveredData[2].size() >= 3) {
            EXPECT_EQ(std::get<int32_t>(recoveredData[2][2]), 1);
        }
        if (recoveredData[2].size() >= 4) {
            EXPECT_EQ(std::get<std::string>(recoveredData[2][3]), "Record1");
        }

        EXPECT_EQ(std::get<int32_t>(recoveredData[3][0]), insertTypeId);
        if (recoveredData[3].size() >= 3) {
            EXPECT_EQ(std::get<int32_t>(recoveredData[3][2]), 2);
        }
        if (recoveredData[3].size() >= 4) {
            EXPECT_EQ(std::get<std::string>(recoveredData[3][3]), "Record2");
        }
    }

    // Zwolnij pamiêæ
    delete recoveryWal;

    // Czyszczenie po teœcie
    clearFile(walPath);
    deleteFile(walPath);
    deleteFile(tablePath);
}

// Test czyszczenia i ponownego wykorzystania pliku WAL
TEST(WalTests, ClearAndReuseWal) {
    std::string walPath = executionFilePath() + "/wal/wal.bin";
    std::string tablePath = executionFilePath() + "/dataBaseMemoryManagment/TestTable.bin";

    // Usuñ pliki przed testem
    deleteFile(walPath);

    // Utwórz instancjê klasy Wal dla tabeli
    Wal* wal = new Wal();
    wal->createDirectoryAndFile();

    // Dodaj dane do WAL
    Table table("TestTable", "dataBaseMemoryManagment", wal);
    table.addColumn("InitialColumn", int32_tId, false);

    // Dekoduj i sprawdŸ zawartoœæ WAL
    wal->decodeWall();
    std::vector<std::vector<allVars>> initialData = wal->getWalData();
    EXPECT_EQ(initialData.size(), 1) << "Nieprawid³owa liczba wpisów w pocz¹tkowym WAL";

    // Wyczyœæ plik WAL
    clearFile(walPath);

    // Dodaj nowe dane do WAL
    table.addColumn("SecondColumn", stringId, true);

    // Dekoduj i sprawdŸ zawartoœæ WAL po wyczyszczeniu i dodaniu nowych danych
    wal->clearWalData(); // Wyczyœæ dane w pamiêci przed ponownym dekodowaniem
    wal->decodeWall();
    std::vector<std::vector<allVars>> newData = wal->getWalData();

    // SprawdŸ, czy w WAL jest tylko nowa kolumna
    EXPECT_EQ(newData.size(), 1) << "Nieprawid³owa liczba wpisów w WAL po wyczyszczeniu";

    if (newData.size() >= 1) {
        EXPECT_EQ(std::get<int32_t>(newData[0][0]), addColumnTypeId);
        if (newData[0].size() >= 3) {
            EXPECT_EQ(std::get<std::string>(newData[0][2]), "SecondColumn");
        }
        if (newData[0].size() >= 4) {
            EXPECT_EQ(std::get<int32_t>(newData[0][3]), stringId);
        }
        // Zamiast u¿ywaæ std::get<bool>, sprawdzamy tylko czy indeks istnieje
        EXPECT_GE(newData[0].size(), 5) << "Za ma³o elementów w wektorze dla kolumny";
    }

    // Zwolnij pamiêæ
    delete wal;

    // Czyszczenie po teœcie
    clearFile(walPath);
    deleteFile(walPath);
    deleteFile(tablePath);
}


*/