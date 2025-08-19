#include "relations.h"



void addRelation(std::string tableName, std::string columnName, std::string foreignTableName, std::string foreignColumnName, std::string onDelete) {
    //createFolder(folderPath);
    //std::string path = "relations" + std::to_string(numberOfRelations);
    //createBinFile(folderPath, path);
    Relation relation(tableName, columnName, foreignTableName, foreignColumnName, onDelete);

    relations.push_back(relation);



}

void marshallRelations() {
    //std::vector<uint8_t> result;
    int32_t tmpSize = 0;
    std::string path = "relations" + std::to_string(numberOfRelations);
    for (auto relation : relations) {
        tmpSize += relation.getSize();
        std::vector<uint8_t> relationBytes = relation.marshall();
        createFolder(folderPath);
        createBinFile(folderPath, path);
        if (getSizeFile(folderPath + "/" + path) > 3900 + (relation.getSize() / (1024.0 * 1024.0))) {
            numberOfRelations += 1;
            tmpSize = 0;
            //path = "relationsData/relations.bin";
        }
        else {
            addToFileBytes(folderPath + "/" + path + ".bin", relationBytes);
        }
        //result.insert(result.end(), relationBytes.begin(), relationBytes.end());
    }
    //std::string path = "relations" + std::to_string(numberOfRelations) + ".bin";

}

void loadRelations() {
    const std::string folder = folderPath;
    try {
        for (const auto& entry : fs::directory_iterator(folder)) {
            if (!fs::is_directory(entry.status())) {
                std::string fileName = entry.path().filename().string();
                std::vector<uint8_t> fileBytes = readFileBytes(entry.path().string());
                int offset = 0;
                while (offset + 4 <= fileBytes.size()) {
                    std::vector<uint8_t> typeBytes(fileBytes.begin() + offset, fileBytes.begin() + offset + 4);
                    int32_t type = 0;
                    UnmarshalInt32_t(&type, &typeBytes);
                    if (type == relationId) {
                        std::vector<uint8_t> sizeBytes(fileBytes.begin() + offset + 4, fileBytes.begin() + offset + 8);
                        int32_t size = 0;
                        UnmarshalInt32_t(&size, &sizeBytes);
                        offset += 8;

                        if (offset + size <= fileBytes.size()) {
                            std::vector<uint8_t> relationBytes(fileBytes.begin() + offset-8, fileBytes.begin() + offset + size);
                            Relation relation("", "", "", "", "");
                            relation.decode(relationBytes);
                            relations.push_back(relation);
                            offset += size;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error occurred during relation decoding: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown error occurred during relation decoding" << std::endl;
    }
}

void test35() {
    std::vector<Relation>test = relations;
}