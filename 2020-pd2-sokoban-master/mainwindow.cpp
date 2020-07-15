#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QLayout>
#include <QString>
#include <QtGlobal>
#include <QMessageBox>
#include <QtMath>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(720, 600);
    current_stage = 1;
    maxstage = 1;
    movenum = 0;
    setmenu();
    setResource();
}
MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *)
{
    //background blue
    QPainter groundPainter(this);
    groundPainter.drawPixmap(0, 0, QPixmap(":/res/Webp.net-resizeimage.png"));
}

void MainWindow::newgame()
{
    selectgame(1);
}

void MainWindow::setmenu()
{
    //setting
    int l = maxstage;
    maplabel = new QLabel**[height];
    for(int i = 0; i < height; i++)
    {
        maplabel[i] = new QLabel*[width];
        for(int j = 0; j < width; j++)
            maplabel[i][j] = nullptr;
    }
    mapground = new QLabel**[height];
    for(int i = 0; i < height; i++)
    {
        mapground[i] = new QLabel*[width];
        for(int j = 0; j < width; j++)
            mapground[i][j] = nullptr;
    }

    //menu layout

    mainWidget = new QWidget;
    //new game
    QPushButton *NewGame = new QPushButton;
    NewGame->setFocusPolicy(Qt::NoFocus);
    NewGame->setText("Level " + QString::number(maxstage));
    connect(NewGame, &QPushButton::clicked, this, [this, l]{selectgame(l);});
    //choose
    QPushButton *Choose = new QPushButton;
    Choose->setFocusPolicy(Qt::NoFocus);
    Choose->setText("Select Level");
    connect(Choose, SIGNAL(clicked()), this, SLOT(select()));
    //quit
    QPushButton *Quit = new QPushButton;
    Quit->setFocusPolicy(Qt::NoFocus);
    Quit->setText("Quit");
    connect(Quit, SIGNAL(clicked()), this, SLOT(close()));
    //buy background
    QPushButton *BuyBackground = new QPushButton;
    BuyBackground->setFocusPolicy(Qt::NoFocus);
    BuyBackground->setText("Buy Background");
    connect(BuyBackground, SIGNAL(clicked()), this, SLOT(buybackground()));
    //coin layout
    QLabel *coinlabel = new QLabel;
    coinlabel->setText(" Coin: ");
    coinnumlabel = new QLabel;
    coinnumlabel->setNum(coinnum);

    QHBoxLayout *coinlayout = new QHBoxLayout;
    coinlayout->addStretch();
    coinlayout->addWidget(coinlabel);
    coinlayout->addWidget(coinnumlabel);
    coinlayout->addStretch();

    QVBoxLayout *menulayoutup = new QVBoxLayout;
    menulayoutup->addStretch();
    menulayoutup->addWidget(NewGame);
    menulayoutup->addWidget(BuyBackground);
    menulayoutup->addStretch();

    QVBoxLayout *menulayoutmid = new QVBoxLayout;
    menulayoutmid->addStretch();
    menulayoutmid->addWidget(Choose);
    menulayoutmid->addWidget(Quit);
    menulayoutmid->addStretch();

    QHBoxLayout *menu = new QHBoxLayout;
    menu->addStretch();
    menu->addLayout(menulayoutup);
    menu->addLayout(menulayoutmid);
    menu->addStretch();
    menu->setSpacing(10);

    QVBoxLayout *menulayoutdone = new QVBoxLayout;
    menulayoutdone->addStretch();
    menulayoutdone->addLayout(menu);
    menulayoutdone->addLayout(coinlayout);
    menulayoutdone->addStretch();
    menulayoutdone->setMargin(0);
    menulayoutdone->setSpacing(10);

    mainWidget->setLayout(menulayoutdone);
    setCentralWidget(mainWidget);
}


