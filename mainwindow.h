#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer> //定时器 实现游戏循环
#include <QVector> //容器 存放砖块
#include <QKeyEvent>
#include <QPixmap>
#include "ball.h"
#include "paddle.h"
#include "brick.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // 重写绘制
    void paintEvent(QPaintEvent*) override;
    //键盘事件
    void keyPressEvent(QKeyEvent*) override;

private slots:
    void gameLoop();   // 游戏主循环定时器

private:

    // 创建砖块图案
    void initBricksByMode(int mode);

    // 全部碰撞检测
    void collisionCheck();

    //当前模式
    int m_currentMode;

    // 每15秒切换一次的定时器
    QTimer* m_patternTimer;

private:
    QTimer* m_timer;//定时器
    Ball*    m_ball;//弹珠
    Paddle*  m_paddle;//挡板
    QVector<Brick*> m_bricks;//砖块数组
    int m_score;//游戏得分
    bool m_gameover;//游戏结束标志
    int m_ballStartX,m_ballStartY;//弹珠初始位置
    int m_paddleStartX;//挡板初始横坐标
    void resetGame();//游戏重置函数

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
