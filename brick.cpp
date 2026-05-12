#include "brick.h"

Brick::Brick(QObject *parent)
    : QObject{parent}
{
    //贴图路径
    m_tex.load(":/res/photo/qiuqiu_brick.png");
}

// 绘制砖块
void Brick::draw(QPainter *painter)
{
    // 已经打碎就不绘制
    if(!m_isalive)
    {
        return;
    }

    painter->drawPixmap(m_x, m_y, m_w, m_h,m_tex);
}

// 获取砖块碰撞矩形
QRect Brick::brick_rect() const
{
    return QRect(m_x, m_y, m_w, m_h);
}

// 判断砖块是否存活
bool Brick::isAlive() const
{
    return m_isalive;
}

// 销毁砖块
void Brick::destroy()
{
    m_isalive = false;
}

// 设置砖块坐标
void Brick::setPos(int x, int y)
{
    m_x = x;
    m_y = y;
}
