#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "paddle.h"
#include <QPainter>
#include <QPixmap>
#include <QVector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 窗口大小
    this->resize(1150,900);

    //砖块图案
    m_currentMode=0;//初始化从模式-0开始
    initBricksByMode(m_currentMode);
    m_patternTimer = new QTimer(this);  //切换图案的定时器
    connect(m_patternTimer, &QTimer::timeout, this, [=](){
        m_currentMode = (m_currentMode + 1) % 5;
        initBricksByMode(m_currentMode);
    });//循环切换0—4模式
    m_patternTimer->start(15000);

    //创建对象
    m_ball   = new Ball(this);//弹珠
    m_paddle = new Paddle(this);//挡板
    m_score=0;//初始得分
    m_gameover=false;//初始游戏开始


    //动画小人

    //动画定时器
    m_animTimer = new QTimer(this);
    m_animTimer->setInterval(120);//120ms切换一帧
    connect(m_animTimer, &QTimer::timeout, this, [&](){
        m_curFrame = (m_curFrame + 1) % 2;//0-1循环
        this->update();//刷新画面
    });
    m_animTimer->start();

    // 初始状态
    m_curFrame = 0;
    m_moveLeftFlag = false;
    m_moveRightFlag = false;

    //加载芙宁娜帧图
    m_roleIdle.load(":/res/photo/Furrina_idle.png");//静止

    m_roleLeft[0].load(":/res/photo/Furrina_left01.png");//向左走
    m_roleLeft[1].load(":/res/photo/Furrina_left02.png");

    m_roleRight[0].load(":/res/photo/Furrina_right01.png");//向右走
    m_roleRight[1].load(":/res/photo/Furrina_right02.png");

    //游戏重启时的初始化
    m_ballStartX = this->width() / 2;//设置弹珠位置
    m_ballStartY = this->height() - 100;
    m_paddleStartX = this->width() / 2 - 50;//设置挡板位置

    //定时器游戏循环
    m_timer = new QTimer(this);
    m_timer->setInterval(15);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::gameLoop);
    m_timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}



//游戏循环:移动+碰撞+重绘


//移动

void MainWindow::gameLoop()
{
    m_ball->move();//弹珠移动
    m_paddle->checkBorder(this->width());//挡板边界限制

    collisionCheck();//碰撞全部检测

    if (m_ball->ball_rect().bottom() > this->height()) {
        m_gameover = true;
        m_timer->stop();       //停止游戏循环
        m_patternTimer->stop();//停止图形切换
    }//判断弹珠底部是否超出窗口



    //跳跃逻辑
    if(m_isJumping)
    {
        if(m_jumpOffset < JUMP_MAX_HEIGHT)
        {
            m_jumpOffset += JUMP_SPEED_UP; // 往上跳
        }
        else
        {
         //跳到最高处，停止上升，开始下落
            m_isJumping = false;
        }
    }
    else
    {
        // 重力自动落回去
        if(m_jumpOffset > 0)
        {
            m_jumpOffset -= JUMP_SPEED_DOWN;
        }
        else{
            m_canJump=true;//已落地，可起跳
        }
    }
    update();//刷新画面
}

//键盘控制挡板移动和跳跃
void MainWindow::keyPressEvent(QKeyEvent* e)
{
    if(e->key() == Qt::Key_Left)//左箭头向左移
    {
        m_paddle->moveLeft();
        m_moveLeftFlag  = true;
        m_moveRightFlag = false;
    }
     if(e->key() == Qt::Key_Right)//右箭头向右移
    {
        m_paddle->moveRight();
        m_moveRightFlag = true;
        m_moveLeftFlag  = false;
    }

    if((e->key() == Qt::Key_Up)&&m_canJump)//上箭头向上跳跃
    {
         m_isJumping = true;
        m_canJump=false;
    }


    //游戏结束按空格重置
    if (m_gameover && e->key() == Qt::Key_Space)
    {
        resetGame();
    }
}

//按键释放
void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    // 松开左右键 → 切回待机
    if(e->key() == Qt::Key_Left || e->key() == Qt::Key_Right)
    {
        m_moveLeftFlag  = false;
        m_moveRightFlag = false;
    }
}


// 碰撞
void MainWindow::collisionCheck()
{
    //球和挡板碰撞
    if(m_ball->ball_rect().intersects(m_paddle->paddle_rect()))
    {
        m_ball->reverseY();
    }

    //球和砖块碰撞
    for(auto &b : m_bricks)
    {
        if(b->isAlive() && m_ball->ball_rect().intersects(b->brick_rect()))
        {
            b->destroy();
            m_ball->reverseY();
            m_score+=10;//打一个砖+10分
        }
    }

    //窗口四边反弹

    //左右墙
    if(m_ball->ball_x() - m_ball->ball_r() <= 0 ||
        m_ball->ball_x() + m_ball->ball_r() >= this->width())
    {
        m_ball->reverseX();
    }
    //上下墙
    if(m_ball->ball_y() - m_ball->ball_r() <= 0)
    {
        m_ball->reverseY();
    }
}


