#pragma once
#define MAX_SqList_LENGTH  100
#define SqList_INCREMENT 10
#include"SearchAlgorithm.h"
#include"Pd.h"

template<typename dataType>
bool SqListCmp(dataType A, dataType B, char symbol = '=');

template <typename dataType>
class SqList {
public:
    dataType* data;
    int length;
    int size;
public:
    SqList();
    SqList(const SqList& p);
    ~SqList();
public:
    SqList& operator=(const SqList& other) {
        if (this != &other) {
            delete[] data;
            data = new dataType[other.size];
            size = other.size;
            for (int i = other.length - 1; i >= 0; i--) {
                this->data[i] = other.data[i];
            }
            length = other.length;
        }
        return *this;
    }
    int FindNode(dataType nodeData, bool(*cmp)(dataType A, dataType B, char symbol) = SqListCmp);
    bool InitSqList(dataType* d = NULL, int n = 0);
    bool DestroySqList();
    void ClearSqList();
    bool SqListEmpty();
    int SqListLength();
    bool GetElem(int i, dataType e);
    int LocatElem(dataType e, char c);
    bool PriorElme(dataType curE, dataType priorE);
    bool NextElme(dataType curE, dataType nextE);
    bool SqListInsert(int i, const dataType& e);
    bool SqListDelete(int i, dataType* e = NULL);
    bool SqListIncrseInsert(dataType& e, bool (*cmp)(dataType A, dataType B, char symbol) = SqListCmp);
};
template<typename dataType>
bool SqListUnion(SqList<dataType>& L1, SqList<dataType>& L2);

template<typename dataType>
void MergeSqList(SqList<dataType>& L1, SqList<dataType>& L2, SqList<dataType>& L3);





//定义
//————————————————————————————————
template<typename dataType>
bool SqList<dataType>::InitSqList(dataType* d, int n) {
    if (n == 0) {
        this->data = new dataType[SqList_INCREMENT];
        if (!this->data)
            return false;
        this->length = 0;
        this->size = SqList_INCREMENT;
        return true;
    }
    else {
        this->data = new dataType[n];
        if (!this->data)
            return false;
        else {
            this->data = d;
        }
        this->length = n;
        this->size = n;
        return true;
    }
}


template<typename dataType>
bool SqList<dataType>::DestroySqList() {
    if (this->data) {
        delete[]this->data;
        return true;
    }
    return false;
}


template<typename dataType>
void SqList<dataType>::ClearSqList() {
    this->length = 0;
}
template<typename dataType>
bool SqList<dataType>::SqListEmpty() {
    if (this->length == 0)
        return true;
    return false;
}
template<typename dataType>
int SqList<dataType>::SqListLength() {
    return this->length;
}
template<typename dataType>
bool SqList<dataType>::GetElem(int i, dataType e) {
    if (i<0 || i>this->length)
        return false;
    e = this->data[i - 1];
    return false;
}


/**
    @brief
    @param  e - 用于比较的数据
    @param  c - 比较方式： = <  >
    @retval   - 第一个满足比较条件的数据的角标
**/
template<typename dataType>
int SqList<dataType>::LocatElem(dataType e, char c) {
    switch (c) {
    case '=':
        for (int i = 0; i < this->length; i++) {
            if (this->data[i] == e)
                return i;
        }
        return 0;
        break;
    case '>':
        for (int i = 0; i < this->length; i++) {
            if (this->data[i] > e)
                return i;
        }
        return 0;
        break;
    case'<':
        for (int i = 0; i < this->length; i++) {
            if (this->data[i] < e)
                return i;
        }
        return 0;
        break;
    }
}
template<typename dataType>
bool SqList<dataType>::PriorElme(dataType curE, dataType priorE) {
    for (int i = 0; i < this->length; i++) {
        if (curE == this->data[i]) {
            priorE = this->data[i - 1];
            return true;
        }
    }
    return false;
}
template<typename dataType>
bool SqList<dataType>::NextElme(dataType curE, dataType nextE) {
    for (int i = 0; i < this->length; i++) {
        if (curE == this->data[i]) {
            nextE = this->data[i + 1];
            return true;
        }
    }
    return false;
}

