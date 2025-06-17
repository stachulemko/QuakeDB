#include "pch.h"
#include "table.h"
#include "wal.h"

// Resetuje przechwytywanie stdout aby uniknπÊ b≥Ídu "Only one stdout capturer can exist at a time"
void ResetStdoutCapture() {
    testing::internal::GetCapturedStdout();
}

TEST(TableTests, ConstructorWithValidParameters) {
    Wal* wal = nullptr; // Inicjalizacja wskaünika jako nullptr
    Table table("testTable", ".", wal);
    EXPECT_EQ(table.getTableName(), "testTable");
}

TEST(TableTests, ConstructorWithEmptyName) {
    Wal* wal = nullptr; // Inicjalizacja wskaünika jako nullptr
    Table table("", ".", wal);
    EXPECT_EQ(table.getTableName(), "");
}

TEST(TableTests, ConstructorWithLongName) {
    std::string longName(100, 'a');
    Wal* wal = nullptr; // Inicjalizacja wskaünika jako nullptr
    Table table(longName, ".", wal);
    EXPECT_EQ(table.getTableName(), longName);
}

TEST(TableTests, ConstructorWithSpecialChars) {
    std::string specialName = "test_special";
    Wal* wal = nullptr; // Inicjalizacja wskaünika jako nullptr
    Table table(specialName, ".", wal);
    EXPECT_EQ(table.getTableName(), specialName);
}

TEST(TableTests, GetTableNameTest) {
    Wal* wal = nullptr; // Inicjalizacja wskaünika jako nullptr
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
    ResetStdoutCapture();
    Wal* wal = nullptr; // Inicjalizacja wskaünika jako nullptr
    Table table("columnTypesTest", ".", wal);

    EXPECT_NO_THROW(table.addColumn("intColumn", 1, false));
    EXPECT_NO_THROW(table.addColumn("stringColumn", 3, false));
    EXPECT_NO_THROW(table.addColumn("nullableIntColumn", 1, true));

    testing::internal::CaptureStdout();
    table.showTable();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("intColumn") != std::string::npos);
    EXPECT_TRUE(output.find("stringColumn") != std::string::npos);
    EXPECT_TRUE(output.find("nullableIntColumn") != std::string::npos);
}

TEST(TableTests, AddColumnWithEmptyName) {
    ResetStdoutCapture();
    Wal* wal = nullptr; // Inicjalizacja wskaünika jako nullptr
    Table table("emptyColumnTest", ".", wal);
    EXPECT_NO_THROW(table.addColumn("", 1, false));

    testing::internal::CaptureStdout();
    table.showTable();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Name: ") != std::string::npos);
}

TEST(TableTests, AddColumnWithLongName) {
    ResetStdoutCapture();
    Wal* wal = nullptr; // Inicjalizacja wskaünika jako nullptr
    Table table("longColumnTest", ".", wal);
    std::string longColumnName(100, 'b');
    EXPECT_NO_THROW(table.addColumn(longColumnName, 1, false));

    testing::internal::CaptureStdout();
    table.showTable();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find(longColumnName) != std::string::npos);
}

TEST(TableTests, MultipleColumnsOrdering) {
    ResetStdoutCapture();
    Wal* wal = nullptr; // Inicjalizacja wskaünika jako nullptr
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
    ResetStdoutCapture();
    Wal* wal = nullptr; // Inicjalizacja wskaünika jako nullptr
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
    ResetStdoutCapture();
    Wal* wal = nullptr; // Inicjalizacja wskaünika jako nullptr
    Table table("emptyTable", ".", wal);

    testing::internal::CaptureStdout();
    table.showTable();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Table Name: emptyTable") != std::string::npos);
    EXPECT_TRUE(output.find("Columns:") != std::string::npos);
}

TEST(TableTests, ShowTableWithColumns) {
    ResetStdoutCapture();
    Wal* wal = nullptr; // Inicjalizacja wskaünika jako nullptr
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
    Wal* wal = nullptr; // Inicjalizacja wskaünika jako nullptr
    Table* table = new Table("emptyTable", ".", wal);
    EXPECT_NO_THROW(table->clearAll());
    EXPECT_NO_THROW(delete table);
}

TEST(TableTests, ClearAllMultipleTimes) {
    Wal* wal = nullptr; // Inicjalizacja wskaünika jako nullptr
    Table* table = new Table("multiClearTable", ".", wal);
    table->addColumn("col1", 1, false);
    EXPECT_NO_THROW(table->clearAll());
    EXPECT_NO_THROW(table->clearAll());
    EXPECT_NO_THROW(delete table);
}

TEST(TableTests, AddColumnsAfterClear) {
    ResetStdoutCapture();
    Wal* wal = nullptr; // Inicjalizacja wskaünika jako nullptr
    Table table("clearAndAdd", ".", wal);

    table.addColumn("col1", 1, false);
    table.clearAll();

    EXPECT_NO_THROW(table.addColumn("col2", 2, true));

    testing::internal::CaptureStdout();
    table.showTable();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_FALSE(output.find("col1") != std::string::npos);
    EXPECT_TRUE(output.find("col2") != std::string::npos);
}

TEST(TableTests, SameTableName) {
    ResetStdoutCapture();
    Wal* wal = nullptr; // Inicjalizacja wskaünika jako nullptr
    Table table1("duplicateNameTest", "./dir1", wal);
    Table table2("duplicateNameTest", "./dir2", wal);

    EXPECT_EQ(table1.getTableName(), table2.getTableName());

    table1.addColumn("col1", 1, false);
    table2.addColumn("col2", 2, true);

    testing::internal::CaptureStdout();
    table1.showTable();
    std::string output1 = testing::internal::GetCapturedStdout();

    ResetStdoutCapture();

    testing::internal::CaptureStdout();
    table2.showTable();
    std::string output2 = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output1.find("col1") != std::string::npos);
    EXPECT_FALSE(output1.find("col2") != std::string::npos);

    EXPECT_FALSE(output2.find("col1") != std::string::npos);
    EXPECT_TRUE(output2.find("col2") != std::string::npos);
}

TEST(TableTests, ColumnTypesValues) {
    ResetStdoutCapture();
    Wal* wal = nullptr; // Inicjalizacja wskaünika jako nullptr
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
    // Wszystkie úcieøki bÍdπ podkatalogami "."
    Wal* wal = nullptr; // Inicjalizacja wskaünika jako nullptr
    Table table1("pathTest1", "./subdir1", wal);
    EXPECT_EQ(table1.getTableName(), "pathTest1");

    Table table2("pathTest2", "./subdir2", wal);
    EXPECT_EQ(table2.getTableName(), "pathTest2");

    Table table3("pathTest3", ".", wal);
    EXPECT_EQ(table3.getTableName(), "pathTest3");
}

