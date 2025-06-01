#include "pch.h"
#include "table.h"

// Resetuje przechwytywanie stdout aby unikn¹æ b³êdu "Only one stdout capturer can exist at a time"
void ResetStdoutCapture() {
    testing::internal::GetCapturedStdout();
}

TEST(TableTests, ConstructorWithValidParameters) {
    Table table("testTable", ".");
    EXPECT_EQ(table.getTableName(), "testTable");
}

TEST(TableTests, ConstructorWithEmptyName) {
    Table table("", ".");
    EXPECT_EQ(table.getTableName(), "");
}

TEST(TableTests, ConstructorWithLongName) {
    std::string longName(100, 'a');
    Table table(longName, ".");
    EXPECT_EQ(table.getTableName(), longName);
}

TEST(TableTests, ConstructorWithSpecialChars) {
    std::string specialName = "test_special";
    Table table(specialName, ".");
    EXPECT_EQ(table.getTableName(), specialName);
}

TEST(TableTests, GetTableNameTest) {
    {
        Table table("normalName", ".");
        EXPECT_EQ(table.getTableName(), "normalName");
    }

    {
        Table table("", ".");
        EXPECT_EQ(table.getTableName(), "");
    }

    {
        Table table("name with spaces", ".");
        EXPECT_EQ(table.getTableName(), "name with spaces");
    }
}

TEST(TableTests, AddColumnBasicTypes) {
    // Najpierw resetujemy stdout
    ResetStdoutCapture();

    Table table("columnTypesTest", ".");

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

    Table table("emptyColumnTest", ".");
    EXPECT_NO_THROW(table.addColumn("", 1, false));

    testing::internal::CaptureStdout();
    table.showTable();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Name: ") != std::string::npos);
}

TEST(TableTests, AddColumnWithLongName) {
    ResetStdoutCapture();

    Table table("longColumnTest", ".");
    std::string longColumnName(100, 'b');
    EXPECT_NO_THROW(table.addColumn(longColumnName, 1, false));

    testing::internal::CaptureStdout();
    table.showTable();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find(longColumnName) != std::string::npos);
}

TEST(TableTests, MultipleColumnsOrdering) {
    ResetStdoutCapture();

    Table table("orderingTest", ".");

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

    Table table("manyColumnsTest", ".");

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

    Table table("emptyTable", ".");

    testing::internal::CaptureStdout();
    table.showTable();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Table Name: emptyTable") != std::string::npos);
    EXPECT_TRUE(output.find("Columns:") != std::string::npos);
}

TEST(TableTests, ShowTableWithColumns) {
    ResetStdoutCapture();

    Table table("populatedTable", ".");

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
    Table* table = new Table("emptyTable", ".");
    EXPECT_NO_THROW(table->clearAll());
    EXPECT_NO_THROW(delete table);
}

TEST(TableTests, ClearAllMultipleTimes) {
    Table* table = new Table("multiClearTable", ".");
    table->addColumn("col1", 1, false);
    EXPECT_NO_THROW(table->clearAll());
    EXPECT_NO_THROW(table->clearAll());
    EXPECT_NO_THROW(delete table);
}

TEST(TableTests, AddColumnsAfterClear) {
    ResetStdoutCapture();

    Table table("clearAndAdd", ".");

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

    Table table1("duplicateNameTest", "./dir1");
    Table table2("duplicateNameTest", "./dir2");

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

    Table table("typesTest", ".");

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
    Table table1("pathTest1", "./subdir1");
    EXPECT_EQ(table1.getTableName(), "pathTest1");

    Table table2("pathTest2", "./subdir2");
    EXPECT_EQ(table2.getTableName(), "pathTest2");

    Table table3("pathTest3", ".");
    EXPECT_EQ(table3.getTableName(), "pathTest3");
}
