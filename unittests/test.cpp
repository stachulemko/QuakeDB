#include "pch.h"
#include "binaryConverter.h"

TEST(BinaryConverterTests, MarshalInt32_t) {

    int32_t positiveValue = 123456;
    std::vector<uint8_t>* positiveBytes = marshalInt32_t(positiveValue);
    EXPECT_EQ(positiveBytes->size(), 4);
    EXPECT_EQ((*positiveBytes)[0], 0x40);  
    EXPECT_EQ((*positiveBytes)[1], 0xE2);  
    EXPECT_EQ((*positiveBytes)[2], 0x01);  
    EXPECT_EQ((*positiveBytes)[3], 0x00);  
    delete positiveBytes;


    int32_t negativeValue = -123456;
    std::vector<uint8_t>* negativeBytes = marshalInt32_t(negativeValue);
    EXPECT_EQ(negativeBytes->size(), 4);
    EXPECT_EQ((*negativeBytes)[0], 0xC0);
    EXPECT_EQ((*negativeBytes)[1], 0x1D);
    EXPECT_EQ((*negativeBytes)[2], 0xFE);
    EXPECT_EQ((*negativeBytes)[3], 0xFF);
    delete negativeBytes;

    int32_t zeroValue = 0;
    std::vector<uint8_t>* zeroBytes = marshalInt32_t(zeroValue);
    EXPECT_EQ(zeroBytes->size(), 4);
    EXPECT_EQ((*zeroBytes)[0], 0x00);
    EXPECT_EQ((*zeroBytes)[1], 0x00);
    EXPECT_EQ((*zeroBytes)[2], 0x00);
    EXPECT_EQ((*zeroBytes)[3], 0x00);
    delete zeroBytes;
}

TEST(BinaryConverterTests, MarshalInt64_t) {
    int64_t positiveValue = 123456789012345;
    std::vector<uint8_t>* positiveBytes = marshalInt64_t(positiveValue);
    EXPECT_EQ(positiveBytes->size(), 8);
    delete positiveBytes;

    int64_t negativeValue = -123456789012345;
    std::vector<uint8_t>* negativeBytes = marshalInt64_t(negativeValue);
    EXPECT_EQ(negativeBytes->size(), 8);
    delete negativeBytes;

    int64_t zeroValue = 0;
    std::vector<uint8_t>* zeroBytes = marshalInt64_t(zeroValue);
    EXPECT_EQ(zeroBytes->size(), 8);
    for (int i = 0; i < 8; i++) {
        EXPECT_EQ((*zeroBytes)[i], 0x00);
    }
    delete zeroBytes;
}


TEST(BinaryConverterTests, MarshalString) {

    std::string testString = "Test";
    std::vector<uint8_t>* stringBytes = marshalString(testString);
    EXPECT_EQ(stringBytes->size(), 4);
    EXPECT_EQ((*stringBytes)[0], 'T');
    EXPECT_EQ((*stringBytes)[1], 'e');
    EXPECT_EQ((*stringBytes)[2], 's');
    EXPECT_EQ((*stringBytes)[3], 't');
    delete stringBytes;


    std::string emptyString = "";
    std::vector<uint8_t>* emptyBytes = marshalString(emptyString);
    EXPECT_EQ(emptyBytes->size(), 0);
    delete emptyBytes;

    std::string specialString = "Test\n\t";
    std::vector<uint8_t>* specialBytes = marshalString(specialString);
    EXPECT_EQ(specialBytes->size(), 6);
    EXPECT_EQ((*specialBytes)[4], '\n');
    EXPECT_EQ((*specialBytes)[5], '\t');
    delete specialBytes;
}

TEST(BinaryConverterTests, UnmarshalInt32_t) {
    std::vector<uint8_t>* positiveBytes = new std::vector<uint8_t>{ 0x40, 0xE2, 0x01, 0x00 };
    int32_t positiveValue = 0;
    UnmarshalInt32_t(&positiveValue, positiveBytes);
    EXPECT_EQ(positiveValue, 123456);
    delete positiveBytes;

    std::vector<uint8_t>* negativeBytes = new std::vector<uint8_t>{ 0xC0, 0x1D, 0xFE, 0xFF };
    int32_t negativeValue = 0;
    UnmarshalInt32_t(&negativeValue, negativeBytes);
    EXPECT_EQ(negativeValue, -123456);
    delete negativeBytes;

    std::vector<uint8_t>* zeroBytes = new std::vector<uint8_t>{ 0x00, 0x00, 0x00, 0x00 };
    int32_t zeroValue = 1;  
    UnmarshalInt32_t(&zeroValue, zeroBytes);
    EXPECT_EQ(zeroValue, 0);
    delete zeroBytes;
}

TEST(BinaryConverterTests, UnmarshalInt64_t) {
    int64_t originalValue = 123456789012345;
    std::vector<uint8_t>* bytes = marshalInt64_t(originalValue);

    int64_t recoveredValue = 0;
    UnmarshalInt64_t(&recoveredValue, bytes);
    EXPECT_EQ(recoveredValue, originalValue);
    delete bytes;

    int64_t originalNegValue = -987654321098765;
    std::vector<uint8_t>* negBytes = marshalInt64_t(originalNegValue);
    int64_t recoveredNegValue = 0;
    UnmarshalInt64_t(&recoveredNegValue, negBytes);
    EXPECT_EQ(recoveredNegValue, originalNegValue);
    delete negBytes;
}

TEST(BinaryConverterTests, UnmarshalString) {
    std::vector<uint8_t>* stringBytes = new std::vector<uint8_t>{ 'T', 'e', 's', 't' };
    std::string recoveredString;
    UnmarshalString(&recoveredString, stringBytes);
    EXPECT_EQ(recoveredString, "Test");
    delete stringBytes;

    std::vector<uint8_t>* emptyBytes = new std::vector<uint8_t>();
    std::string recoveredEmptyString;
    UnmarshalString(&recoveredEmptyString, emptyBytes);
    EXPECT_EQ(recoveredEmptyString, "");
    delete emptyBytes;

    std::vector<uint8_t>* specialBytes = new std::vector<uint8_t>{ 'T', 'e', 's', 't', '\n', '\t' };
    std::string recoveredSpecialString;
    UnmarshalString(&recoveredSpecialString, specialBytes);
    EXPECT_EQ(recoveredSpecialString, "Test\n\t");
    delete specialBytes;
}

TEST(BinaryConverterTests, MarshalUnmarshalComplementary) {
    int32_t originalInt32 = 42;
    std::vector<uint8_t>* bytes32 = marshalInt32_t(originalInt32);
    int32_t recoveredInt32 = 0;
    UnmarshalInt32_t(&recoveredInt32, bytes32);
    EXPECT_EQ(originalInt32, recoveredInt32);
    delete bytes32;

    int64_t originalInt64 = 9223372036854775807;  
    std::vector<uint8_t>* bytes64 = marshalInt64_t(originalInt64);
    int64_t recoveredInt64 = 0;
    UnmarshalInt64_t(&recoveredInt64, bytes64);
    EXPECT_EQ(originalInt64, recoveredInt64);
    delete bytes64;

    std::string originalString = "Hello, World!";
    std::vector<uint8_t>* bytesString = marshalString(originalString);
    std::string recoveredString;
    UnmarshalString(&recoveredString, bytesString);
    EXPECT_EQ(originalString, recoveredString);
    delete bytesString;
}



