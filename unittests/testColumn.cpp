#include "pch.h"
#include "column.h"

// Test konstruktora
TEST(ColumnTests, Constructor) {
    // Test konstruktora dla kolumny z ró¿nymi wartoœciami
    std::string name = "TestColumn";
    int32_t columnType = 2;
    bool allowNull = true;

    Column column(name, columnType, allowNull);

    EXPECT_EQ(column.getColumnName(), name);
    EXPECT_EQ(column.getColumnType(), columnType);
    EXPECT_EQ(column.isAllowNull(), allowNull);
    EXPECT_GT(column.getColumnSize(), 0);
}

// Test konstruktora z pust¹ nazw¹ - powinien rzuciæ asercjê
TEST(ColumnTests, ConstructorWithEmptyName) {
#ifdef NDEBUG
    // W trybie release asercje s¹ wy³¹czone, wiêc test pomijamy
    SUCCEED() << "Asercje s¹ wy³¹czone w trybie release, test pominiêty";
#else
    // W trybie debug asercja powinna przerwaæ dzia³anie
    EXPECT_DEATH({
        Column emptyColumn("", 1, false);
        }, "column string name is empty");
#endif
}

// Test metody SetColumn
TEST(ColumnTests, SetColumn) {
    // Najpierw tworzymy kolumnê z poprawn¹ wartoœci¹
    Column column("Initial", 1, false);

    // Nastêpnie zmieniamy wartoœci
    std::string newName = "Updated";
    int32_t newType = 3;
    bool newAllowNull = true;

    column.SetColumn(newName, newType, newAllowNull);

    // Sprawdzamy, czy wartoœci zosta³y poprawnie zaktualizowane
    EXPECT_EQ(column.getColumnName(), newName);
    EXPECT_EQ(column.getColumnType(), newType);
    EXPECT_EQ(column.isAllowNull(), newAllowNull);
}

// Test metody SetColumn z pust¹ nazw¹ - w odró¿nieniu od konstruktora, SetColumn pozwala na pust¹ nazwê
TEST(ColumnTests, SetColumnWithEmptyName) {
    Column column("Initial", 1, false);

    // SetColumn nie ma asercji na pust¹ nazwê
    column.SetColumn("", 2, true);

    // Sprawdzamy czy pusta nazwa zosta³a zaakceptowana
    EXPECT_EQ(column.getColumnName(), "");
    EXPECT_EQ(column.getColumnType(), 2);
    EXPECT_EQ(column.isAllowNull(), true);
}

// Test metody getColumnSize
TEST(ColumnTests, GetColumnSize) {
    std::string name = "Test";
    int32_t columnType = 1;
    bool allowNull = false;

    Column column(name, columnType, allowNull);

    int32_t expectedSize = column.getColumnSize();
    EXPECT_GT(expectedSize, 4); // Rozmiar powinien byæ wiêkszy ni¿ 4 bajty
}

// Test metody getColumnName
TEST(ColumnTests, GetColumnName) {
    std::string name = "TestColumn";
    Column column(name, 1, false);

    EXPECT_EQ(column.getColumnName(), name);

    // Testujemy tak¿e po modyfikacji SetColumn z pust¹ nazw¹
    Column modifiableColumn("NotEmpty", 1, false);
    modifiableColumn.SetColumn("", 1, false);
    EXPECT_EQ(modifiableColumn.getColumnName(), "");
}

// Test metody getColumnType
TEST(ColumnTests, GetColumnType) {
    int32_t columnType = 5;
    Column column("Test", columnType, false);

    EXPECT_EQ(column.getColumnType(), columnType);
}

// Test metody isAllowNull
TEST(ColumnTests, IsAllowNull) {
    // Test dla allowNull = true
    Column columnWithNull("Test", 1, true);
    EXPECT_TRUE(columnWithNull.isAllowNull());

    // Test dla allowNull = false
    Column columnWithoutNull("Test", 1, false);
    EXPECT_FALSE(columnWithoutNull.isAllowNull());
}

// Test metody marshaling i dekodowania
TEST(ColumnTests, MarshalAndDecode) {
    std::string name = "TestColumn";
    int32_t columnType = 2;
    bool allowNull = true;

    // Tworzenie i marshalling kolumny
    Column column(name, columnType, allowNull);
    std::vector<uint8_t> marshalledData = column.MarshalColumn();

    // Dekodowanie marsza³kowanych danych
    Column decodedColumn("NonEmpty", 0, false); // U¿ywamy niepustej nazwy, aby unikn¹æ asercji
    decodedColumn.loadAllBytesToDecode(marshalledData);
    decodedColumn.decodeColumn();

    // Sprawdzanie, czy dekodowane wartoœci s¹ zgodne z orygina³em
    EXPECT_EQ(decodedColumn.getColumnName(), name);
    EXPECT_EQ(decodedColumn.getColumnType(), columnType);
    EXPECT_EQ(decodedColumn.isAllowNull(), allowNull);
}

