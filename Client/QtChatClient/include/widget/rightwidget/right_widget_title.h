#pragma once
#include <public.h>
#include <utils/log.h>

namespace ChatWidget
{
    class RightWidgetTitle : public QWidget
    {
        Q_OBJECT
       private:
        explicit RightWidgetTitle(QWidget *parent = nullptr);
        void _InitRightWidgetTitle();         // 初始化右侧信息区标题栏
        static RightWidgetTitle *s_instance;  // 单例实例指针
       public:
        ~RightWidgetTitle() override;
        static RightWidgetTitle *GetInstance(QWidget *parent = nullptr);  // 获取单例实例的静态方法

        QLabel *m_titleLabel;        // 标题标签
        QPushButton *m_titleButton;  // 标题按钮
    };
}  // namespace ChatWidget