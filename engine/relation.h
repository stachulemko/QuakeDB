#ifndef RELATION_H  
#define RELATION_H

#include <iostream>
#include <string>
#include <vector>
#include "simpleOperations.h"
#include "binaryConverter.h"
#include "typeManager.h"

class Relation {
private:
    std::string sourceTable;
    std::string sourceColumn;
    std::string targetTable;
    std::string targetColumn;
    std::string onDelete;

    int32_t size = 0;
public:
	std::string getSourceTable() const {
		return sourceTable;
	}
    std::string getSourceColumn() const {
        return sourceColumn;
    }
	std::string getTargetTable() const {
		return targetTable;
	}
	std::string getTargetColumn() const {
		return targetColumn;
	}
	std::string getOnDelete() const {
		return onDelete;
	}

    int32_t getSize() {
		return size;
    }

    Relation(std::string sourceTable, std::string sourceColumn, std::string targetTable, std::string targetColumn, std::string onDelete) {
        this->sourceTable = sourceTable;
        this->sourceColumn = sourceColumn;
        this->targetTable = targetTable;
        this->targetColumn = targetColumn;
        this->onDelete = onDelete;
        size = sourceTable.size() + sourceColumn.size() + targetTable.size() + targetColumn.size() + onDelete.size() + 40;
    }
    std::vector<uint8_t> marshall() const {
		std::vector<uint8_t> result;
		std::vector<uint8_t>* idRelation = marshalInt32_t(relationId);
		result.insert(result.end(), idRelation->begin(), idRelation->end());
        std::vector<uint8_t>* sizeBytes = marshalInt32_t(size);
		result.insert(result.end(), sizeBytes->begin(), sizeBytes->end());
		std::vector<uint8_t>* sourceTableBytes = marshalString(sourceTable);
		result.insert(result.end(), sourceTableBytes->begin(), sourceTableBytes->end());
		std::vector<uint8_t>* sourceColumnBytes = marshalString(sourceColumn);
		result.insert(result.end(), sourceColumnBytes->begin(), sourceColumnBytes->end());
		std::vector<uint8_t>* targetTableBytes = marshalString(targetTable);
		result.insert(result.end(), targetTableBytes->begin(), targetTableBytes->end());
		std::vector<uint8_t>* targetColumnBytes = marshalString(targetColumn);
		result.insert(result.end(), targetColumnBytes->begin(), targetColumnBytes->end());
		std::vector<uint8_t>* onDeleteBytes = marshalString(onDelete);
		result.insert(result.end(), onDeleteBytes->begin(), onDeleteBytes->end());
		return result;

    }
    void decode(const std::vector<uint8_t>& result) {
        int offset = 0;

        std::string decodedRelationId;
        std::vector<uint8_t> relationIdBytes(result.begin() + offset, result.begin() + offset + 4);
        UnmarshalString(&decodedRelationId, &relationIdBytes);
        offset += 4;

        std::string sizeL = 0;
        std::vector<uint8_t> sizeBytes(result.begin() + offset, result.begin() + offset + 4);
        UnmarshalString(&sizeL, &sizeBytes);
        offset += 4;

        std::string sourceTableVal = 0;
        std::vector<uint8_t> sourceTableBytes(result.begin() + offset, result.begin() + offset + 4);
        UnmarshalString(&sourceTableVal, &sourceTableBytes);
        offset += 4;

        std::string sourceColumnVal = 0;
        std::vector<uint8_t> sourceColumnBytes(result.begin() + offset, result.begin() + offset + 4);
        UnmarshalString(&sourceColumnVal, &sourceColumnBytes);
        offset += 4;

        std::string targetTableVal = 0;
        std::vector<uint8_t> targetTableBytes(result.begin() + offset, result.begin() + offset + 4);
        UnmarshalString(&targetTableVal, &targetTableBytes);
        offset += 4;

        std::string targetColumnVal = 0;
        std::vector<uint8_t> targetColumnBytes(result.begin() + offset, result.begin() + offset + 4);
        UnmarshalString(&targetColumnVal, &targetColumnBytes);
        offset += 4;

        std::string onDeleteVal = 0;
        std::vector<uint8_t> onDeleteBytes(result.begin() + offset, result.begin() + offset + 4);
        UnmarshalString(&onDeleteVal, &onDeleteBytes);
        offset += 4;

        this->sourceTable = sourceTableVal;
		this->sourceColumn = sourceColumnVal;
		this->targetTable = targetTableVal;
		this->targetColumn = targetColumnVal;
		this->onDelete = onDeleteVal;
    }



};

#endif