// Test metody marshaling i dekodowania dla pustej nazwy kolumny, który unika asercji
TEST(ColumnTests, MarshalAndDecodeEmptyName) {
    // Zamiast tworzyæ kolumnê z pust¹ nazw¹, u¿ywamy SetColumn
    Column column("NonEmpty", 1, false);
    column.SetColumn("", 2, true);

    std::vector<uint8_t> marshalledData = column.MarshalColumn();

    // Dekodowanie marsza³kowanych danych
    Column decodedColumn("NonEmpty", 0, false);
    decodedColumn.loadAllBytesToDecode(marshalledData);
    decodedColumn.decodeColumn();

    // Sprawdzanie, czy pusta nazwa zosta³a poprawnie zdekodowana
    EXPECT_EQ(decodedColumn.getColumnName(), "");
    EXPECT_EQ(decodedColumn.getColumnType(), 2);
    EXPECT_EQ(decodedColumn.isAllowNull(), true);
}

// Test metody clearAll
TEST(ColumnTests, ClearAll) {
    Column column("Test", 1, true);
    column.clearAll();

    // Po wyczyszczeniu powinniœmy dostaæ wartoœci domyœlne/puste
    EXPECT_EQ(column.getColumnName(), "");
    EXPECT_EQ(column.getColumnType(), -1); // -1 to wartoœæ domyœlna dla nietypowej kolumny
}

// Test dla szczególnych przypadków
TEST(ColumnTests, EdgeCases) {
    // Kolumna z bardzo d³ug¹ nazw¹
    std::string longName(1000, 'a');
    Column longNameColumn(longName, 1, false);
    std::vector<uint8_t> longNameData = longNameColumn.MarshalColumn();

    Column decodedLongName("NonEmpty", 0, false);
    decodedLongName.loadAllBytesToDecode(longNameData);
    decodedLongName.decodeColumn();

    EXPECT_EQ(decodedLongName.getColumnName(), longName);

    // Kolumna z maksymaln¹ wartoœci¹ typu
    int32_t maxType = std::numeric_limits<int32_t>::max();
    Column maxTypeColumn("Test", maxType, false);
    std::vector<uint8_t> maxTypeData = maxTypeColumn.MarshalColumn();

    Column decodedMaxType("NonEmpty", 0, false);
    decodedMaxType.loadAllBytesToDecode(maxTypeData);
    decodedMaxType.decodeColumn();

    EXPECT_EQ(decodedMaxType.getColumnType(), maxType);
}

// Test dla nieprawid³owych danych wejœciowych podczas dekodowania
TEST(ColumnTests, InvalidDecoding) {
    // Aby unikn¹æ asercji, u¿ywamy niepustej nazwy
    Column column("NonEmpty", 0, false);
    std::vector<uint8_t> emptyData;
    column.loadAllBytesToDecode(emptyData);
    column.decodeColumn();

    // Po dekodowaniu pustych danych, wartoœci powinny pozostaæ bez zmian
    EXPECT_EQ(column.getColumnName(), "NonEmpty");
    EXPECT_EQ(column.getColumnType(), 0);
    EXPECT_FALSE(column.isAllowNull());

    // Próba dekodowania zbyt ma³ego wektora (mniej ni¿ 8 bajtów)
    std::vector<uint8_t> tooSmallData = { 1, 2, 3, 4, 5, 6, 7 };
    column.loadAllBytesToDecode(tooSmallData);
    column.decodeColumn();

    // Nie powinno to koñczyæ siê awari¹
    EXPECT_EQ(column.getColumnName(), "NonEmpty");
}

// Test dla specjalnych znaków w nazwie kolumny
TEST(ColumnTests, SpecialCharacters) {
    std::string specialChars = "!@#$%^&*()_+{}:\"|<>?[];',./";
    Column column(specialChars, 1, false);
    std::vector<uint8_t> data = column.MarshalColumn();

    Column decoded("NonEmpty", 0, false);
    decoded.loadAllBytesToDecode(data);
    decoded.decodeColumn();

    EXPECT_EQ(decoded.getColumnName(), specialChars);
}

// Test dla sztucznego utworzenia pustej nazwy kolumny (bez wywo³ywania asercji)
TEST(ColumnTests, EmptyNameViaSetColumn) {
    // Tworzenie kolumny z niepust¹ nazw¹
    Column column("NonEmpty", 1, false);

    // Modyfikacja na pust¹ nazwê
    column.SetColumn("", 1, false);

    // Sprawdzenie czy pusta nazwa jest akceptowana przez SetColumn
    EXPECT_EQ(column.getColumnName(), "");

    // Marshalling kolumny z pust¹ nazw¹
    std::vector<uint8_t> data = column.MarshalColumn();

    // Dekodowanie
    Column decoded("NonEmpty", 0, false);
    decoded.loadAllBytesToDecode(data);
    decoded.decodeColumn();

    // Sprawdzenie czy pusta nazwa zosta³a poprawnie zdekodowana
    EXPECT_EQ(decoded.getColumnName(), "");
}
