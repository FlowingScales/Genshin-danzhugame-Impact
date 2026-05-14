#ifndef BALL_H
#define BALL_H

#include <QObject>
#include <QPainter>
#include <QRect>
class Ball : public QObject
{
    Q_OBJECT
public:
    explicit Ball(QObject *parent = nullptr);

signals:

private:
    //弹珠圆心坐标
    int m_x=400;
    int m_y=450;
    //弹珠半径
    int m_r=25;
    //弹珠初速度
    int m_vx=4;
    int m_vy=-4;
public:
//弹珠移动
void move();

//弹珠绘制
void draw(QPainter* painter);

//碰撞反弹
void reverseX();//水平分速度
void reverseY();//竖直分速度
void set_ball_y(double y);//小球y坐标

//属性调用函数
int ball_x() const;
int ball_y() const;
int ball_r() const;
int ball_vx() const;
int ball_vy() const;

//游戏重置
void setPos(int x, int y);// 重置弹珠位置
void resetVelocity(); // 重置弹珠速度

//碰撞检测基础
QRect ball_rect() const;

};
#endif // BALL_H
