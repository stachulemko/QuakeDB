#include "pch.h"
#include "column.h"


TEST(ColumnTests, Constructor) {
    Column column("TestColumn", 2, true);
    EXPECT_EQ(column.getColumnName(), "TestColumn");
    EXPECT_EQ(column.getColumnType(), 2);
    EXPECT_TRUE(column.isAllowNull());

    Column column2("AnotherColumn", 3, false);
    EXPECT_EQ(column2.getColumnName(), "AnotherColumn");
    EXPECT_EQ(column2.getColumnType(), 3);
    EXPECT_FALSE(column2.isAllowNull());
}

TEST(ColumnTests, SetColumn) {
    Column column("InitialName", 1, true);
    column.SetColumn("NewName", 4, false);

    EXPECT_EQ(column.getColumnName(), "NewName");
    EXPECT_EQ(column.getColumnType(), 4);
    EXPECT_FALSE(column.isAllowNull());
}


TEST(ColumnTests, MarshalAndDecode) {
    Column column("TestColumn", 5, true);
    std::vector<uint8_t> bytes = column.MarshalColumn();
    EXPECT_FALSE(bytes.empty());
    Column decodedColumn("", 0, false);
    decodedColumn.loadloadAllBytesToDecode(bytes);
    decodedColumn.decodeColumn();
    EXPECT_EQ(decodedColumn.getColumnName(), "TestColumn");
    EXPECT_EQ(decodedColumn.getColumnType(), 5);
    EXPECT_TRUE(decodedColumn.isAllowNull());
}

TEST(ColumnTests, DifferentColumnTypes) {
    Column intColumn("IntColumn", 1, false);
    EXPECT_EQ(intColumn.getColumnType(), 1);

    Column varcharColumn("VarcharColumn", 2, true);
    EXPECT_EQ(varcharColumn.getColumnType(), 2);

    Column datetimeColumn("DateTimeColumn", 3, false);
    EXPECT_EQ(datetimeColumn.getColumnType(), 3);
}

TEST(ColumnTests, EmptyColumnName) {
    Column column("", 1, true);
    EXPECT_EQ(column.getColumnName(), "");

    std::vector<uint8_t> bytes = column.MarshalColumn();

    Column decodedColumn("NotEmpty", 0, false);
    decodedColumn.loadloadAllBytesToDecode(bytes);
    decodedColumn.decodeColumn();

    EXPECT_EQ(decodedColumn.getColumnName(), "");
}

TEST(ColumnTests, AllowNullFlag) {
    Column nullableColumn("Nullable", 1, true);
    EXPECT_TRUE(nullableColumn.isAllowNull());

    std::vector<uint8_t> bytes1 = nullableColumn.MarshalColumn();

    Column decodedNullable("", 0, false);
    decodedNullable.loadloadAllBytesToDecode(bytes1);
    decodedNullable.decodeColumn();

    EXPECT_TRUE(decodedNullable.isAllowNull());

    Column nonNullableColumn("NonNullable", 1, false);
    EXPECT_FALSE(nonNullableColumn.isAllowNull());

    std::vector<uint8_t> bytes2 = nonNullableColumn.MarshalColumn();

    Column decodedNonNullable("", 0, true);
    decodedNonNullable.loadloadAllBytesToDecode(bytes2);
    decodedNonNullable.decodeColumn();

    EXPECT_FALSE(decodedNonNullable.isAllowNull());
}

TEST(ColumnTests, LongColumnNames) {
    std::string longName(100, 'a');
    Column column(longName, 1, true);
    EXPECT_EQ(column.getColumnName(), longName);

    std::vector<uint8_t> bytes = column.MarshalColumn();

    Column decodedColumn("", 0, false);
    decodedColumn.loadloadAllBytesToDecode(bytes);
    decodedColumn.decodeColumn();

    EXPECT_EQ(decodedColumn.getColumnName(), longName);
}

TEST(ColumnTests, SpecialCharactersInName) {
    std::string specialName = "Test!@#$%^&*()_+{}:|<>?[];',./";
    Column column(specialName, 1, true);
    EXPECT_EQ(column.getColumnName(), specialName);

    std::vector<uint8_t> bytes = column.MarshalColumn();

    Column decodedColumn("", 0, false);
    decodedColumn.loadloadAllBytesToDecode(bytes);
    decodedColumn.decodeColumn();

    EXPECT_EQ(decodedColumn.getColumnName(), specialName);
}

TEST(ColumnTests, InvalidData) {
    Column column("Test", 1, true);
    column.loadloadAllBytesToDecode(std::vector<uint8_t>());
    column.decodeColumn();

    EXPECT_EQ(column.getColumnName(), "Test");
    EXPECT_EQ(column.getColumnType(), 1);
    EXPECT_TRUE(column.isAllowNull());

    std::vector<uint8_t> tooSmall = { 0, 0, 0, 1 };
    column.loadloadAllBytesToDecode(tooSmall);
    column.decodeColumn();

    EXPECT_EQ(column.getColumnName(), "Test");
    EXPECT_EQ(column.getColumnType(), 1);
    EXPECT_TRUE(column.isAllowNull());
}

TEST(ColumnTests, ClearAll) {
    Column* column = new Column("TestColumn", 1, true);

    EXPECT_EQ(column->getColumnName(), "TestColumn");

    delete column;
}

TEST(ColumnTests, MultipleSetOperations) {
    Column column("First", 1, true);

    column.SetColumn("Second", 2, false);
    EXPECT_EQ(column.getColumnName(), "Second");
    EXPECT_EQ(column.getColumnType(), 2);
    EXPECT_FALSE(column.isAllowNull());

    column.SetColumn("Third", 3, true);
    EXPECT_EQ(column.getColumnName(), "Third");
    EXPECT_EQ(column.getColumnType(), 3);
    EXPECT_TRUE(column.isAllowNull());

    std::vector<uint8_t> bytes = column.MarshalColumn();

    Column decodedColumn("", 0, false);
    decodedColumn.loadloadAllBytesToDecode(bytes);
    decodedColumn.decodeColumn();

    EXPECT_EQ(decodedColumn.getColumnName(), "Third");
    EXPECT_EQ(decodedColumn.getColumnType(), 3);
    EXPECT_TRUE(decodedColumn.isAllowNull());
}