void MainWindow::restartgame()
{
    //reset movenum
    movenum = 0;
    movenumlabel->setNum(movenum);

    //reset map
    gamemap = new Map(current_stage);
    for(int i = 0; i < gamemap->height; i++)
    {
        for(int j = 0; j < gamemap->width; j++)
        {
            maplabel[i][j]->setPixmap(Ground);
            gamemap->mapdata[i][j] = gamemap->mapbackup[i][j];
            if(gamemap->mapbackup[i][j] == WALL)
            {
                  maplabel[i][j]->setPixmap(Wall);
            }
            else if(gamemap->mapbackup[i][j] == PLAYER)
            {
                maplabel[i][j]->setPixmap(Player);
                current_position = {j, i};
            }
            else if(gamemap->mapbackup[i][j] == CRATES)
            {
                maplabel[i][j]->setPixmap(Crates);
            }
            else if(gamemap->mapbackup[i][j] == GOAL)
            {
                maplabel[i][j]->setPixmap(Goal);
            }
            else if(gamemap->mapbackup[i][j] ==COIN)
            {
                maplabel[i][j]->setPixmap(Coin);
            }
            else if(gamemap->mapbackup[i][j] ==BOMB)
            {
                maplabel[i][j]->setPixmap(Bomb);
            }
            else {
                maplabel[i][j]->setPixmap(Ground);
            }
        }
    }
    mainWidget->update();
}


void MainWindow::select()
{
    mainWidget = new QWidget;
    setCentralWidget(mainWidget);
    QPushButton **levelbutton = new QPushButton*[2];
    for(int i = 0; i < 2; i++)
    {
        levelbutton[i] = new QPushButton;
    }
    QHBoxLayout *levellayout = new QHBoxLayout;
    levellayout->addStretch();
    for(int i = 0; i < 2; i++)
    {
        levelbutton[i]->setText(QString::number(i+1));
        levellayout->addWidget(levelbutton[i]);
        connect(levelbutton[i], &QPushButton::clicked, this, [this, i]{selectgame(i+1);});
        if(maxstage < i + 1)
        {
            levelbutton[i]->setEnabled(false);
        }
        else {
            levelbutton[i]->setEnabled(true);
        }
    }
    levellayout->addStretch();
    mainWidget->setLayout(levellayout);
}
void MainWindow::selectgame(int l)
{
    movenum = 0;
    current_stage = l;
    if(maxstage<current_stage)
        maxstage = current_stage;
    mainWidget = new QWidget;
    setCentralWidget(mainWidget);
    gamemap = new Map(l);
    height = gamemap->height;
    width = gamemap->width;
    QGridLayout *MAP = new QGridLayout;
    MAP->setMargin(0);
    MAP->setSpacing(0);
    MAP->setSizeConstraint(QLayout::SetFixedSize);
    maplabel = new QLabel **[height];
    for(int i = 0; i < height; i++)
    {
        maplabel[i] = new QLabel *[width];
        for(int j = 0; j < width; j++)
        {
            maplabel[i][j] = new QLabel;
        }
    }
    mapground = new QLabel **[height];
    for(int i = 0; i < height; i++)
    {
        mapground[i] = new QLabel *[width];
        for(int j = 0; j < width; j++)
        {
            mapground[i][j] = new QLabel;
        }
    }
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            mapground[i][j]->setPixmap(Ground);
            if(gamemap->mapdata[i][j] == WALL)
            {
                  maplabel[i][j]->setPixmap(Wall);
            }
            else if(gamemap->mapdata[i][j] == PLAYER)
            {
                maplabel[i][j]->setPixmap(Player);
                current_position = {j, i};
            }
            else if(gamemap->mapdata[i][j] == CRATES)
            {
                maplabel[i][j]->setPixmap(Crates);

            }
            else if(gamemap->mapdata[i][j] == GOAL)
            {
                maplabel[i][j]->setPixmap(Goal);
            }
            else if(gamemap->mapdata[i][j] == COIN)
            {
                maplabel[i][j]->setPixmap(Coin);
            }
            else if(gamemap->mapdata[i][j] == BOMB)
            {
                maplabel[i][j]->setPixmap(Bomb);
            }
            else {
                maplabel[i][j]->setPixmap(Ground);
            }
            mapground[i][j]->setFixedSize(45,45);
            mapground[i][j]->setScaledContents(true);
            maplabel[i][j]->setFixedSize(45,45);
            maplabel[i][j]->setScaledContents(true);
            MAP->addWidget(mapground[i][j], i, j);
            MAP->addWidget(maplabel[i][j], i, j);
        }
    }
    //put map on the up
    QHBoxLayout *uplayout = new QHBoxLayout;
    uplayout->addStretch();
    uplayout->addLayout(MAP);
    uplayout->addStretch();

    //set menu,next level,restart button
    //restart
    QPushButton *restart =new QPushButton;
    restart->setText("Restart");
    restart->setFocusPolicy(Qt::NoFocus);
    connect(restart, SIGNAL(clicked()), this, SLOT(restartgame()));
    //next level
    nextlevel =new QPushButton;
    nextlevel->setText("Next Level");
    nextlevel->setFocusPolicy(Qt::NoFocus);
    connect(nextlevel, &QPushButton::clicked, this, [this, l]{selectgame(l+1);});
    nextlevel->setEnabled(false);
    //menu
    QPushButton *backtomenu = new QPushButton;
    backtomenu->setText("Menu");
    backtomenu->setFocusPolicy(Qt::NoFocus);
    connect(backtomenu, SIGNAL(clicked()), this, SLOT(setmenu()));

    //present move and coin num
    //movenum
    movenumlabel = new QLabel;
    movenumlabel->setNum(movenum);
    QLabel *movelabel = new QLabel;
    movelabel->setText(" step: ");
    coinnumlabel = new QLabel;
    //coinnum
    coinnumlabel->setNum(coinnum);
    QLabel *coinlabel = new QLabel;
    coinlabel->setText(" Coin: ");

    //coin layout put horizontal
    QHBoxLayout *coinlayout = new QHBoxLayout;
    coinlayout->addWidget(coinlabel);
    coinlayout->addWidget(coinnumlabel);
    //step layout put horizontal
    QHBoxLayout *steplayout = new QHBoxLayout;
    steplayout->addWidget(movelabel);
    steplayout->addWidget(movenumlabel);


    QHBoxLayout *downlayout = new QHBoxLayout;

    downlayout->addStretch();

    downlayout->addStretch();

    downlayout->addStretch();
    downlayout->addLayout(steplayout);
    downlayout->addLayout(coinlayout);
    downlayout->addStretch();

    downlayout->addStretch();

    downlayout->addStretch();

    downlayout->addWidget(restart);
    if(current_stage < 2)
    downlayout->addWidget(nextlevel);
    downlayout->addWidget(backtomenu);
    downlayout->addStretch();
    downlayout->addStretch();
    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->addStretch();
    mainlayout->addLayout(uplayout);
    mainlayout->addLayout(downlayout);
    mainlayout->addStretch();
    mainWidget->setLayout(mainlayout);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(gamemap->cratenum != 0)
    {

    switch(e->key())
    {
    case Qt::Key_A:
    case Qt::Key_Left:
        left();
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        right();
        break;
    case Qt::Key_W:
    case Qt::Key_Up:
        up();
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        down();
        break;
    default:
        break;
      }
    }

}

