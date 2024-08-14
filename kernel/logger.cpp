#include <logger.hpp>
#include <stdio.h>
#include <arch.hpp>
#include <string_view>

namespace artemis
{
namespace details
{
constexpr std::string_view LogColor(LogLevel level)
{
	// Reference: docs/escape_sequences.md
	switch(level)
	{
		case LogReset:
			return "\e[0m\n";
		case LogPanic:
		case LogFatal:
			return "\e[38;5;210m";
		case LogError:
			return "\e[38;5;214m";
		case LogWarning:
			return "\e[38;5;226m";
		case LogInfo:
			return "\e[38;5;46m";
		case LogDebug:
			return "\e[38;5;51m";
		case LogTrace:
			return "\e[38;5;195m";
		case LogVerbose:
			return "\e[38;5;82m";
	}
}

constexpr std::string_view LogType(LogLevel level)
{
	switch(level)
	{
		case LogReset:
			return "";
		case LogPanic:
		case LogFatal:
			return "F";
		case LogError:
			return "E";
		case LogWarning:
			return "W";
		case LogInfo:
			return "I";
		case LogDebug:
			return "D";
		case LogTrace:
			return "T";
		case LogVerbose:
			return "V";
	}
}
} // namespace details

void Log(LogLevel log_level, const char* format, ...)
{
	constexpr std::string_view reset_color = details::LogColor(LogReset);
	std::string_view color = details::LogColor(log_level);

	fprintf(stderr, "%s[%s] ", color.data(), details::LogType(log_level).data());

	va_list args = {};
	va_start(args, format);

	vfprintf(stderr, format, args);

	va_end(args);

	fprintf(stderr, reset_color.data());

	if(log_level == LogPanic)
	{
		arch::Halt(false);
	}
}
} // namespace artemis