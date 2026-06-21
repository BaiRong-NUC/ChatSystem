#pragma once
#include <public.h>
#include <utils/log.h>

namespace ChatWidget
{
    class MessageEdit : public QLineEdit
    {
        Q_OBJECT
       private:
        explicit MessageEdit(QWidget *parent = nullptr);
        void _InitMessageEdit();         // 初始化消息输入框
        static MessageEdit *s_instance;  // 单例实例指针
       public:
        ~MessageEdit() override;
        static MessageEdit *GetInstance(QWidget *parent = nullptr);  // 获取单例实例的静态方法
    };
}  // namespace ChatWidget