
#ifndef FILEOPERATIONMANAGER_H
#define FILEOPERATIONMANAGER_H
#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <fstream>
#if __cplusplus < 201703L || (!defined(_MSC_VER) && !defined(__cpp_lib_filesystem))
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif



void createFolder(std::string path);

void createBinFile(std::string path, std::string name);

void addToFileBytes(std::string path, const std::vector<uint8_t>& bytes);

void deleteFile(std::string path);

std::vector<uint8_t> readFileBytes(std::string path);

void showFileBytes(std::string path);

bool isFileExists(const std::string& path);

std::string executionFilePath();

void clearFile(std::string path);

/*
double getSizeFile(const std::string& path) {
    try {
        if (!fs::exists(path)) {
            std::cerr << "Error: File '" << path << "' does not exist!" << std::endl;
            return 0.0;
        }

        uintmax_t sizeInBytes = fs::file_size(path);

        double sizeInMB = static_cast<double>(sizeInBytes) / (1024.0 * 1024.0);

        return sizeInMB;
    }
    catch (const std::exception& e) {
        std::cerr << "Error while getting file size: " << e.what() << std::endl;
        return 0.0;
    }
}
*/

#endif 
