#include "selectlevel.h"
#include"iconbutton.h"
#include"gamescene.h"

#include <QPainter>
#include<QTimer>
SelectLevel::SelectLevel(QWidget *parent)
    : QMainWindow{parent}
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(("fate grand order VS genshin"));
    setFixedSize(750,500);
    mainPath=":/image/choselevel.png";
    iconPath=":/image/icon_of_exe.png";
    setWindowIcon(QIcon(iconPath));
    //paint background


    //constuct a button
    IconButton *st=new IconButton(":/image/select_level1.png",0.8,0.8,":/image/select_level2.png",true);
    st->setParent(this);
    st->move(520,280);
    //jump to game
    connect(st,&IconButton::clicked,[=](){
        st->changeIcon();
        st->zoomDown();
        st->zoomUp();
        QTimer::singleShot(250,this,[=](){
            st->changeIcon();
            this->hide();
            GameScene *gameScene=new GameScene(1);
            gameScene->show();
            connect(gameScene,&GameScene::gameClose,this,[=](){
                QTimer::singleShot(500,this,[=](){
                    this->show();
                });
            });
        });       
    });

    //free to design level
    IconButton *autoLevel=new IconButton(":/image/select_level1.png",0.8,0.8,":/image/select_level2.png",true);
    autoLevel->setParent(this);
    autoLevel->move(400,200);

    connect(autoLevel,&IconButton::clicked,[=](){
        autoLevel->changeIcon();
        autoLevel->zoomDown();
        autoLevel->zoomUp();
        QTimer::singleShot(250,this,[=](){
            autoLevel->changeIcon();
            this->hide();
            GameScene *gameScene=new GameScene(0);
            gameScene->show();
            connect(gameScene,&GameScene::gameClose,this,[=](){
                QTimer::singleShot(500,this,[=](){
                    this->show();
                });
            });
        });
    });

}
void SelectLevel::paintEvent(QPaintEvent*){
    QPainter painter(this);
    QPixmap pix;
    pix.load(mainPath);
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}
