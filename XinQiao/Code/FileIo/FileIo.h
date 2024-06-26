#pragma once
#ifndef FILE_IO_H
#define FILE_IO_H
#include <fstream>
#include <filesystem>
#include"../PdSdk/Pd.h"
#define MAIN_PATHR  "D:/XinQiao/"
ErrRet ReadFile(IN std::string fileName, OUT std::string& data, IN std::ios::openmode mode);
ErrRet WriteFile(IN std::string fileName, IN std::string data, IN std::ios::openmode mode);
#endif // !FILE_IO_H

