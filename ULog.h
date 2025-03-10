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
	// �α� �޽��� �����ϰ� ����
	static void Log(const char* category, const char* verbosity, const char* fmt, ...);

	static void DrawLogWindow();
private:
	// �α� �޽����� �����ϱ� ���� ����, ���ؽ�
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