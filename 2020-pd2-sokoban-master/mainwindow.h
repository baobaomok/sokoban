#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QKeyEvent>
#include <QTimer>
#include <QPushButton>
#include <QPainter>
#include "map.h"
#ifdef totallevel
#undef totallevel
#endif

namespace Ui {
class MainWindow;
}
enum{LEFT = 1, RIGHT = 2, UP = 3, DOWN = 4};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setResource();
    void keyPressEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *);

    ~MainWindow();
private:
    Ui::MainWindow *ui;
    point current_position={0,0};
    QLabel ***maplabel,***mapground,*movenumlabel=nullptr,*coinnumlabel;
    QWidget *mainWidget;
    QPixmap Wall, Ground, Crates, Player, Goal, PLeft, PUp, PRight, PDown,Coin,Bomb,Poop;
    Map *gamemap=nullptr;
    QPushButton *nextlevel=nullptr;
    int height=0;
    int width=0;
    int current_stage;
    int maxstage;
    int movenum;
    int coinnum=0;
    int background=0;
private slots:
    void newgame();
    void select();
    void setmenu();
    void restartgame();
    void selectgame(int);
    void buybackground();
    void background1();
    void background2();
    void up();
    void down();
    void left();
    void right();
};

#endif // MAINWINDOW_H

