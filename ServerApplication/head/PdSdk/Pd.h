#define _CRT_SECURE_NO_WARNINGS
#ifndef PDSDK
#define PDSDK
#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <iostream>
#include "SqList.h"
#include <signal.h>
#include <cassert>
typedef unsigned int  uint;
typedef char* Str;
#define PV              //函数涉及pv操作，可能阻塞
#define KB              1024
#define MB              1024*KB
#define GB              1024*MB
#define IN
#define OUT
#define MALLOC          //函数涉及内存空间分配
#define FREE            //函数设计内存空间释放
#define STR_END_LEN     1
#define BIT_SIZE        8
#define FALSE           0
#define TRUE            1
#define INDEX  //二进制流，位数开始下标
#define SIZE   //字节大小
#define BYTE_LEN   8  //一字节的二进制数大小
#define ASEERRT(A)  Assert(A,__FILE__,__LINE__)
#define  PTR_CHECK(A)       \
    if(A == nullptr)        \
        ASEERRT(false)
#define LOG  Log()

typedef enum {
    SUC,
    FileNotExist,
    OpenFileErr,
    OpenFileSuc,
    WriteFileErr
}ErrNum;
class Log {
public:
    Log();
    template<typename AnyType>
    Log& operator<<(const AnyType& n) {
        std::cout << n;
        return *this;
    }
    Log& operator<<(ostream& (*pf)(ostream&));
};

class ErrRet{
public:
    ErrRet();
    ErrRet(ErrNum errNum);
    bool operator==(uint64_t value);
private:
    ErrNum m_ErrNum;
};

/**
 * @brief 截取二进制数据流为整数类型
 * @param param1 二进制数据流
 * @param param2 截取开始的角标（包含被截取部分）
 * @param param3 截取结束的角标（不包含进截取部分）
 * @return 截取后的整数类型
 */
uint64_t BintoInt(IN byte * data, IN uint16_t begin, IN uint16_t end);

void StrnCpy(Str dest, Str source, int n);

size_t ZPstrlen(const char* str);

void InsertString(Str & destStr, const Str insertStr, int pos);

void Assert(bool condition, const char* file, int line);

#endif // PDSDK

