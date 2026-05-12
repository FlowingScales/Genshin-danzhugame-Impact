#ifndef BRICK_H
#define BRICK_H

#include <QObject>
#include <QPainter>
#include <QRect>
#include <QPixmap>

class Brick : public QObject
{
    Q_OBJECT
public:
    explicit Brick(QObject *parent = nullptr);

    // 绘制砖块
    void draw(QPainter* painter);

    // 获取砖块，用于碰撞(返回矩形判定)
    QRect brick_rect() const;

    // 判断砖块是否仍存在
    bool isAlive() const;

    // 砖块被销毁
    void destroy();

    // 设置砖块位置和大小
    void setPos(int x, int y);

private:
    int m_x;
    int m_y;
    int m_w = 100;
    int m_h = 100;
    //是否存活
    bool m_isalive = true;
    //砖块贴图
    QPixmap m_tex;
};

#endif // BRICK_H
