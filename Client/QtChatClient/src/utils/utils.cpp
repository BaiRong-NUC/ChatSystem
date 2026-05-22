#include <utils/utils.h>

namespace Utils
{
    QIcon QByteArrayToQIcon(const QByteArray &data)
    {
        QPixmap pixmap;
        pixmap.loadFromData(data);
        return QIcon(pixmap);
    }

    QByteArray ReadFileToByteArray(const QString &filePath)
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "无法打开文件:" << filePath;
            return QByteArray();
        }
        return file.readAll();  // file.close()会在QFile对象析构时自动调用
    }

    bool WriteByteArrayToFile(const QString &filePath, const QByteArray &data)
    {
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly))
        {
            qWarning() << "无法打开文件:" << filePath;
            return false;
        }
        qint64 bytesWritten = file.write(data);
        if (bytesWritten == -1)
        {
            qWarning() << "写入文件失败:" << filePath;
            return false;
        }
        file.flush();  // 刷新缓冲区确保数据写入磁盘
        return true;   // file.close()会在QFile对象析构时自动调用
    }
}  // namespace Utils