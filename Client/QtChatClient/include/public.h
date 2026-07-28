#pragma once

// model/data.h; utils/utils.h; utils/log.h;
#include <QString>     // 字符串类
#include <QIcon>       // 图标资源
#include <QByteArray>  // 二进制数据
#include <QUuid>       // 生成唯一ID

// utils/utils.h
#include <QDateTime>  // 获取当前时间戳
#include <QFile>      // 文件操作
#include <QFileInfo>  // 获取文件信息

// utils/log.h
#include <QDebug>  // 输出日志到控制台

// widget/*
#include <QWidget>      // 窗口基类
#include <QPushButton>  // 按钮
#include <QHBoxLayout>  // 水平布局
#include <QVBoxLayout>  // 垂直布局
#include <QGridLayout>  // 网格布局
#include <QLineEdit>    // 输入框
#include <QLabel>       // 标签
#include <QScrollArea>  // 滚动区域
#include <QScrollBar>   // 滚动条
#include <QCursor>
#include <QEvent>
#include <QResizeEvent>
#include <QFontMetrics>  // 计算字体需要的宽度

#include <QPainter>  // 绘制不规则对话框背景 TextChatMessage
#include <QPainterPath>

#define DEBUG_CODE 1  // 调试代码开关,在正式发布前可以关闭

// ChatMessage/TextChatMessage
#define CHAT_MESSAGE_WIDTH 0.6  // 聊天消息内容控件宽度占父元素的比例,0.6表示占父元素宽度的60%

// 默认聊天字体
#define DEFAULT_CHAT_FONT "微软雅黑"