// 绘制所有元素
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    //开始背景
    QPixmap bg(":/res/photo/yuanshen_start.png");

    //将图片铺满整个窗口
    if (!bg.isNull()) {
        p.drawPixmap(this->rect(), bg.scaled(this->size(), Qt::KeepAspectRatioByExpanding));
    }
    //游戏进行
    if(!m_gameover)
    {
        //画挡板
        QPixmap drawPic;

        if(m_moveLeftFlag)
        {
            drawPic = m_roleLeft[m_curFrame];
        }
        else if(m_moveRightFlag)
        {
            drawPic = m_roleRight[m_curFrame];
        }
        else
        {
            drawPic = m_roleIdle;
        }

        //缩放到挡板尺寸
        drawPic = drawPic.scaled(m_paddle->paddle_width(), m_paddle->paddle_height()+85,
                                 Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        //用挡板坐标绘制
        p.drawPixmap(m_paddle->paddle_x(), m_paddle->paddle_y()-m_jumpOffset, drawPic);

    //画弹珠
    m_ball->draw(&p);
    //画砖块
    for(auto &b : m_bricks) b->draw(&p);
    //绘制得分
    p.setPen(Qt::black);
    p.setFont(QFont("Arial", 20));
    p.drawText(10, 30, QString("分数: %1").arg(m_score));
    }
    //游戏结束
    if(m_gameover)
    {
        //切换成结束背景
        bg=QPixmap(":/res/photo/yuanshen_begin.png");
         //将图片铺满整个窗口
        if (!bg.isNull()) {
            p.drawPixmap(this->rect(), bg.scaled(this->size(), Qt::KeepAspectRatioByExpanding));
        }
        //发布提示语
        if (m_gameover) {
            p.setPen(Qt::black);
            p.setFont(QFont("Arial", 50, QFont::Bold));
            p.drawText(this->rect(), Qt::AlignCenter, "游戏结束!按空格键重新开始");
        }
    }
}


//重新开始
void MainWindow::resetGame()
{
    //重置得分和游戏
    m_score = 0;
    m_gameover = false;


    //重置弹珠位置和速度
    m_ball->setPos(m_ballStartX, m_ballStartY);
    m_ball->resetVelocity();

    //重置挡板位置
    m_paddle->setPos(m_paddleStartX, this->height() - 100);

    //重置跳跃状态
    m_isJumping=false;
    m_canJump=true;
    m_jumpOffset=0;

    //重新生成砖块(切回模式0)
    m_currentMode = 0;
    initBricksByMode(m_currentMode);

    //重启定时器
    m_timer->start();
    m_patternTimer->start();
}

//创建砖块图案
// 核心函数：根据模式生成不同图形的砖块
void MainWindow::initBricksByMode(int mode)
{
    // 先清空旧砖块
    for(auto &b : m_bricks) delete b;
    m_bricks.clear();

    int brickW = 100;
    int brickH = 100;
    int gap =1;
    int startX = 80;
    int startY = 70;

    switch(mode)
    {
    case 0: // 模式0：普通矩形（4行10列，原来的样子）
        for(int r=0; r<4; r++)
            for(int c=0; c<10; c++)
            {
                Brick* b = new Brick(this);
                b->setPos(startX + c*(brickW+gap), startY + r*(brickH+gap));
                m_bricks.append(b);
            }
        break;

    case 1: // 模式1：上宽下窄三角形
        for(int r=0; r<4; r++)
            for(int c=0; c<10-2*r; c++)
            {
                Brick* b = new Brick(this);
                b->setPos(startX + c*(brickW+gap) + r*(brickW+gap), startY + r*(brickH+gap));
                m_bricks.append(b);
            }
        break;

    case 2: // 模式2：上窄下宽倒三角形
        for(int r=0; r<4; r++)
            for(int c=0; c<4+2*r; c++)
            {
                Brick* b = new Brick(this);
                b->setPos(startX + c*(brickW+gap) + (3-r)*(brickW+gap), startY + r*(brickH+gap));
                m_bricks.append(b);
            }
        break;

    case 3: // 模式3：菱形
        for(int r=0; r<5; r++)
        {
            int cols = 4 + 2*((r<=2)? r : 4-r);
            for(int c=0; c<cols; c++)
            {
                Brick* b = new Brick(this);
                b->setPos(startX + c*(brickW+gap) + (4-cols/2)*(brickW+gap), startY + r*(brickH+gap));
                m_bricks.append(b);
            }
        }
        break;

    case 4: // 模式4：空心矩形
        for(int r=0; r<4; r++)
            for(int c=0; c<10; c++)
                if(r==0 || r==3 || c==0 || c==9)
                {
                    Brick* b = new Brick(this);
                    b->setPos(startX + c*(brickW+gap), startY + r*(brickH+gap));
                    m_bricks.append(b);
                }
        break;
    }
}