/**
    @brief
    @param  i - 插入的角标
    @param  e - 插入的数据
    @retval   -
**/
template<typename dataType>
bool SqList<dataType>::SqListInsert(int i, const dataType& e) {
    bool ret = false;
    if (this->length == 0 && i != 0) {
        return false;
    }
    if (i<0 || i>this->length + 1)
        ret = false;
    if (this->length + 1 > this->size) {
        dataType* temp = new dataType[this->size + SqList_INCREMENT];
        memcpy(temp, this->data, this->size + SqList_INCREMENT);
        delete[] this->data;
        this->data = temp;
        this->size += SqList_INCREMENT;
    }
    for (int j = this->length - 1; j >= i; j--) {
        this->data[j + 1] = this->data[j];
    }
    this->data[i] = e;
    this->length++;
    return true;
}

/**
    @brief
    @param  i - 删除数据的角标
    @param  e - 返回删除的数据
    @retval   -
**/
template<typename dataType>
bool SqList<dataType>::SqListDelete(int i, dataType* e) {
    if (i<0 || i>this->length - 1)
        return false;
    if (e != NULL) {
        *e = this->data[i];
    }
    for (int j = i + 1; j < this->length; j++) {
        this->data[j - 1] = this->data[j];
    }
    this->length--;
}
template<typename dataType>
bool SqListUnion(SqList<dataType>& L1, SqList<dataType>& L2) {
    for (int i = 0; i < L2.length; i++) {
        if (!LocatElem(L1, L2.data[i], '='))
            if (!SqListInsert(L1, i + 1, L2.data[i]))
                return false;
    }
    return true;
}
template<typename dataType>
void MergeSqList(SqList<dataType>& L1, SqList<dataType>& L2, SqList<dataType>& L3) {
    InitSqList(L3);
    int index;
    for (int i = 0; i < L1.length; i++) {
        if (index = LocatElem(L3, L1.data[i], '>')) {
            SqListInsert(L3, index, L1.data[i]);
        }
        else
            SqListInsert(L3, L3.length + 1, L1.data[i]);
    }
    for (int i = 0; i < L2.length; i++) {
        if (index = LocatElem(L3, L2.data[i], '>')) {
            SqListInsert(L3, index, L2.data[i]);
        }
        else
            SqListInsert(L3, L3.length + 1, L2.data[i]);
    }
}


template<typename dataType>
bool SqListCmp(dataType A, dataType B, char symbol) {
    if (symbol == '=') {
        if (A == B)
            return true;
        else
            return false;
    }
    else if (symbol == '<') {
        if (A < B)
            return true;
        else
            return false;
    }
}


template<typename dataType>
bool SqList<dataType>::SqListIncrseInsert(dataType& e, bool (*cmp)(dataType A, dataType B, char symbol)) {
    if (this->length == 0) {
        SqListInsert(0, e);
        return true;
    }
    uint left = 0;
    uint right = this->length - 1;
    while (right != left) {
        uint mid = left + (right - left) / 2;
        if (cmp(e, this->data[mid], '<')) {
            left = mid - 1;
        }
        else {
            right = mid + 1;
        }
    }
    SqListInsert(left, e);
    return true;
}

template<typename dataType>
SqList<dataType>::SqList() {
    data = new dataType[SqList_INCREMENT];
    length = 0;
    size = SqList_INCREMENT;
}

template<typename dataType>
SqList<dataType>::SqList(const SqList& p) {
    data = new dataType[p.length];
    for (int i = p.length - 1; i >= 0; i--) {
        this->data[i] = p.data[i];
    }
    this->length = p.length;
    this->size = p.length;
}

template<typename dataType>
SqList<dataType>::~SqList() {
    delete[]data;
}

/**
    @brief
    @param  nodeData - 寻找的数据
    @param  cmp      - 查找大于或者小于的比较函数
    @retval          - 查找到的数据第一次出现位置的角标
**/
template<typename dataType>
int SqList<dataType>::FindNode(dataType nodeData, bool(*cmp)(dataType A, dataType B, char symbol)) {
    dataType* ptrBegin = data;
    dataType* ptrEnd = ptrBegin + length - 1;

    std::vector<dataType> vec(ptrBegin, ptrEnd);
    return BinarySearch(vec, nodeData, cmp);
}