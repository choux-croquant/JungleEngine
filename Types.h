#pragma once

#include <vector>
#include <string>
#include <unordered_map>

// 기본 STL 컨테이너 타입 재정의
template <typename T>
using TArray = std::vector<T>;
using FString = std::string;

// 자료형 타입 재정의
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef float float32;
typedef double float64;