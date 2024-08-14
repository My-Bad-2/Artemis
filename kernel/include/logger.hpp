#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <sys/defs.h>

#define LogPanic(format, ...) artemis::Log(artemis::LogLevel::LogPanic, format, ##__VA_ARGS__)
#define LogFatal(format, ...) artemis::Log(artemis::LogFatal, format, ##__VA_ARGS__)
#define LogError(format, ...) artemis::Log(artemis::LogError, format, ##__VA_ARGS__)
#define LogWarning(format, ...) artemis::Log(artemis::LogWarning, format, ##__VA_ARGS__)
#define LogInfo(format, ...) artemis::Log(artemis::LogInfo, format, ##__VA_ARGS__)
#define LogDebug(format, ...) artemis::Log(artemis::LogDebug, format, ##__VA_ARGS__)
#define LogTrace(format, ...) artemis::Log(artemis::LogTrace, format, ##__VA_ARGS__)
#define LogVerbose(format, ...) artemis::Log(artemis::LogVerbose, format, ##__VA_ARGS__)

namespace artemis
{
enum LogLevel
{
	LogReset,
	LogPanic,
	LogFatal,
	LogError,
	LogWarning,
	LogInfo,
	LogDebug,
	LogTrace,
	LogVerbose,
};

void Log(LogLevel log_level, const char* format, ...) __PRINTFLIKE(2, 3);
} // namespace artemis

#endif // LOGGER_HPP