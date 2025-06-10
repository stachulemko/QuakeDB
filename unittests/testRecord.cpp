#include "pch.h"
#include "record.h"
#include "column.h"
#include "typeManager.h"

// Helper function to create test columns
std::vector<Column*> createTestColumns() {
    std::vector<Column*> columns;
    columns.push_back(new Column("IntColumn", 1, false));   // int32_t, NOT NULL
    columns.push_back(new Column("LongColumn", 2, true));   // int64_t, NULL
    columns.push_back(new Column("StringColumn", 3, false)); // string, NOT NULL
    return columns;
}

void cleanupColumns(std::vector<Column*>& columns) {
    for (auto col : columns) {
        delete col;
    }
    columns.clear();
}

// Test record construction with valid data
TEST(RecordTests, ConstructWithValidData) {
    // Create test record data
    std::vector<allVars> recordData = {
        42,                     // int32_t
        static_cast<int64_t>(123456789012345), // int64_t
        std::string("Test")     // string
    };

    // Create matching columns
    std::vector<Column*> columns = createTestColumns();

    // Create record and check validation
    Record* record = nullptr;
    EXPECT_NO_THROW(record = new Record(recordData, columns));
    EXPECT_TRUE(record->isDataTypeCorrect(recordData, columns));

    // Clean up
    delete record;
    cleanupColumns(columns);
}

// Test record validation with mismatched data types - u¿ywaj¹c ASSERT_DEATH
TEST(RecordTests, ValidateWithIncorrectTypes) {
    testing::FLAGS_gtest_death_test_style = "threadsafe"; // Ustaw tryb testu œmierci

    // Create columns
    std::vector<Column*> columns = createTestColumns();

    // Create record data with incorrect types
    std::vector<allVars> wrongData = {
        std::string("Wrong"), // String instead of int32_t
        42,                   // int32_t instead of int64_t
        static_cast<int64_t>(123) // int64_t instead of string
    };

    // Sprawdzamy czy funkcja isDataTypeCorrect zg³asza asercjê
    // dla niepoprawnych danych - jeœli tak, test powinien przejœæ
    ASSERT_DEATH({
        Record tempRecord({ 0, static_cast<int64_t>(0), std::string("") }, columns);
        tempRecord.isDataTypeCorrect(wrongData, columns);
        }, ".*") << "Test ZDANY: asercja zosta³a zg³oszona dla nieprawid³owych typów danych";

    // Clean up
    cleanupColumns(columns);
}

// Test na asercjê przy tworzeniu rekordu z pustym stringiem
TEST(RecordTests, EmptyStringAssertion) {
    testing::FLAGS_gtest_death_test_style = "threadsafe";

    // Create columns
    std::vector<Column*> columns;
    columns.push_back(new Column("StringCol", 3, false)); // string, NOT NULL

    // Tworzymy rekord z pustym stringiem - powinien byæ assert
    std::vector<allVars> dataWithEmptyString = {
        std::string("") // Pusty string
    };

    // Sprawdzamy czy zostanie zg³oszona asercja
    ASSERT_DEATH({
        Record * record = new Record(dataWithEmptyString, columns);
        }, ".*") << "Test ZDANY: asercja zosta³a zg³oszona dla pustego stringa";

    // Clean up
    cleanupColumns(columns);
}

// Test record validation with wrong number of columns - u¿ywaj¹c ASSERT_DEATH
TEST(RecordTests, ValidateWithWrongColumnCount) {
    testing::FLAGS_gtest_death_test_style = "threadsafe";

    // Create columns
    std::vector<Column*> columns = createTestColumns();

    // Create record data with too few fields
    std::vector<allVars> tooFewData = {
        42,
        static_cast<int64_t>(12345)
        // Missing the string field
    };

    // Sprawdzamy czy zostanie zg³oszona asercja dla niepasuj¹cej liczby kolumn
    ASSERT_DEATH({
        Record tempRecord({ 0, static_cast<int64_t>(0), std::string("") }, columns);
        tempRecord.isDataTypeCorrect(tooFewData, columns);
        }, ".*") << "Test ZDANY: asercja zosta³a zg³oszona dla nieprawid³owej liczby kolumn";

    // Clean up
    cleanupColumns(columns);
}

