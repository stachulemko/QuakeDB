//1.0.0
#ifndef BINARYCONVERTER_H   
#define BINARYCONVERTER_H
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <bitset>


const int32_t int32_tId = 1;
const int32_t int64_tId = 2;
const int32_t stringId = 3;
const int32_t columnTypeId = 4;


std::vector<uint8_t>* marshalInt32_t(int32_t val);
std::vector<uint8_t>* marshalInt64_t(int64_t val);
std::vector<uint8_t>* marshalString(std::string val);

void UnmarshalInt32_t(int32_t* val, std::vector<uint8_t>* int32InBinary);
void UnmarshalInt64_t(int64_t* val, std::vector<uint8_t>* int64InBinary);
void UnmarshalString(std::string* val, std::vector<uint8_t>* stringInBinary);

void showBytes(const std::vector<uint8_t>& bytes);
#endif
