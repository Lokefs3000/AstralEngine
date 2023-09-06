#pragma once

#include "Data/Enum/LogSeverity.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>

#ifdef _WIN32
#include <Windows.h>
#endif

static class Logger {
public:
	template<typename... Args>
	static void LogWithSeverity(LogSeverity severity, const char* function, uint32_t line, Args... args);
};

template<typename... Args>
inline void Logger::LogWithSeverity(LogSeverity severity, const char* function, uint32_t line, Args... args)
{
#ifdef _WIN32
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD wOldColorAttrs;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

	GetConsoleScreenBufferInfo(h, &csbiInfo);
	wOldColorAttrs = csbiInfo.wAttributes;

	switch (severity)
	{
	case LogSeverity::Debug:
		SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		break;
	case LogSeverity::Info:
		SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case LogSeverity::Warning:
		SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case LogSeverity::Error:
		SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_RED);
		break;
	case LogSeverity::Fatal:
		SetConsoleTextAttribute(h, FOREGROUND_RED);
		break;
	default:
		break;
	}
#endif

	std::ostringstream stream;

	stream << "[";

	switch (severity)
	{
	case LogSeverity::Debug:
		stream << "Debug";
		break;
	case LogSeverity::Info:
		stream << "Log";
		break;
	case LogSeverity::Warning:
		stream << "Warning";
		break;
	case LogSeverity::Error:
		stream << "Error";
		break;
	case LogSeverity::Fatal:
		stream << "Fatal";
		break;
	default:
		break;
	}

	stream << "/" << function << ":" << line << "]: ";

	([&]
		{
			stream << args;
		}(), ...);

	stream << std::endl;
	std::cout << stream.str();
}

#define Debug(...) Logger::LogWithSeverity(LogSeverity::Debug, __FUNCTION__, __LINE__, __VA_ARGS__);
#define Log(...) Logger::LogWithSeverity(LogSeverity::Info, __FUNCTION__, __LINE__, __VA_ARGS__);
#define Warning(...) Logger::LogWithSeverity(LogSeverity::Warning, __FUNCTION__, __LINE__, __VA_ARGS__);
#define Error(...) Logger::LogWithSeverity(LogSeverity::Error, __FUNCTION__, __LINE__, __VA_ARGS__);
#define Fatal(...) Logger::LogWithSeverity(LogSeverity::Fatal, __FUNCTION__, __LINE__, __VA_ARGS__);