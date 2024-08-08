#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <source_location>
#include <sys/defs.h>

#define LogFatal(format, ...) artemis::Log(artemis::LogFatal, std::source_location::current(), format, ##__VA_ARGS__)
#define LogError(format, ...) artemis::Log(artemis::LogError, std::source_location::current(), format, ##__VA_ARGS__)
#define LogWarning(format, ...) artemis::Log(artemis::LogWarning, std::source_location::current(), format, ##__VA_ARGS__)
#define LogInfo(format, ...) artemis::Log(artemis::LogInfo, std::source_location::current(), format, ##__VA_ARGS__)
#define LogDebug(format, ...) artemis::Log(artemis::LogDebug, std::source_location::current(), format, ##__VA_ARGS__)
#define LogTrace(format, ...) artemis::Log(artemis::LogTrace, std::source_location::current(), format, ##__VA_ARGS__)
#define LogVerbose(format, ...) artemis::Log(artemis::LogVerbose, std::source_location::current(), format, ##__VA_ARGS__)

namespace artemis
{
enum LogLevel
{
	LogReset,
	LogFatal,
	LogError,
	LogWarning,
	LogInfo,
	LogDebug,
	LogTrace,
	LogVerbose,
};

void Log(LogLevel log_level, std::source_location location, const char* format, ...) __PRINTFLIKE(3, 4);
} // namespace artemis

#endif // LOGGER_HPP