#include "paddle.h"

Paddle::Paddle(QObject *parent)
    : QObject{parent}
{}

// 挡板移动
void Paddle::moveLeft()//左移
{
    m_x -= m_speed;
}

void Paddle::moveRight()//右移
{
     m_x += m_speed;
}

// 绘制矩形挡板
void Paddle::draw(QPainter *painter)
{
    painter->setBrush(Qt::blue);//挡板内部填充颜色
    painter->setPen(Qt::black);//挡板边框颜色
    painter->drawRect(m_x, m_y, m_w, m_h);//绘制挡板
}

// 限制左右边界
void Paddle::checkBorder(int winWidth)
{
    //限定左边界
    if(m_x < 0)
        m_x = 0;
    //限定右边界
    if(m_x + m_w > winWidth)
        m_x = winWidth - m_w;
}

//挡板属性获取
int Paddle::paddle_x() const     { return m_x; }
int Paddle::paddle_y() const     { return m_y; }
int Paddle::paddle_width() const { return m_w; }
int Paddle::paddle_height() const{ return m_h; }

//游戏重启
//重置挡板位置
void Paddle::setPos(int x, int y)
{
    m_x = x;
    m_y = y;
}

//碰撞检测
QRect Paddle::paddle_rect() const
{
    return QRect(m_x, m_y, m_w, m_h);
}