void MainWindow::left(){

        maplabel[current_position.y][current_position.x]->setPixmap(PLeft);
        if(gamemap->mapdata[current_position.y][current_position.x-1] == GROUND || gamemap->mapdata[current_position.y][current_position.x-1] == GOAL)
        {         
            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == COIN)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == BOMB)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            else{
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            current_position = {current_position.x - 1, current_position.y};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(PLeft);
            movenum++;
            movenumlabel->setNum(movenum);
        }
        else if(gamemap->mapdata[current_position.y][current_position.x-1] == COIN)
        {
            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == COIN)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == BOMB)
            {

                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);

            }
            else{
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            current_position = {current_position.x - 1, current_position.y};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(PLeft);
            movenum++;
            coinnum++;
            coinnumlabel->setNum(coinnum);
            movenumlabel->setNum(movenum);
        }
        else if(gamemap->mapdata[current_position.y][current_position.x-1] == BOMB)
        {
            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == COIN)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == BOMB)
            {

                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);

            }
            else{
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            gamemap->mapdata[current_position.y][current_position.x-1] = GROUND;
            maplabel[current_position.y][current_position.x-1]->setPixmap(Ground);
            current_position = {gamemap->start.x, gamemap->start.y};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(Player);

            movenum++;
            coinnum--;
            coinnumlabel->setNum(coinnum);
            movenumlabel->setNum(movenum);

        }

        else if (gamemap->mapdata[current_position.y][current_position.x-1] == CRATES && (gamemap->mapdata[current_position.y][current_position.x-2] == GROUND || gamemap->mapdata[current_position.y][current_position.x-2] == GOAL)) {

                if(gamemap->mapdata[current_position.y][current_position.x-2] == GOAL)
                    gamemap->cratenum--;
                if(gamemap->mapbackup[current_position.y][current_position.x-1] == GOAL)
                    gamemap->cratenum++;
                gamemap->mapdata[current_position.y][current_position.x-2] = CRATES;
                maplabel[current_position.y][current_position.x-2]->setPixmap(Crates);
                if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
                {
                    gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                    maplabel[current_position.y][current_position.x]->setPixmap(Goal);
                }
                else if(gamemap->mapbackup[current_position.y][current_position.x] == COIN)
                {
                    gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                    maplabel[current_position.y][current_position.x]->setPixmap(Ground);
                }
                else if(gamemap->mapbackup[current_position.y][current_position.x] == BOMB)
                {
                    gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                    maplabel[current_position.y][current_position.x]->setPixmap(Ground);
                }
                else{
                    gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                    maplabel[current_position.y][current_position.x]->setPixmap(Ground);
                }
            current_position = {current_position.x - 1, current_position.y};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(PLeft);
            movenum++;
            movenumlabel->setNum(movenum);
        }
        if(gamemap->cratenum == 0)
        {
            nextlevel->setEnabled(true);
                QMessageBox::information(this, tr(nullptr), tr("WIN!!"));
        }
        else {
            if(nextlevel->isEnabled())
                nextlevel->setEnabled(false);
        }
 }
