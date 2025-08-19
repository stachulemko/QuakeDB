#ifndef JOIN_H 
#define JOIN_H
#include <iostream>
#include <vector>
#include "typeManager.h"
#include "relations.h"

std::vector<std::vector<allVars>> joinFunction(
    std::vector<std::vector<allVars>> lastStateFirstTableData,
    std::vector<std::vector<allVars>> allFirstTableData,
    std::vector<std::vector<allVars>> allSecondTableData,
    std::string secondTableColumnName,
    std::string firstTableColumnName);

#endif