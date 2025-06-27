
#include "column.h"
#include "tlv.h"
#include "dataBase.h"
int main() {
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
    /*
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
    */
}
