#pragma once
#include <public.h>

namespace Utils
{
    // 工具类,提供一些静态方法
    // eg:时间格式化等
    // 2024-05-22 14:52:00
    static inline QString GetFormattedTime(int64_t dateTime, const QString &format = "yyyy-MM-dd HH:mm:ss")
    {
        return QDateTime::fromSecsSinceEpoch(dateTime).toString(format);
    }
    // 获取当前时间戳,单位秒
    static inline int64_t GetCurrentTimestamp() { return QDateTime::currentSecsSinceEpoch(); }

    // QByteArray转QIcon
    QIcon QByteArrayToQIcon(const QByteArray &data);

    // 二进制文件的读写
    QByteArray ReadFileToByteArray(const QString &filePath);
    bool WriteByteArrayToFile(const QString &filePath, const QByteArray &data);

    // 根据文件路径获取文件名
    static inline QString GetFileNameFromPath(const QString &filePath) { return QFileInfo(filePath).fileName(); }
}  // namespace Utils