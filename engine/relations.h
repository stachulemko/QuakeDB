
#ifndef RELATIONS_H  
#define RELATIONS_H
#include <iostream>
#include <string>
#include <vector>
#include "typeManager.h"
#include "fileOperationManager.h"
#include "relation.h"

namespace {
	std::vector<Relation> relations;
	int numberOfRelations = 0;
	std::string folderPath = "relationsData";
}

void addRelation(std::string tableName, std::string columnName, std::string foreignTableName, std::string foreignColumnName, std::string onDelete);

void marshallRelations();	

void loadRelations();

void test35();

#endif
