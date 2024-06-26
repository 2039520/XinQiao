#pragma once
#include <vector>
#include <iostream>
using namespace std;

// 二分查找算法
template<typename dataType>
int BinarySearch(const std::vector<dataType>& arr, dataType target, bool (*cmp)(dataType A, dataType B, char symbol));


template<typename dataType>
int BinarySearch(const std::vector<dataType>& arr, dataType target, bool (*cmp)(dataType A, dataType B, char symbol)) {
    int low = 0;
    int high = arr.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (cmp(arr[mid], target, '=')) {
            return mid; // 找到目标元素，返回索引
        }
        else if (cmp(arr[mid], target, '<')) {
            low = mid + 1; // 目标在右半部分
        }
        else {
            high = mid - 1; // 目标在左半部分
        }
    }
    return -1; // 目标元素不存在
}