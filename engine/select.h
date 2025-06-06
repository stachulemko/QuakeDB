#ifndef SELECT_H  
#define SELECT_H
#include "table.h"
#include "column.h"
#include "simpleOperations.h"
#include "tlv.h"
#include "typeManager.h"

std::vector<std::vector<allVars>> selectFunc(std::vector<Table*> tables, std::string tableName, std::vector<std::string> columnNames);
void showSelect(std::vector<Table*> tables, std::string tableName, std::vector<std::string> columnNames);

#endif