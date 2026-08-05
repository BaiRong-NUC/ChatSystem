#pragma once

#include <public.h>
#include <utils/log.h>
#include <model/data.h>

/**
 * 头像+姓名组合控件
 */

namespace ChatWidget
{
#define AVATAR_NAME_WIDTH 65  // 头像+姓名组合控件的宽度

    class AvatarName final : public QWidget
    {
        Q_OBJECT

       private:
        void _InitAvatarName();  // 初始化UI界面

       public:
        // 一般被添加到布局管理器中,不需要设置父对象,但为了方便,仍然提供了父对象参数
        explicit AvatarName(const QIcon &avatarIcon, const QString &name, QWidget *parent = nullptr);
        ~AvatarName() override = default;

       public:
        QPointer<QPushButton> m_avatarButton;  // 用户头像按钮
        QPointer<QLabel> m_nameLabel;          // 用户姓名标签
    };
}  // namespace ChatWidget