#ifndef WALL_H
#define WALL_H
#include <iostream>
#include <vector>
#include <string>
#include "typeManager.h"
#include "fileOperationManager.h"
#include "binaryConverter.h"
#include "column.h"
#include "record.h"
class Wal {
private:
	std::vector<std::vector<allVars>> walData;
	const std::string path= executionFilePath() + "/wal" +"/wal" + ".bin";
public:
	Wal() {};

	void createDirectoryAndFile();

	std::vector<std::vector<allVars>> getWalData() const;

	void clearWalData();

	int getWalDataSize();

    void addToWall(std::vector<uint8_t>dataBytes, int32_t operationId, std::vector<allVars>data, int32_t dataBytesSize);

    void loadWal() ;

	void showWalData();


};


#endif