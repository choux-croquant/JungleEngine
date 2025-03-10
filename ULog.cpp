#include <chrono>
#include <ctime>

#include "ULog.h"
#include "ImGui/imgui.h"
#include "Types.h"

TArray<FString> ULog::LogMessages;  
std::mutex ULog::LogMutex;

void ULog::Log(const char* category, const char* verbosity, const char* fmt, ...)
{
	// fmt 문자열, 가변 인자 포맷해 buffer에 저장
	char buffer[1024];
	va_list args;	
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	// 타임스탬프
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	char timeBuffer[64];
	std::tm localTime;
	localtime_s(&localTime, &now_time);
	std::strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &localTime);

	// 문자열로 결합
	FString logEntry = FString(timeBuffer) + "[" + category + "][" + verbosity + "]" + buffer;
	{
		std::lock_guard<std::mutex> lock(LogMutex);
		LogMessages.push_back(logEntry);
	}
}

void ULog::DrawLogWindow()
{
	ImGui::Begin("Example : Console");

	if (ImGui::Button("Clear"))
	{
		std::lock_guard<std::mutex> lock(LogMutex);
		LogMessages.clear();
	}
	ImGui::SameLine();
	if (ImGui::Button("Copy"))
	{
		FString allLogs;
		{
			std::lock_guard<std::mutex> lock(LogMutex);
			for (const auto& msg : LogMessages)
			{
				allLogs += msg + "\n";
			}
		}
		ImGui::SetClipboardText(allLogs.c_str());
	}
	ImGui::Separator();
	static char filter[128] = "";
	ImGui::InputText("Filter", filter, IM_ARRAYSIZE(filter));
	ImGui::SameLine();
	ImGui::Text(": LogTemp, Error ...");
	ImGui::Separator();

	std::lock_guard<std::mutex> lock(LogMutex);  // lock LogMutex -> 여러 스레드 접근 시 발생하는 경쟁상태 예방
	for (const auto& msg : LogMessages)
	{
		if (strlen(filter) != 0 && msg.find(filter) == FString::npos)
			continue;

		ImVec4 color = ImGui::GetStyle().Colors[ImGuiCol_Text];

		if (msg.find("[Warning]") != FString::npos)
			color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
		else if (msg.find("[Error]") != FString::npos)
			color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		
		ImGui::PushStyleColor(ImGuiCol_Text, color);
		ImGui::TextWrapped("%s", msg.c_str());
		ImGui::PopStyleColor();
	}
	ImGui::End();
}