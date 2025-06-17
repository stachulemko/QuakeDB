#include "fileOperationManager.h"

void createFolder(std::string path) {
    try {
        if (!fs::exists(path)) {
            if (fs::create_directory(path)) {
            }
            else {
                std::cerr << "Error: Failed to create folder '" << path << "'" << std::endl;
            }
        }
        else {
            std::cerr << "WARNING: Folder '" << path << "' already exists!" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error while creating folder: " << e.what() << std::endl;
    }
}

void createBinFile(std::string path, std::string name) {
    std::string filePath = path + "/" + name + ".bin";
    if (fs::exists(path)) {
        std::ifstream fileCheck(filePath, std::ios::binary);
        if (fileCheck) {
            std::cerr << "Error: file " << filePath << " already exists" << std::endl;
            fileCheck.close();
            return;
        }

        std::ofstream newFile(filePath, std::ios::binary);
        if (!newFile) {
            std::cerr << "Error while making file " << filePath << "!" << std::endl;
            return;
        }
        newFile.close();
    }
    else {
		std::cerr << "Error path:  '" << filePath << "' does not exist!" << std::endl;
    }
   
}
void addToFileBytes(std::string path, const std::vector<uint8_t>& bytes) {
    std::ofstream file(path, std::ios::binary | std::ios::app);
    if (!file) {
        std::cerr << "Error: Cannot open file " << path << " for appending!" << std::endl;
        return;
    }
    if (!bytes.empty()) {
        file.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    }
    file.close();
}
void deleteFile(std::string path) {
    try {
        if (fs::exists(path)) {
            fs::remove(path);
			std::cerr << "File '" << path << "' deleted successfully." << std::endl;
        }
        else {
            std::cerr << "Error: File '" << path << "' does not exist!" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error while deleting file: " << e.what() << std::endl;
    }
}
std::vector<uint8_t> readFileBytes(std::string path) {
    std::vector<uint8_t> buffer;

    if (!fs::exists(path)) {
        std::cerr << "Error: File '" << path << "' does not exist!" << std::endl;
        return buffer; 
    }

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Error: Cannot open file '" << path << "' for reading!" << std::endl;
        return buffer; 
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (fileSize == 0) {
        std::cerr << "Warning: File '" << path << "' is empty." << std::endl;
        return buffer;
    }

    buffer.resize(static_cast<size_t>(fileSize));

    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        std::cerr << "Error: Failed to read data from file '" << path << "'!" << std::endl;
        buffer.clear(); 
    }

    file.close();
    return buffer;
}
void showFileBytes(std::string path) {
    std::vector<uint8_t> tmp = readFileBytes(path);
    for (int i = 0; i < tmp.size(); i++) {
        std::cout << std::bitset<8>(tmp[i]) << " ";

    }
}
bool isFileExists(const std::string& path) {
	return fs::exists(path);
}
std::string executionFilePath() {
	std::string path = fs::current_path().string();
    for (char& c : path) {
        if (c == '\\') c = '/';
    }
	return path;
}

void clearFile(std::string path) {
    try {
        if (fs::exists(path)) {
            std::ofstream file(path, std::ios::binary | std::ios::trunc);
            if (!file) {
                std::cerr << "Error: Cannot open file '" << path << "' for clearing!" << std::endl;
                return;
            }
            file.close();
            std::cerr << "File '" << path << "' cleared successfully." << std::endl;
        }
        else {
            std::cerr << "Error: File '" << path << "' does not exist!" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error while clearing file: " << e.what() << std::endl;
    }
}