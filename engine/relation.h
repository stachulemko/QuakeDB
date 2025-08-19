#ifndef RELATION_H  
#define RELATION_H

#include <iostream>
#include <string>
#include <vector>
#include "simpleOperations.h"
#include "binaryConverter.h"
#include "typeManager.h"
#include "tlv.h"
#include "dataBase.h"

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
        size = sourceTable.size() + sourceColumn.size() + targetTable.size() + targetColumn.size() + onDelete.size() + 48;
    }
    std::vector<uint8_t> marshall() const {
		std::vector<uint8_t> result;
		std::vector<uint8_t>* idRelation = marshalInt32_t(relationId);
		result.insert(result.end(), idRelation->begin(), idRelation->end());
        std::vector<uint8_t>* sizeBytes = marshalInt32_t(size-8);
		result.insert(result.end(), sizeBytes->begin(), sizeBytes->end());
		
        Tlv sourceTableTlv(sourceTable);
        std::vector<uint8_t>vec = sourceTableTlv.marshalTlv();
        result.insert(result.end(), vec.begin(), vec.end());

        Tlv sourceColumnTlv(sourceColumn);
        std::vector<uint8_t>vec1 = sourceColumnTlv.marshalTlv();
        result.insert(result.end(), vec1.begin(), vec1.end());

        Tlv targetTableTlv(targetTable);
        std::vector<uint8_t>vec2 = targetTableTlv.marshalTlv();
        result.insert(result.end(), vec2.begin(), vec2.end());

        Tlv targetColumnTlv(targetColumn);
        std::vector<uint8_t>vec3 = targetColumnTlv.marshalTlv();
        result.insert(result.end(), vec3.begin(), vec3.end());

        Tlv onDeleteTlv(onDelete);
        std::vector<uint8_t>vec4 = onDeleteTlv.marshalTlv();
        result.insert(result.end(), vec4.begin(), vec4.end());
        return result;
    }
    void decode(const std::vector<uint8_t>& result) {
        int offset = 0;

        // Odczytaj ID relacji (pierwsze 4 bajty)
        if (offset + 4 <= result.size()) {
            std::vector<uint8_t> typeBytes(result.begin() + offset, result.begin() + offset + 4);
            int32_t type = 0;
            UnmarshalInt32_t(&type, &typeBytes);
            offset += 4;

            // Sprawdü czy to relationId
            if (type != relationId) {
                std::cerr << "Error: Invalid relation ID" << std::endl;
                return;
            }
        }
        else {
            std::cerr << "Error: Insufficient data for relation ID" << std::endl;
            return;
        }

        // Odczytaj rozmiar ca≥ej relacji (kolejne 4 bajty)
        if (offset + 4 <= result.size()) {
            std::vector<uint8_t> sizeBytes(result.begin() + offset, result.begin() + offset + 4);
            int32_t totalSize = 0;
            UnmarshalInt32_t(&totalSize, &sizeBytes);
            offset += 4;

            // Sprawdü czy mamy wystarczajπco danych
            if (offset + totalSize > result.size()) {
                std::cerr << "Error: Insufficient data for relation content" << std::endl;
                return;
            }
        }
        else {
            std::cerr << "Error: Insufficient data for relation size" << std::endl;
            return;
        }

        // Dekoduj sourceTable
        if (offset + 8 <= result.size()) {
            std::vector<uint8_t> headerBytes(result.begin() + offset, result.begin() + offset + 8);
            int32_t tlvType = 0;
            int32_t tlvLength = 0;


            std::vector<uint8_t>tlVTypeBytes(headerBytes.begin(), headerBytes.begin() + 4);
            std::vector<uint8_t>tlvLenghtBytes(headerBytes.begin() + 4, headerBytes.end());


            UnmarshalInt32_t(&tlvType, &tlVTypeBytes);
            UnmarshalInt32_t(&tlvLength, &tlvLenghtBytes);

            if (tlvType == stringId && offset + 8 + tlvLength <= result.size()) {
                std::vector<uint8_t> sourceTableBytes(result.begin() + offset, result.begin() + offset + 8 + tlvLength);
                Tlv tlvSourceTable(sourceTableBytes);
                tlvSourceTable.decode();
                const std::string* value = tlvSourceTable.getStringValue();
                if (value) {
                    sourceTable = *value;
                }
                offset += 8 + tlvLength;
            }
            else {
                std::cerr << "Error: Invalid TLV type for sourceTable or insufficient data" << std::endl;
                return;
            }
        }
        else {
            std::cerr << "Error: Insufficient data for sourceTable TLV header" << std::endl;
            return;
        }

        // Dekoduj sourceColumn
        if (offset + 8 <= result.size()) {
            std::vector<uint8_t> headerBytes(result.begin() + offset, result.begin() + offset + 8);
            int32_t tlvType = 0;
            int32_t tlvLength = 0;

            std::vector<uint8_t>tlVTypeBytes(headerBytes.begin(), headerBytes.begin() + 4);
            std::vector<uint8_t>tlvLenghtBytes(headerBytes.begin() + 4, headerBytes.end());


            UnmarshalInt32_t(&tlvType, &tlVTypeBytes);
            UnmarshalInt32_t(&tlvLength, &tlvLenghtBytes);

            if (tlvType == stringId && offset + 8 + tlvLength <= result.size()) {
                std::vector<uint8_t> sourceColumnBytes(result.begin() + offset, result.begin() + offset + 8 + tlvLength);
                Tlv tlvSourceColumn(sourceColumnBytes);
                tlvSourceColumn.decode();
                const std::string* value = tlvSourceColumn.getStringValue();
                if (value) {
                    sourceColumn = *value;
                }
                offset += 8 + tlvLength;
            }
            else {
                std::cerr << "Error: Invalid TLV type for sourceColumn or insufficient data" << std::endl;
                return;
            }
        }
        else {
            std::cerr << "Error: Insufficient data for sourceColumn TLV header" << std::endl;
            return;
        }

        // Dekoduj targetTable
        if (offset + 8 <= result.size()) {
            std::vector<uint8_t> headerBytes(result.begin() + offset, result.begin() + offset + 8);
            int32_t tlvType = 0;
            int32_t tlvLength = 0;

            std::vector<uint8_t>tlVTypeBytes(headerBytes.begin(), headerBytes.begin() + 4);
            std::vector<uint8_t>tlvLenghtBytes(headerBytes.begin() + 4, headerBytes.end());


            UnmarshalInt32_t(&tlvType, &tlVTypeBytes);
            UnmarshalInt32_t(&tlvLength, &tlvLenghtBytes);

            if (tlvType == stringId && offset + 8 + tlvLength <= result.size()) {
                std::vector<uint8_t> targetTableBytes(result.begin() + offset, result.begin() + offset + 8 + tlvLength);
                Tlv tlvTargetTable(targetTableBytes);
                tlvTargetTable.decode();
                const std::string* value = tlvTargetTable.getStringValue();
                if (value) {
                    targetTable = *value;
                }
                offset += 8 + tlvLength;
            }
            else {
                std::cerr << "Error: Invalid TLV type for targetTable or insufficient data" << std::endl;
                return;
            }
        }
        else {
            std::cerr << "Error: Insufficient data for targetTable TLV header" << std::endl;
            return;
        }

        // Dekoduj targetColumn
        if (offset + 8 <= result.size()) {
            std::vector<uint8_t> headerBytes(result.begin() + offset, result.begin() + offset + 8);
            int32_t tlvType = 0;
            int32_t tlvLength = 0;

            std::vector<uint8_t>tlVTypeBytes(headerBytes.begin(), headerBytes.begin() + 4);
            std::vector<uint8_t>tlvLenghtBytes(headerBytes.begin() + 4, headerBytes.end());


            UnmarshalInt32_t(&tlvType, &tlVTypeBytes);
            UnmarshalInt32_t(&tlvLength, &tlvLenghtBytes);

            if (tlvType == stringId && offset + 8 + tlvLength <= result.size()) {
                std::vector<uint8_t> targetColumnBytes(result.begin() + offset, result.begin() + offset + 8 + tlvLength);
                Tlv tlvTargetColumn(targetColumnBytes);
                tlvTargetColumn.decode();
                const std::string* value = tlvTargetColumn.getStringValue();
                if (value) {
                    targetColumn = *value;
                }
                offset += 8 + tlvLength;
            }
            else {
                std::cerr << "Error: Invalid TLV type for targetColumn or insufficient data" << std::endl;
                return;
            }
        }
        else {
            std::cerr << "Error: Insufficient data for targetColumn TLV header" << std::endl;
            return;
        }

        // Dekoduj onDelete
        if (offset + 8 <= result.size()) {
            std::vector<uint8_t> headerBytes(result.begin() + offset, result.begin() + offset + 8);
            int32_t tlvType = 0;
            int32_t tlvLength = 0;

            std::vector<uint8_t>tlVTypeBytes(headerBytes.begin(), headerBytes.begin() + 4);
            std::vector<uint8_t>tlvLenghtBytes(headerBytes.begin() + 4, headerBytes.end());


            UnmarshalInt32_t(&tlvType, &tlVTypeBytes);
            UnmarshalInt32_t(&tlvLength, &tlvLenghtBytes);

            if (tlvType == stringId && offset + 8 + tlvLength <= result.size()) {
                std::vector<uint8_t> onDeleteBytes(result.begin() + offset, result.begin() + offset + 8 + tlvLength);
                Tlv tlvOnDelete(onDeleteBytes);
                tlvOnDelete.decode();
                const std::string* value = tlvOnDelete.getStringValue();
                if (value) {
                    onDelete = *value;
                }
                offset += 8 + tlvLength;
            }
            else {
                std::cerr << "Error: Invalid TLV type for onDelete or insufficient data" << std::endl;
                return;
            }
        }
        else {
            std::cerr << "Error: Insufficient data for onDelete TLV header" << std::endl;
            return;
        }

        // Aktualizuj rozmiar w obiekcie
        size = sourceTable.size() + sourceColumn.size() + targetTable.size() + targetColumn.size() + onDelete.size() + 48;
    }





};

#endif