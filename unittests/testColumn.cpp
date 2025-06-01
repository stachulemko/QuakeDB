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

// Test metody SetColumn
TEST(ColumnTests, SetColumn) {
    // Najpierw tworzymy kolumnê z pocz¹tkow¹ wartoœci¹
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

// Test metody getColumnSize
TEST(ColumnTests, GetColumnSize) {
    std::string name = "Test";
    int32_t columnType = 1;
    bool allowNull = false;

    Column column(name, columnType, allowNull);

    // Spodziewany rozmiar to suma d³ugoœci TLV plus 4 bajty na rozmiar kolumny
    int32_t expectedSize = column.getColumnSize();
    EXPECT_GT(expectedSize, 4); // Rozmiar powinien byæ wiêkszy ni¿ 4 bajty
}

// Test metody getColumnName
TEST(ColumnTests, GetColumnName) {
    std::string name = "TestColumn";
    Column column(name, 1, false);

    EXPECT_EQ(column.getColumnName(), name);

    // Test dla pustej nazwy
    Column emptyNameColumn("", 1, false);
    EXPECT_EQ(emptyNameColumn.getColumnName(), "");
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
    Column decodedColumn("", 0, false); // Pusta kolumna do dekodowania
    decodedColumn.loadAllBytesToDecode(marshalledData);
    decodedColumn.decodeColumn();

    // Sprawdzanie, czy dekodowane wartoœci s¹ zgodne z orygina³em
    EXPECT_EQ(decodedColumn.getColumnName(), name);
    EXPECT_EQ(decodedColumn.getColumnType(), columnType);
    EXPECT_EQ(decodedColumn.isAllowNull(), allowNull);
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

    Column decodedLongName("", 0, false);
    decodedLongName.loadAllBytesToDecode(longNameData);
    decodedLongName.decodeColumn();

    EXPECT_EQ(decodedLongName.getColumnName(), longName);

    // Kolumna z maksymaln¹ wartoœci¹ typu
    int32_t maxType = std::numeric_limits<int32_t>::max();
    Column maxTypeColumn("Test", maxType, false);
    std::vector<uint8_t> maxTypeData = maxTypeColumn.MarshalColumn();

    Column decodedMaxType("", 0, false);
    decodedMaxType.loadAllBytesToDecode(maxTypeData);
    decodedMaxType.decodeColumn();

    EXPECT_EQ(decodedMaxType.getColumnType(), maxType);
}

// Test dla nieprawid³owych danych wejœciowych podczas dekodowania
TEST(ColumnTests, InvalidDecoding) {
    // Próba dekodowania pustego wektora
    Column column("", 0, false);
    std::vector<uint8_t> emptyData;
    column.loadAllBytesToDecode(emptyData);
    column.decodeColumn();

    // Nie powinno to koñczyæ siê awari¹, a jedynie zwróciæ wartoœci domyœlne
    EXPECT_EQ(column.getColumnName(), "");
    EXPECT_EQ(column.getColumnType(), -1);
    EXPECT_FALSE(column.isAllowNull());

    // Próba dekodowania zbyt ma³ego wektora (mniej ni¿ 8 bajtów)
    std::vector<uint8_t> tooSmallData = { 1, 2, 3, 4, 5, 6, 7 };
    column.loadAllBytesToDecode(tooSmallData);
    column.decodeColumn();

    // Nie powinno to koñczyæ siê awari¹
    EXPECT_EQ(column.getColumnName(), "");
}

// Test dla specjalnych znaków w nazwie kolumny
TEST(ColumnTests, SpecialCharacters) {
    std::string specialChars = "!@#$%^&*()_+{}:\"|<>?[];',./";
    Column column(specialChars, 1, false);
    std::vector<uint8_t> data = column.MarshalColumn();

    Column decoded("", 0, false);
    decoded.loadAllBytesToDecode(data);
    decoded.decodeColumn();

    EXPECT_EQ(decoded.getColumnName(), specialChars);
}

// Test dla pustej nazwy kolumny
TEST(ColumnTests, EmptyName) {
    Column column("", 1, false);
    std::vector<uint8_t> data = column.MarshalColumn();

    Column decoded("NotEmpty", 0, false);
    decoded.loadAllBytesToDecode(data);
    decoded.decodeColumn();

    EXPECT_EQ(decoded.getColumnName(), "");
}
