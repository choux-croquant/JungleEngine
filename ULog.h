#pragma once

#ifndef ULOG_H
#define ULOG_H

#include <string>
#include <vector>
#include <mutex>
#include <cstdarg>

#include "Types.h"

class ULog
{
public:
	// 로그 메시지 포맷하고 저장
	static void Log(const char* category, const char* verbosity, const char* fmt, ...);

	static void DrawLogWindow();
private:
	// 로그 메시지를 저장하기 위한 벡터, 뮤텍스
	static TArray<FString> LogMessages;
	static std::mutex LogMutex;
};

/*
	 * A macro that logs a formatted message if the log category is active at the requested verbosity level.
	 *
	 * @param CategoryName   Name of the log category as provided to DEFINE_LOG_CATEGORY.
	 * @param Verbosity      Verbosity level of this message. See ELogVerbosity.
	 * @param Format         Format string literal in the style of printf.
*/
#define UE_LOG(Category, Verbosity, Format, ...) ULog::Log(#Category, #Verbosity, Format, __VA_ARGS__)

#endif