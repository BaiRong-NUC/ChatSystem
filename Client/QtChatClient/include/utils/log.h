#pragma once
#include <public.h>
#include <utils/utils.h>
namespace Log
{
    enum class LogLevel
    {
        INFO,
        WARNING,
        ERROR,
    };

    extern LogLevel GLOBAL_LOG_LEVEL;  // 全局日志级别,默认为INFO,在log.cpp中定义

    static inline void SetLogLevel(const LogLevel &level) { GLOBAL_LOG_LEVEL = level; }

    void LogBuilder(const LogLevel &level, const QString &message, const QString &file, int line);
}  // namespace Log

#define LogInfo(level, message) Log::LogBuilder(level, message, Utils::GetFileNameFromPath(QString(__FILE__)), __LINE__)
