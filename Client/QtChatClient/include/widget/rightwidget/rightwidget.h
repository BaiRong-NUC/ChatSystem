#pragma once
#include <public.h>
namespace ChatWidget
{
    class RightWidget : public QWidget
    {
        Q_OBJECT
       private:
        explicit RightWidget(QWidget *parent = nullptr);
        // void _InitRightWidget();         // 初始化右侧信息区
        static RightWidget *s_instance;  // 单例实例指针
       public:
        ~RightWidget() override;
        static RightWidget *GetInstance(QWidget *parent = nullptr);  // 获取单例实例的静态方法
    };
}  // namespace ChatWidget