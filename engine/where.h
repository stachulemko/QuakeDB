#ifndef WHERE_H  
#define WHERE_H
#include <iostream>
#include <string>
#include <vector>
#include "typeManager.h"
#include "table.h"

std::vector<std::vector<allVars>> whereFunc(std::vector<std::vector<allVars>> AllTableData, std::vector<std::vector<allVars>> lastState, std::string columnName, std::string mathOperator, allVars ifValue);

#endif