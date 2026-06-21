#pragma once
#include <public.h>
#include <utils/log.h>

namespace ChatWidget
{
    class MessageWidget : public QScrollArea
    {
        Q_OBJECT
       private:
        explicit MessageWidget(QWidget *parent = nullptr);
        void _InitMessageWidget();         // 初始化消息信息区
        static MessageWidget *s_instance;  // 单例实例指针
       public:
        ~MessageWidget() override;
        static MessageWidget *GetInstance(QWidget *parent = nullptr);  // 获取单例实例
    };
}  // namespace ChatWidget