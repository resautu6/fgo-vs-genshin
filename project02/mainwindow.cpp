#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iconbutton.h"

#include <QPushButton>
#include <QPainter>
#include <QTimer>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    //information of mainwindow
    setWindowTitle(("fate grand order VS genshin"));
    setFixedSize(1080,675);
    mainPath=":/image/title.png";
    iconPath=":/image/icon_of_exe.png";
    setWindowIcon(QIcon(iconPath));
    selectLevel=new SelectLevel;
    //constuct a button
    IconButton *st=new IconButton(":/image/playgame.png");
    st->setParent(this);
    st->move(340,500);
    //jump to choice level
    connect(st,&IconButton::clicked,[=](){
       st->zoomDown();
       st->zoomUp();
       QTimer::singleShot(500,this,[=](){
           this->hide();
           selectLevel->show();
       });

    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPixmap pix;
    pix.load(mainPath);
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}

