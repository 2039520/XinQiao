#include"FileIo.h"
namespace fs = std::filesystem;
ErrRet ReadFile(IN std::string fileName, OUT std::string& data, IN std::ios::openmode mode) {
    if (!fs::exists(fileName)) {
        return FileNotExist;
    }
    std::ifstream file(fileName, mode);
    if (!file.is_open()) {
        LOG << "Failed to open file: " << fileName << std::endl;
        return OpenFileErr;
    }
    data.assign((std::istreambuf_iterator<char>(file)), {});
    file.close();
    return SUC;
}

ErrRet WriteFile(IN std::string fileName, IN std::string data, IN std::ios::openmode mode) {
    std::ofstream file(fileName, mode);
    /*if (!file.is_open()) {
        LOG << "Failed to open file: " << fileName << std::endl;
        return OpenFileErr;
    }*/
    file.write(data.c_str(), data.size());
    if (!file) {
        LOG << "Failed to write file: " << fileName << std::endl;
        return WriteFileErr;
    }
    file.close();
    return SUC;
}