// Test record serialization (Marshal)
TEST(RecordTests, MarshallRecord) {
    // Create test record data
    std::vector<allVars> recordData = {
        42,                     // int32_t
        static_cast<int64_t>(123456789012345), // int64_t
        std::string("Test")     // string
    };

    // Create columns
    std::vector<Column*> columns = createTestColumns();

    // Create record
    Record record(recordData, columns);

    // Marshal record
    std::vector<uint8_t> bytes = record.MarshalRecord();

    // Check that the marshalled data is not empty
    EXPECT_FALSE(bytes.empty());

    // Clean up
    cleanupColumns(columns);
}

// Test constructing record with empty data
TEST(RecordTests, ConstructWithEmptyData) {
    // Create empty record data and empty columns
    std::vector<allVars> emptyData;
    std::vector<Column*> emptyColumns;

    // Create record
    Record* record = nullptr;
    EXPECT_NO_THROW(record = new Record(emptyData, emptyColumns));

    // Check that the record validates as empty
    EXPECT_TRUE(record->isDataTypeCorrect(emptyData, emptyColumns));

    // Marshal empty record
    std::vector<uint8_t> bytes = record->MarshalRecord();

    // Clean up
    delete record;
}

// Test record with NULL values
TEST(RecordTests, RecordWithNullValues) {
    // Create columns with NULL allowances
    std::vector<Column*> columns;
    columns.push_back(new Column("IntColumn", 1, false)); // int32_t, NOT NULL
    columns.push_back(new Column("NullableInt", 1, true)); // int32_t, NULL

    // Create record data with a NULL value (represented as default constructed int)
    std::vector<allVars> dataWithNull = {
        42,         // Normal int32_t
        0           // "NULL" int32_t
    };

    // Create record
    Record* record = new Record(dataWithNull, columns);

    // Validation should pass because second column allows NULL
    EXPECT_TRUE(record->isDataTypeCorrect(dataWithNull, columns));

    // Clean up
    delete record;
    cleanupColumns(columns);
}

// Test boundary values in record
TEST(RecordTests, BoundaryValues) {
    // Create record data with boundary values
    std::vector<allVars> boundaryData = {
        std::numeric_limits<int32_t>::min(),
        static_cast<int64_t>(std::numeric_limits<int64_t>::max()),
        std::string(1000, 'a')  // Very long string
    };

    // Create matching columns
    std::vector<Column*> columns = createTestColumns();

    // Create record
    Record* record = new Record(boundaryData, columns);

    // Validation should pass
    EXPECT_TRUE(record->isDataTypeCorrect(boundaryData, columns));

    // Marshal record
    std::vector<uint8_t> bytes = record->MarshalRecord();

    // Check that the marshalled data is not empty
    EXPECT_FALSE(bytes.empty());

    // Clean up
    delete record;
    cleanupColumns(columns);
}

// Test with mixed record types
TEST(RecordTests, MixedRecordTypes) {
    // Create columns with mixed types
    std::vector<Column*> mixedColumns;
    mixedColumns.push_back(new Column("StringCol", 3, false)); // string, NOT NULL
    mixedColumns.push_back(new Column("IntCol", 1, false));    // int32_t, NOT NULL
    mixedColumns.push_back(new Column("LongCol", 2, false));   // int64_t, NOT NULL

    // Create matching record data
    std::vector<allVars> mixedData = {
        std::string("Hello"),
        42,
        static_cast<int64_t>(123456789012345)
    };

    // Create record
    Record* record = new Record(mixedData, mixedColumns);

    // Validation should pass
    EXPECT_TRUE(record->isDataTypeCorrect(mixedData, mixedColumns));

    // Clean up
    delete record;
    cleanupColumns(mixedColumns);
}