void MainWindow::right(){
        maplabel[current_position.y][current_position.x]->setPixmap(PRight);
        if(gamemap->mapdata[current_position.y][current_position.x+1] == GROUND || gamemap->mapdata[current_position.y][current_position.x+1] == GOAL)
        {
            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == COIN)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);

            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == BOMB)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            else {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            current_position = {current_position.x + 1, current_position.y};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(PRight);
            movenum++;
            movenumlabel->setNum(movenum);
        }
        else if(gamemap->mapdata[current_position.y][current_position.x+1] == COIN)
        {
            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == COIN)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);

            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == BOMB)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            else {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            current_position = {current_position.x + 1, current_position.y};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(PRight);
            movenum++;
            coinnum++;
            coinnumlabel->setNum(coinnum);
            movenumlabel->setNum(movenum);
        }
        else if(gamemap->mapdata[current_position.y][current_position.x+1] == BOMB)
        {
            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == COIN)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);

            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == BOMB)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            else {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            gamemap->mapdata[current_position.y][current_position.x+1] = GROUND;
            maplabel[current_position.y][current_position.x+1]->setPixmap(Ground);
            current_position = {gamemap->start.x, gamemap->start.y};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(Player);
            movenum++;
            coinnum--;
            coinnumlabel->setNum(coinnum);
            movenumlabel->setNum(movenum);
        }
        else if (gamemap->mapdata[current_position.y][current_position.x+1] == CRATES && (gamemap->mapdata[current_position.y][current_position.x+2] == GROUND || gamemap->mapdata[current_position.y][current_position.x+2] == GOAL)) {


            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == COIN)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);

            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == BOMB)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            else {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            if(gamemap->mapdata[current_position.y][current_position.x+2] == GOAL)
                gamemap->cratenum--;
            if(gamemap->mapbackup[current_position.y][current_position.x+1] == GOAL)
                gamemap->cratenum++;
            gamemap->mapdata[current_position.y][current_position.x+2] = CRATES;
            maplabel[current_position.y][current_position.x+2]->setPixmap(Crates);
            current_position = {current_position.x + 1, current_position.y};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(PRight);
            movenum++;
            movenumlabel->setNum(movenum);
        }
        if(gamemap->cratenum == 0)
        {
            nextlevel->setEnabled(true);
                QMessageBox::information(this, tr(nullptr), tr("WIN!!"));
        }
        else {
            if(nextlevel->isEnabled())
                nextlevel->setEnabled(false);
        }
}
void MainWindow::up(){
        maplabel[current_position.y][current_position.x]->setPixmap(PUp);
        if(gamemap->mapdata[current_position.y-1][current_position.x] == GROUND || gamemap->mapdata[current_position.y-1][current_position.x] == GOAL)
        {

            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == COIN)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);

            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == BOMB)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);


            }
            else {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            current_position = {current_position.x , current_position.y-1};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(PUp);
            movenum++;
            movenumlabel->setNum(movenum);
        }
        else if(gamemap->mapdata[current_position.y-1][current_position.x] == COIN)
        {

            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == COIN)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);

            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == BOMB)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);


            }
            else {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            current_position = {current_position.x , current_position.y-1};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(PUp);
            movenum++;
            coinnum++;
            coinnumlabel->setNum(coinnum);
            movenumlabel->setNum(movenum);
        }
        else if(gamemap->mapdata[current_position.y-1][current_position.x] == BOMB)
        {

            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == COIN)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);

            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == BOMB)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);


            }
            else {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            gamemap->mapdata[current_position.y][current_position.x-1] = GROUND;
            maplabel[current_position.y][current_position.x-1]->setPixmap(Ground);
            current_position = {gamemap->start.x, gamemap->start.y};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(Player);
            movenum++;
            coinnum--;
            coinnumlabel->setNum(coinnum);
            movenumlabel->setNum(movenum);
        }
        else if (gamemap->mapdata[current_position.y-1][current_position.x] == CRATES && (gamemap->mapdata[current_position.y-2][current_position.x] == GROUND || gamemap->mapdata[current_position.y-2][current_position.x] == GOAL)) {


            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == COIN)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);

            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == BOMB)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);


            }
            else {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            if(gamemap->mapdata[current_position.y-2][current_position.x] == GOAL)
                gamemap->cratenum--;
            if(gamemap->mapbackup[current_position.y-1][current_position.x] == GOAL)
                gamemap->cratenum++;
            gamemap->mapdata[current_position.y - 2][current_position.x] = CRATES;
            maplabel[current_position.y - 2][current_position.x]->setPixmap(Crates);
            current_position = {current_position.x , current_position.y-1};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(PUp);
            movenum++;
            movenumlabel->setNum(movenum);
        }
        if(gamemap->cratenum == 0)
        {
            nextlevel->setEnabled(true);
                QMessageBox::information(this, tr(nullptr), tr("WIN!!"));
        }
        else {
            if(nextlevel->isEnabled())
                nextlevel->setEnabled(false);
        }
}
void MainWindow::down(){

        maplabel[current_position.y][current_position.x]->setPixmap(PDown);
        if(gamemap->mapdata[current_position.y+1][current_position.x] == GROUND || gamemap->mapdata[current_position.y+1][current_position.x] == GOAL)
        {

            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == COIN)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);

            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == BOMB)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);

            }
            else {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            current_position = {current_position.x , current_position.y+1};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(PDown);
            movenum++;
            movenumlabel->setNum(movenum);
        }
        else if(gamemap->mapdata[current_position.y+1][current_position.x] == COIN)
        {

            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == COIN)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);

            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == BOMB)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);


            }
            else {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            current_position = {current_position.x , current_position.y+1};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(PDown);
            movenum++;
            coinnum++;
            coinnumlabel->setNum(coinnum);
            movenumlabel->setNum(movenum);

        }
        else if(gamemap->mapdata[current_position.y+1][current_position.x] == BOMB)
        {

            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == COIN)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);

            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == BOMB)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);


            }
            else {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            gamemap->mapdata[current_position.y+1][current_position.x] = GROUND;
            maplabel[current_position.y+1][current_position.x]->setPixmap(Ground);
            current_position = {gamemap->start.x, gamemap->start.y};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(Player);
            movenum++;
            coinnum--;
            coinnumlabel->setNum(coinnum);
            movenumlabel->setNum(movenum);

        }
        else if (gamemap->mapdata[current_position.y+1][current_position.x] == CRATES && (gamemap->mapdata[current_position.y+2][current_position.x] == GROUND || gamemap->mapdata[current_position.y+2][current_position.x] == GOAL)) {


            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == COIN)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);

            }
            else if(gamemap->mapbackup[current_position.y][current_position.x] == BOMB)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);


            }
            else {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            if(gamemap->mapdata[current_position.y+2][current_position.x] == GOAL)
                gamemap->cratenum--;
            if(gamemap->mapbackup[current_position.y+1][current_position.x] == GOAL)
                gamemap->cratenum++;
            gamemap->mapdata[current_position.y+2][current_position.x] = CRATES;
            maplabel[current_position.y+2][current_position.x]->setPixmap(Crates);
            current_position = {current_position.x , current_position.y+1};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(PDown);
            movenum++;
            movenumlabel->setNum(movenum);
        }
        if(gamemap->cratenum == 0)
        {
            nextlevel->setEnabled(true);
                QMessageBox::information(this, tr(nullptr), tr("WIN!!"));
        }
        else {
            if(nextlevel->isEnabled())
                nextlevel->setEnabled(false);
        }

}

