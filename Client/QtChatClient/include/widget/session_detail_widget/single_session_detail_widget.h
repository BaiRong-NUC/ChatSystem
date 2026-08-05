#pragma once

#include <public.h>
#include <utils/log.h>
#include <model/data.h>
#include <widget/component/avatar_name.h>

/**
 * 单聊会话详情窗口
 */

namespace ChatWidget
{
    class SingleSessionDetailWidget final : public QWidget
    {
        Q_OBJECT

       private:
        void _InitSingleSessionDetailWidget(const Model::UserInfo &userInfo);  // 初始化UI界面

       public:
        explicit SingleSessionDetailWidget(const Model::UserInfo &userInfo, QWidget *parent = nullptr);
        ~SingleSessionDetailWidget() override = default;

       public:
        QPointer<AvatarName> m_avatarName;   // 用户头像+姓名组合控件
    };
}  // namespace ChatWidget