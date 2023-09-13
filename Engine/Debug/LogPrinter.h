#pragma once

#include "VarData.h"

#include <Windows.h>
#include <cstdint>
#include <string>
#include <sstream>
#include <iostream>

enum class LogPrinterSeverity {
	Unkown,
	Log,
	Warn,
	Error,
	Fatal
};

enum class LogPrinterButtons {
	AbortRetryIgnore,
	CancelTryAgainContinue,
	Help,
	Ok,
	OkCancel,
	RetryCancel,
	YesNo,
	YesNoCancel
};

class LogPrinter {
public:
	template<typename... Args>
	static uint8_t ShowMessageBox(LogPrinterSeverity severity, LogPrinterButtons buttons, uint32_t line, const char* function, Args... args);

	template<typename... Args>
	static void PrintSeverity(LogPrinterSeverity severity, uint32_t line, const char* function, Args... args);
};

template<typename ...Args>
inline uint8_t LogPrinter::ShowMessageBox(LogPrinterSeverity severity, LogPrinterButtons buttons, uint32_t line, const char* function, Args ...args)
{
	std::string title;

	title += std::string(function) + ":" + std::to_string(line) + " - ";

	uint32_t type = 0;
	switch (severity)
	{
	case LogPrinterSeverity::Log:
		title += "Log";
		type = MB_ICONINFORMATION;
		break;
	case LogPrinterSeverity::Warn:
		title += "Warn";
		type = MB_ICONWARNING;
		break;
	case LogPrinterSeverity::Error:
		title += "Error";
		type = MB_ICONSTOP;
		break;
	case LogPrinterSeverity::Fatal:
		title += "Fatal";
		type = MB_ICONEXCLAMATION;
		break;
	default:
		title += "Unkown";
		type = MB_ICONQUESTION;
		break;
	}

	switch (buttons)
	{
	case LogPrinterButtons::AbortRetryIgnore:
		type |= MB_ABORTRETRYIGNORE;
		break;
	case LogPrinterButtons::CancelTryAgainContinue:
		type |= MB_CANCELTRYCONTINUE;
		break;
	case LogPrinterButtons::Help:
		type |= MB_HELP;
		break;
	case LogPrinterButtons::Ok:
		type |= MB_OK;
		break;
	case LogPrinterButtons::OkCancel:
		type |= MB_OKCANCEL;
		break;
	case LogPrinterButtons::RetryCancel:
		type |= MB_RETRYCANCEL;
		break;
	case LogPrinterButtons::YesNo:
		type |= MB_YESNO;
		break;
	case LogPrinterButtons::YesNoCancel:
		type |= MB_YESNOCANCEL;
		break;
	default:
		type |= MB_OK;
		break;
	}

	std::ostringstream stream;

	([&]
		{
			stream << args;
		}(), ...);

#ifdef UNICODE
	std::wstring msgw = std::wstring(stream.str().begin(), stream.str().end());
	std::wstring titlew = std::wstring(title.begin(), title.end());

	const wchar_t* pmsg = msgw.c_str();
	const wchar_t* ptitle = titlew.c_str();

	return MessageBox(NULL, pmsg, ptitle, type | MB_TASKMODAL);
#else
	const char* pmsg = stream.str().c_str();
	const char* ptitle = title.c_str();

	return MessageBox(NULL, pmsg, ptitle, type | MB_TASKMODAL);
#endif
}

template<typename ...Args>
inline void LogPrinter::PrintSeverity(LogPrinterSeverity severity, uint32_t line, const char* function, Args ...args)
{
	std::ostringstream stream;

	stream << "[";

	switch (severity)
	{
	case LogPrinterSeverity::Log:
		stream << "Log";
		break;
	case LogPrinterSeverity::Warn:
		stream << "Warn";
		break;
	case LogPrinterSeverity::Error:
		stream << "Error";
		break;
	case LogPrinterSeverity::Fatal:
		stream << "Fatal";
		break;
	default:
		stream << "Unkown";
		break;
	}

	stream << "/" << function << ":" << line << "]: ";

	([&]
		{
			stream << args;
		}(), ...);

	std::cout << stream.str() << std::endl;
}

#define ShowMessageBox(severity, buttons, ...) LogPrinter::ShowMessageBox(severity, buttons, __LINE__, __FUNCTION__, __VA_ARGS__);
#define Log(...) LogPrinter::PrintSeverity(LogPrinterSeverity::Log, __LINE__, __FUNCTION__, __VA_ARGS__);
#define Warn(...) LogPrinter::PrintSeverity(LogPrinterSeverity::Warn, __LINE__, __FUNCTION__, __VA_ARGS__);
#define Error(...) LogPrinter::PrintSeverity(LogPrinterSeverity::Error, __LINE__, __FUNCTION__, __VA_ARGS__);
#define Fatal(...) LogPrinter::PrintSeverity(LogPrinterSeverity::Fatal, __LINE__, __FUNCTION__, __VA_ARGS__);