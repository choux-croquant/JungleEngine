#pragma once

#include <vector>
#include <string>
#include <unordered_map>

// �⺻ STL �����̳� Ÿ�� ������
template <typename T>
using TArray = std::vector<T>;
using FString = std::string;

template <typename K, typename V>
using TMap = std::unordered_map<K, V>;

// �ڷ��� Ÿ�� ������
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef float float32;
typedef double float64;