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
    //按键释放
    void keyReleaseEvent(QKeyEvent*)override;

private slots:
    void gameLoop();   // 游戏主循环定时器

    //砖块图案，定时切换
private:
    // 创建砖块图案
    void initBricksByMode(int mode);
    // 全部碰撞检测
    void collisionCheck();
    //当前模式
    int m_currentMode;
    // 每15秒切换一次的定时器
    QTimer* m_patternTimer;


    //动画小人
private:
    // 人物逐帧图片
    QPixmap m_roleIdle;//待机
    QPixmap m_roleLeft[2];//左移2帧
    QPixmap m_roleRight[2];//右移2帧

    QTimer* m_animTimer;//动画帧切换定时器
    int m_curFrame;// 当前第几帧 0/1
    // 记录当前移动状态
    bool m_moveLeftFlag;
    bool m_moveRightFlag;


    //跳跃相关
private:
    bool m_isJumping = false;//是否跳跃
    bool m_canJump=true;//能否跳跃
    int m_jumpOffset = 0;//当前向上偏移
    const int JUMP_SPEED_UP = 8;//上升速度
    const int JUMP_SPEED_DOWN = 8;//下落重力
    const int JUMP_MAX_HEIGHT = 80;//最大跳跃高度


    //分身技能相关
private:
    QVector<Ball*> m_allBalls; //存放全场所有弹珠
    void spawnSplitBalls();//技能:生成2个新球，不同贴图
    //两张贴图
private:
    QPixmap m_texBall2;
    QPixmap m_texBall3;

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
