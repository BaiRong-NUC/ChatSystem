#include <utils/utils.h>

namespace Utils
{
    QString GetFormattedTime(int64_t dateTime, const QString &format)
    {
        return QDateTime::fromSecsSinceEpoch(dateTime).toString(format);
    }

    int64_t GetCurrentTimestamp() { return QDateTime::currentSecsSinceEpoch(); }
}  // namespace Utils