// Test special string values - zamieniony na test asercji
TEST(RecordTests, SpecialStringValues) {
    testing::FLAGS_gtest_death_test_style = "threadsafe";

    // Create a column for string
    std::vector<Column*> columns;
    columns.push_back(new Column("StringCol", 3, false)); // string, NOT NULL

    // Empty string powinien wywo³aæ asercjê
    std::vector<allVars> specialStrings = {
        std::string(""), // Empty string
    };

    // Sprawdzamy czy jest asercja dla pustego stringa
    ASSERT_DEATH({
        Record record(specialStrings, columns);
        }, ".*") << "Test ZDANY: asercja zosta³a zg³oszona dla pustego stringa";

    // Stringi ze znakami specjalnymi powinny dzia³aæ normalnie
    std::vector<allVars> specialChars = {
        std::string("!@#$%^&*()_+{}:\"|<>?[];',./")
    };

    Record* record2 = nullptr;
    EXPECT_NO_THROW(record2 = new Record(specialChars, columns));
    EXPECT_TRUE(record2->isDataTypeCorrect(specialChars, columns));

    // Clean up
    delete record2;
    cleanupColumns(columns);
}

// Test for handling very large records
TEST(RecordTests, LargeRecord) {
    // Create a large number of columns
    std::vector<Column*> manyColumns;
    const int numColumns = 50; // Test with 50 columns

    for (int i = 0; i < numColumns; i++) {
        // Alternate between types
        int type = (i % 3) + 1; // Types 1, 2, 3
        manyColumns.push_back(new Column("Column" + std::to_string(i), type, false));
    }

    // Create matching record data
    // Create matching record data
    std::vector<allVars> manyValues;

    for (int i = 0; i < numColumns; i++) {
        switch (i % 3) {
        case 0: // int32_t
            manyValues.push_back(i);
            break;
        case 1: // int64_t
            manyValues.push_back(static_cast<int64_t>(i * 1000000));
            break;
        case 2: // string
            manyValues.push_back(std::string("Value" + std::to_string(i))); // Niepusty string
            break;
        }
    }

    // Create record
    Record* record = nullptr;
    EXPECT_NO_THROW(record = new Record(manyValues, manyColumns));

    // Validation should pass
    EXPECT_TRUE(record->isDataTypeCorrect(manyValues, manyColumns));

    // Marshal record
    std::vector<uint8_t> bytes;
    EXPECT_NO_THROW(bytes = record->MarshalRecord());

    // Check that we got data back
    EXPECT_FALSE(bytes.empty());

    // Clean up
    delete record;
    cleanupColumns(manyColumns);
}

// Test asercji przy dekodowaniu rekordu z nieprawid³owym rozmiarem
TEST(RecordTests, InvalidTlvSizeAssertion) {
    testing::FLAGS_gtest_death_test_style = "threadsafe";

    // Tworzymy nieprawid³owe dane TLV (za ma³y rozmiar)
    std::vector<uint8_t> invalidTlvData = {
        // Typ rekordu (5)
        0x05, 0x00, 0x00, 0x00,
        // Rozmiar (20)
        0x14, 0x00, 0x00, 0x00,
        // Typ TLV (1 - int32_t)
        0x01, 0x00, 0x00, 0x00,
        // D³ugoœæ TLV (100 - zbyt du¿a dla pozosta³ych bajtów)
        0x64, 0x00, 0x00, 0x00
        // Brak danych dla tej d³ugoœci
    };

    // Sprawdzamy czy zostanie zg³oszona asercja
    Record record;
    record.loadAllConnectedBytes(invalidTlvData);

    ASSERT_DEATH({
        record.decode();
        }, ".*") << "Test ZDANY: asercja zosta³a zg³oszona dla nieprawid³owego rozmiaru TLV";
}
