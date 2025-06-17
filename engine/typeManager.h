#ifndef TYPEMANAGER_H   
#define TYPEMANAGER_H
#include <iostream>
#include <variant>
#include <vector>

const int32_t int32_tId = 1;
const int32_t int64_tId = 2;
const int32_t stringId = 3;
const int32_t columnTypeId = 4;
const int32_t recordTypeId = 5;
const int32_t insertTypeId = 6;
const int32_t addColumnTypeId = 7;

using allVars = std::variant<int32_t, int64_t, std::string>;

int32_t getTypeId(const allVars& value);

void showVariantVariable(allVars vec);

void showVariantVectorOfVector(std::vector<std::vector<allVars>> vec);

#endif