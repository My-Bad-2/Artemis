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
		case LogFatal:
			return "\e[38;5;160m";
		case LogError:
			return "\e[38;5;88m";
		case LogWarning:
			return "\e[38;5;202m";
		case LogInfo:
			return "\e[38;5;40m";
		case LogDebug:
			return "\e[38;5;27m";
		case LogTrace:
			return "\e[38;5;33m";
		case LogVerbose:
			return "\e[38;5;22m";
	}
}
} // namespace details

void Log(LogLevel log_level, std::source_location location, const char* format, ...)
{
	constexpr std::string_view reset_color = details::LogColor(LogReset);
	std::string_view color = details::LogColor(log_level);

	fprintf(stderr, "%s[%s @ %s] -> ", color.data(), location.function_name(),
			location.file_name());

	va_list args = {};
	va_start(args, format);

	vfprintf(stderr, format, args);

	va_end(args);

	fprintf(stderr, reset_color.data());

	if(log_level == LogFatal)
	{
		arch::Halt(false);
	}
}
} // namespace artemis