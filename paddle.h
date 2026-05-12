#ifndef PADDLE_H
#define PADDLE_H

#include <QObject>
#include <QPainter>
#include <QRect>

class Paddle : public QObject
{
    Q_OBJECT
public:
    explicit Paddle(QObject *parent = nullptr);

    // 挡板移动
    void moveLeft();//左移
    void moveRight();//右移

    // 绘制挡板
    void draw(QPainter* painter);

    // 边界限制：不让挡板移出窗口
    void checkBorder(int winWidth);

    // 只读获取
    int paddle_x() const;
    int paddle_y() const;
    int paddle_width() const;
    int paddle_height() const;

    //碰撞检测（包围盒）
    QRect paddle_rect() const;

    //游戏重启
     void setPos(int x, int y);//重置挡板位置

private:
    // 挡板左上角坐标
    int m_x = 525;
    int m_y = 800;

    // 挡板大小
    int m_w = 100;
    int m_h = 15;

    // 挡板移动速度
    int m_speed = 50;
};

#endif // PADDLE_H
