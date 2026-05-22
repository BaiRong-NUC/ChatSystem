#include <utils/log.h>

namespace Log
{
    LogLevel GLOBAL_LOG_LEVEL = LogLevel::INFO;

    void LogBuilder(const LogLevel &level, const QString &message, const QString &file, int line)
    {
        if (level < GLOBAL_LOG_LEVEL) { return; }

        switch (level)
        {
            case LogLevel::INFO:
                qInfo().noquote().nospace() << "[INFO " << file << ":" << line << "]:" << message;
                break;
            case LogLevel::WARNING:
                qWarning().noquote().nospace() << "[WARNING " << file << ":" << line << "]:" << message;
                break;
            case LogLevel::ERROR:
                qCritical().noquote().nospace() << "[ERROR " << file << ":" << line << "]:" << message;
                break;
        }
    }
}  // namespace Log
