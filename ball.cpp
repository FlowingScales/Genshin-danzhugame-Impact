#include "ball.h"

Ball::Ball(QObject *parent)
    : QObject{parent}
{

}

//弹珠移动
void Ball:: move()
{
    m_x += m_vx;
    m_y += m_vy;
}

//弹珠绘制
void Ball::draw(QPainter* painter)
{
    QPixmap ballPic(":/res/photo/Surintendante_Chevalmarin.png");
    //将图片缩放到弹珠的大小
    ballPic = ballPic.scaled(2*m_r, 2*m_r, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    painter->drawPixmap(m_x-m_r, m_y-m_r, ballPic);
}

//碰撞反弹
void Ball::reverseX()//水平分速度
{
    m_vx = -m_vx;
}
void Ball::reverseY()//竖直分速度
{
    m_vy = -m_vy;
}

//弹珠属性获取
int Ball::ball_x() const     {return m_x;}
int Ball::ball_y() const     {return m_y;}
int Ball::ball_r() const     {return m_r;}
int Ball::ball_vx() const    {return m_vx;}
int Ball::ball_vy() const    {return m_vy;}

//游戏重启
//重置弹珠位置
void Ball::setPos(int x, int y)
{
    m_x = x;
    m_y = y;
}

// 重置弹珠速度
void Ball::resetVelocity()
{
    m_vx = 4;
    m_vy = -4;
}

//碰撞检测基础
QRect Ball::ball_rect() const
{
    return QRect(m_x - m_r, m_y - m_r,m_r * 2, m_r * 2);//矩形边框
}




