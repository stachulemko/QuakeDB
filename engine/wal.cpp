#include "wal.h"


void Wal::addToWall(std::vector<uint8_t>dataBytes, int32_t operationId, std::vector<allVars>data, int32_t dataBytesSize) {
    std::vector<allVars>tmp = data;
    std::vector<uint8_t>tmpDataBytes;
	std::cout << "dataBytes.size()" << dataBytes.size() <<"dataBytesSecond.size()"<<dataBytesSize << std::endl;
    std::vector<uint8_t>* tmpDataBytesSize = marshalInt32_t(dataBytes.size());
    std::vector<uint8_t>* tmpDataBytesOperationId = marshalInt32_t(operationId);
    tmp.insert(tmp.begin(), operationId);
    tmp.insert(tmp.begin(), dataBytesSize);
    tmpDataBytes.insert(tmpDataBytes.begin(), tmpDataBytesSize->begin(), tmpDataBytesSize->end());
    tmpDataBytes.insert(tmpDataBytes.begin(), tmpDataBytesOperationId->begin(), tmpDataBytesOperationId->end());
	tmpDataBytes.insert(tmpDataBytes.end(), dataBytes.begin(), dataBytes.end());
    walData.push_back(tmp);
    addToFileBytes(path, tmpDataBytes);
}
void Wal::loadWal() {
    std::vector<uint8_t> dataWal = readFileBytes(path);
    int32_t offset = 0;
    int32_t operationId = 0;
    int32_t dataBytesSize = 0;
    while (offset < dataWal.size()) {
        if (offset + 4 <= dataWal.size()) {
            std::vector<uint8_t> operationIdBytes(dataWal.begin() + offset, dataWal.begin() + offset + 4);
            UnmarshalInt32_t(&operationId, &operationIdBytes);
            offset += 4;
            if (offset + 4 <= dataWal.size()) {
                std::vector<uint8_t> dataBytesSizeBytes(dataWal.begin() + offset, dataWal.begin() + offset + 4);
                UnmarshalInt32_t(&dataBytesSize, &dataBytesSizeBytes);
                offset += 4;
                if (offset + dataBytesSize <= dataWal.size()) {
                    std::vector<uint8_t> dataBytes(dataWal.begin() + offset, dataWal.begin() + offset + dataBytesSize);
                    offset += dataBytesSize;
                    if (operationId == addColumnTypeId) {
                        Column column;
                        column.loadAllBytesToDecode(dataBytes);
                        column.decodeColumn();
                        walData.push_back({ operationId, dataBytesSize, column.getColumnName(), column.getColumnType(), column.isAllowNull() });
                    }
                    else if (operationId == insertTypeId) {
                        Record record;
                        record.loadAllConnectedBytes(dataBytes);
                        record.decode();
                        std::vector<allVars> recordData;
                        recordData.push_back(operationId);
                        recordData.push_back(dataBytesSize);
                        std::vector<allVars> recordDataTlv = record.getRecordData();
                        for (int i = 0; i < recordDataTlv.size(); i++) {
                            recordData.push_back(recordDataTlv[i]);
                        }
                        walData.push_back(recordData);
                    }
                }
                else {
                    std::cerr << "Error: Insufficient data for operation ID and size decoding" << std::endl;
                    return;
                }
            }
            else {
                std::cerr << "Error: Insufficient data for operation ID decoding" << std::endl;
                return;
            }
        }
        else {
            std::cerr << "Error: Insufficient data for operation ID decoding" << std::endl;
            return;
        }
    }
}
void Wal::showWalData() {
    for (const auto& row : walData) {
        for (const auto& value : row) {
            showVariantVariable(value);
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}
void Wal::createDirectoryAndFile() {
    createFolder(executionFilePath() + "/wal");
    createBinFile(executionFilePath() + "/wal", "wal");
}
std::vector<std::vector<allVars>> Wal::getWalData() const {
    return walData;
}
void Wal::clearWalData() {
    walData.clear();
}
int Wal::getWalDataSize() {
    return walData.size();
}

