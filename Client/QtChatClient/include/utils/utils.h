#pragma once
#include <public.h>

namespace Utils
{
    // 工具类,提供一些静态方法
    // eg:时间格式化等
    // 2024-05-22 14:52:00
    QString GetFormattedTime(int64_t dateTime, const QString &format = "yyyy-MM-dd HH:mm:ss");

    // 获取当前时间戳,单位秒
    int64_t GetCurrentTimestamp();

    // QByteArray转QIcon
    QIcon QByteArrayToQIcon(const QByteArray &data);

    // 二进制文件的读写
    QByteArray ReadFileToByteArray(const QString &filePath);
    bool WriteByteArrayToFile(const QString &filePath, const QByteArray &data);
}  // namespace Utils