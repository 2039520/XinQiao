#define _CRT_SECURE_NO_WARNINGS
#include"Pd.h"

void Assert(bool condition, const char* file, int line) {
    if (!condition) {
        std::cerr << "Assertion failed: " << " at " << file << ":" << line << std::endl;
        assert(condition);
    }
}

/**
 * @brief 截取二进制数据流为整数类型
 * @param param1 二进制数据流
 * @param param2 截取开始的角标（包含被截取部分）
 * @param param3 截取结束的角标（不包含进截取部分）
 * @return 截取后的整数类型
 */
uint64_t BintoInt(IN byte* data, IN uint16_t begin, IN uint16_t end) {
    uint startByte = begin / BIT_SIZE;
    uint startOffset = begin % BIT_SIZE;
    uint endByte = (end - 1) / BIT_SIZE;
    uint endOffset = (end - 1) % BIT_SIZE;
    uint64_t number = 0;
    for (uint i = startByte, j = 0; i <= endByte; i++, j++) {
        uint64_t uint64Data = (uint64_t)data[i];
        uint64Data &= 0xff;
        uint64_t byteData = uint64Data << (j * BIT_SIZE); //[!]仅适用于小端模式
        number = number | byteData;
    }
    number = (number << startOffset) >> (startOffset + ((BIT_SIZE - 1) - endOffset));
    return number;
}

void StrnCpy(Str dest, Str source, int n) {
    int i;
    for (i = 0; i < n; i++) {
        dest[i] = source[i];
    }
    dest[i] = '\0';
}

size_t ZPstrlen(const char* str) {
    return strlen(str) + STR_END_LEN;
}

void InsertString(Str& destStr, const Str insertStr, int pos) {//后插
    long long length = strlen(destStr) + strlen(insertStr);

    destStr = (Str)realloc(destStr, length + 1);
    *(destStr + length + 1) = '\0';
    Str newStr = (Str)malloc(strlen(destStr) + strlen(insertStr) + 1);
    memset(newStr, 0, strlen(destStr) + strlen(insertStr) + 1);

    StrnCpy(newStr, destStr, pos + 1);
    strcat(newStr, insertStr);
    strcat(newStr, destStr + pos + 1);
    strcpy(destStr, newStr);

    free(newStr);
}

Log::Log() {
    time_t now = time(nullptr);
    tm* localTm;
    localTm = localtime(&now);
    std::cout << "[--" << localTm->tm_mon << "/" << localTm->tm_mday << "/" << localTm->tm_hour << "/" << localTm->tm_min << "--]  ";
}

Log& Log::operator<<(std::ostream& (*fp)(std::ostream&)) {
    // 调用插入运算符
    std::cout << fp;
    return *this;
}

bool ErrRet::operator==(uint64_t value) {

    if (value == this->m_ErrNum)
        return true;
    return false;
}

ErrRet::ErrRet() :m_ErrNum(SUC) {};

ErrRet::ErrRet(ErrNum errNum) {
    this->m_ErrNum = errNum;
}


