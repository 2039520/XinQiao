#pragma once
#define MAX_SqList_LENGTH  100
#define SqList_INCREMENT 10
#include"SearchAlgorithm.h"
template<typename dataType>
bool SqListCmp(dataType A, dataType B, char symbol);

template <typename dataType>
class SqList {
public:
    dataType* data;
    int length;
    int size;
    SqList() {
        data = new dataType[SqList_INCREMENT];
        length = 0;
        size = SqList_INCREMENT;
    }
    SqList(const SqList& p) {
        data = new dataType[p.length];
        for (int i = p.length - 1; i >= 0; i--) {
            this->data[i] = p.data[i];
        }
        this->length = p.length;
        this->size = p.length;
    }
    ~SqList() {
        delete[]data;
    }
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
    int FindNode(dataType nodeData, bool(*cmp)(dataType A, dataType B, char symbol) = SqListCmp) {
        dataType* ptrBegin = data;
        dataType* ptrEnd = ptrBegin + length - 1;

        std::vector<dataType> vec(ptrBegin, ptrEnd);
        return BinarySearch(vec, nodeData, cmp);
    }
};
template<typename dataType>
bool InitSqList(SqList<dataType>& L, dataType* d = NULL, int n = 0);
template<typename dataType>
bool DestroySqList(SqList<dataType>& L);
template<typename dataType>
void ClearSqList(SqList<dataType>& L);
template<typename dataType>
bool SqListEmpty(SqList<dataType>& L);
template<typename dataType>
int SqListLength(SqList<dataType>& L);
template<typename dataType>
bool GetElem(SqList<dataType>& L, int i, dataType e);

template<typename dataType>
int LocatElem(SqList<dataType>& L, dataType e, char c);
template<typename dataType>
bool PriorElme(SqList<dataType>& L, dataType curE, dataType priorE);
template<typename dataType>
bool NextElme(SqList<dataType>& L, dataType curE, dataType nextE);
template<typename dataType>
bool SqListInsert(SqList<dataType>& L, int i, dataType& e);
template<typename dataType>
bool SqListDelete(SqList<dataType>& L, int i, dataType* e = NULL);
template<typename dataType>
bool SqListUnion(SqList<dataType>& L1, SqList<dataType>& L2);
template<typename dataType>
void MergeSqList(SqList<dataType>& L1, SqList<dataType>& L2, SqList<dataType>& L3);
template<typename ElemType>
void PrintSqList(SqList<ElemType> l);
template<typename dataType>
bool SqListIncrseInsert(SqList<dataType>& l, dataType& e, bool (*cmp)(dataType A, dataType B, char symbol) = SqListCmp);


using namespace std;
template<typename dataType>
bool InitSqList(SqList<dataType>& L, dataType* d, int n) {
    if (n == 0) {
        L.data = new dataType[SqList_INCREMENT];
        if (!L.data)
            return false;
        L.length = 0;
        L.size = SqList_INCREMENT;
        return true;
    }
    else {
        L.data = new dataType[n];
        if (!L.data)
            return false;
        else {
            L.data = d;
        }
        L.length = n;
        L.size = n;
        return true;
    }
}
template<typename dataType>
bool DestroySqList(SqList<dataType>& L) {
    if (L.data) {
        delete[]L;
        return true;
    }
    return false;
}
template<typename dataType>
void ClearSqList(SqList<dataType>& L) {
    L.length = 0;
}
template<typename dataType>
bool SqListEmpty(SqList<dataType>& L) {
    if (L.length == 0)
        return true;
    return false;
}
template<typename dataType>
int SqListLength(SqList<dataType>& L) {
    return L.length;
}
template<typename dataType>
bool GetElem(SqList<dataType>& L, int i, dataType e) {
    if (i<0 || i>L.length)
        return false;
    e = L.data[i - 1];
    return false;
}

template<typename dataType>
int LocatElem(SqList<dataType>& L, dataType e, char c) {
    switch (c) {
        case '=':
            for (int i = 0; i < L.length; i++) {
                if (L.data[i] == e)
                    return i + 1;
            }
            return 0;
            break;
        case '>':
            for (int i = 0; i < L.length; i++) {
                if (L.data[i] > e)
                    return i + 1;
            }
            return 0;
            break;
        case'<':
            for (int i = 0; i < L.length; i++) {
                if (L.data[i] < e)
                    return i + 1;
            }
            return 0;
            break;
    }
}
template<typename dataType>
bool PriorElme(SqList<dataType>& L, dataType curE, dataType priorE) {
    for (int i = 0; i < L.length; i++) {
        if (curE == L.data[i]) {
            priorE = L.data[i - 1];
            return true;
        }
    }
    return false;
}
template<typename dataType>
bool NextElme(SqList<dataType>& L, dataType curE, dataType nextE) {
    for (int i = 0; i < L.length; i++) {
        if (curE == L.data[i]) {
            nextE = L.data[i + 1];
            return true;
        }
    }
    return false;
}


template<typename DataType>
void ZPReNew(DataType*& dataP, uint newSize) {
    DataType* newP = new DataType[newSize];
    memcpy(newP, dataP, newSize);
    delete dataP;
    dataP = newP;
}

template<typename dataType>
bool SqListInsert(SqList<dataType>& L, int i, dataType& e) {
    bool ret = false;
    if (i<0 || i>L.length + 1)
        ret = false;
    if (L.length + 1 > L.size) {
        dataType* temp = new dataType[L.size + SqList_INCREMENT];
        memcpy(temp, L.data, L.size + SqList_INCREMENT);
        delete [] L.data;
        L.data = temp;
        L.size += SqList_INCREMENT;
    }
    for (int j = L.length - 1; j >= i; j--) {
        L.data[j + 1] = L.data[j];
    }
    L.data[i] = e;
    L.length++;
    return true;
}
template<typename dataType>
bool SqListDelete(SqList<dataType>& L, int i, dataType* e) {
    if (i<0 || i>L.length - 1)
        return false;
    if (e != NULL) {
        *e = L.data[i];
    }
    for (int j = i + 1; j < L.length; j++) {
        L.data[j - 1] = L.data[j];
    }
    L.length--;
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
template<typename ElemType>
void PrintSqList(SqList<ElemType> l) {
    for (int i = 0; i < l.length; i++)
        cout << l.data[i] << endl;
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
bool SqListIncrseInsert(SqList<dataType>& l, dataType& e, bool (*cmp)(dataType A, dataType B, char symbol)) {
    if (l.length == 0) {
        SqListInsert(l, 0, e);
        return true;
    }
    uint left = 0;
    uint right = l.length - 1;
    while (right != left) {
        uint mid = left + (right - left) / 2;
        if (cmp(e, l.data[mid], '<')) {
            left = mid - 1;
        }
        else {
            right = mid + 1;
        }
    }
    SqListInsert(l, left, e);
    return true;
}