void MainWindow::setResource()
{
    Wall.load(":/res/wall.png");
    Ground.load(":/res/ground.png");
    Crates.load(":/res/box.png");
    Player.load(":/res/PDown.png");
    Goal.load(":/res/goal.png");
    PLeft.load(":/res/PLeft.png");
    PUp.load(":/res/PUp.png");
    PRight.load(":/res/PRight.png");
    PDown.load(":/res/PDown.png");
    Coin.load(":/res/coin.jpg");
    Bomb.load(":/res/bomb.jpg");
    Poop.load(":/res/poop.png");

}
//buy background widget
void MainWindow::buybackground()
{
    mainWidget = new QWidget;
    QPushButton *Background1 = new QPushButton;
    QPushButton *Background2 = new QPushButton;
    QPushButton *tomenu = new QPushButton;
    QVBoxLayout *menulayout = new QVBoxLayout;
    QHBoxLayout *menu = new QHBoxLayout;
    tomenu->setFocusPolicy(Qt::NoFocus);
    Background1->setFocusPolicy(Qt::NoFocus);
    Background2->setFocusPolicy(Qt::NoFocus);
    Background1->setText("pink:$5");
    Background2->setText("white:$10");
    connect(Background1, SIGNAL(clicked()), this, SLOT(background1()));
    connect(Background2, SIGNAL(clicked()), this, SLOT(background2()));
    tomenu->setText("Menu");
    connect(tomenu, SIGNAL(clicked()), this, SLOT(setmenu()));
    coinnumlabel = new QLabel;
    coinnumlabel->setNum(coinnum);
    QLabel *coinlabel = new QLabel;
    coinlabel->setText(" Coin: ");
    QHBoxLayout *coinlayout = new QHBoxLayout;
    coinlayout->addWidget(coinlabel);
    coinlayout->addWidget(coinnumlabel);
    menulayout->addStretch();
    menulayout->addWidget(Background1);
    menulayout->addWidget(Background2);
    menulayout->addWidget(tomenu);
    menulayout->addLayout(coinlayout);
    menulayout->addStretch();
    menu->addStretch();
    menu->addLayout(menulayout);
    menu->addStretch();
    mainWidget->setLayout(menu);
    setCentralWidget(mainWidget);

}
//pink background
void MainWindow::background1()
{
    if(coinnum>=5)
    {
      coinnum-=5;
      coinnumlabel->setNum(coinnum);
     QMessageBox::information(this, tr(nullptr), tr("You got pink as your background!"));
     setStyleSheet("background-color:pink");

    }else {
        QMessageBox::information(this, tr(nullptr), tr("Poor you"));
}

}
//white background
void MainWindow::background2()
{
    if(coinnum>=10)
    {
      coinnum-=10;
      coinnumlabel->setNum(coinnum);
     QMessageBox::information(this, tr(nullptr), tr("You got white as your background!"));
     setStyleSheet("background-color:white");

    }else {
        QMessageBox::information(this, tr(nullptr), tr("Poor you"));
}

}


