#include "tower.h"
#include "place-line/mapplace.h"
#include <QPixmap>
Tower::Tower():
    towerNumber(0),curHealth(20),maxHealth(20),atk(2),atkCount(0),towerInterval(30),healthBarLength(100),\
    pos(NULL),target(NULL),hasSetMovie(false),hasPauseMovie(false),hasClicked(false),canClicked(true),\
    towerMovie(NULL),canChangeDirect(false),haveChangeDirectButton(false)
{
    healthBar=new QLabel;
    healthBarBack=new QLabel;
    healthBar->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    healthBarBack->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    healthBar->setStyleSheet("QLabel{border:1px solid rgb(0,0,0);}");
    healthBarBack->setStyleSheet("QLabel{background-color:rgb(0,255,0);}");

    treasureButton=NULL;
    changeDirectButton=NULL;
    removeButton=new IconButton(":/image/removeLabel.png",0.8,0.8);
    //treasureButton=new IconButton(":/image/removeLabel.png",0.6,0.6);
    removeButton->setVisible(false);
    //treasureButton->setVisible(false);
    removeButton->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    //treasureButton->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    connect(removeButton,&QPushButton::clicked,this,[=](){
        emit hasCancelClick(towerNumber);
        hideAttackRange();
        setDeath();
    });
//    connect(treasureButton,&QPushButton::clicked,this,[=](){
//        setDeath();
//    });
    setAttribute(Qt::WA_TransparentForMouseEvents,false);
}

void Tower::ReduceHealth(int h)
{
    QMutex mutex;
    mutex.lock();
    curHealth-=h;
    qDebug()<<"防御塔减少了"<<h<<"滴血, 剩余血量"<<curHealth<<",其编号为:"<<towerNumber;
    if(curHealth<=0){
        setDeath();
    }
    mutex.unlock();
}

void Tower::addPlace(MapPlace* mp){
    pos=mp;
    mp->addTower(this);
    connect(pos,&MapPlace::hasAddEnemy,this,[&](){
        getTarget();
    });
}

void Tower::mouseReleaseEvent(QMouseEvent *event)
{
    if(canClicked)
    {
        if(!hasClicked){

            removeButton->setVisible(true);
            //treasureButton->setVisible(true);
            removeButton->setAttribute(Qt::WA_TransparentForMouseEvents,false);
            //treasureButton->setAttribute(Qt::WA_TransparentForMouseEvents,false);
            if(haveChangeDirectButton){
                changeDirectButton->setVisible(true);
                changeDirectButton->setAttribute(Qt::WA_TransparentForMouseEvents,false);
            }
            hasClicked=true;
            showAttackRange();
            emit hasBeenClick(towerNumber);
        }
        else{
            removeButton->setVisible(false);
            //treasureButton->setVisible(false);
            removeButton->setAttribute(Qt::WA_TransparentForMouseEvents,true);
            //treasureButton->setAttribute(Qt::WA_TransparentForMouseEvents,true);
            if(haveChangeDirectButton){
                changeDirectButton->setVisible(false);
                changeDirectButton->setAttribute(Qt::WA_TransparentForMouseEvents,true);
            }
            hasClicked=false;
            hideAttackRange();
            emit hasCancelClick(towerNumber);
        }
    }
}

void Tower::deleteClickLabel()
{
    delete removeButton;
    //delete treasureButton;
    if(haveChangeDirectButton){
        delete changeDirectButton;
    }
}

void Tower::setMapPlace(QVector<QVector<MapPlace *> > *pls)
{
    places=pls;
}

void Tower::addHealth(int x)
{
    curHealth=min(curHealth+x,maxHealth);
    qDebug()<<"防御塔增加了"<<x<<"滴血, 剩余血量"<<curHealth<<",其编号为:"<<towerNumber;
}

void Tower::drawTower(int x, int y)
{
    QPixmap pix(stationaryPath);
    setGeometry(x+xOffest,y+yOffest,85,99);
    xPos=x;
    yPos=y;
    setPixmap(pix);
    setVisible(true);
    healthBarLength=this->width()-20;
    drawBloodLine();
    removeButton->move(xPos+xOffest-80,yPos+yOffest+15);
    //treasureButton->move(xPos+xOffest+70,yPos+yOffest+15);
    if(haveChangeDirectButton){
        changeDirectButton->move(xPos+xOffest+70,yPos+yOffest+15);
    }
}

void Tower::controlMovie()
{
    if(hasSetMovie && !hasPauseMovie){
        towerMovie->setPaused(true);
        hasPauseMovie=true;
    }
    else if(hasSetMovie && hasPauseMovie){
        qDebug()<<"continue";
        towerMovie->setPaused(false);
        hasPauseMovie=false;
    }
}

void Tower::setDeath()
{
    deleteClickLabel();
    pos->removeTower();
    setVisible(false);
    setAttribute(Qt::WA_TransparentForMouseEvents,true);
    healthBar->setVisible(false);
    healthBarBack->setVisible(false);
    emit haveDeath(towerNumber);
    qDebug()<<"防御塔死了"<<",其编号为:"<<towerNumber;
}

void Tower::setParent(QWidget *parent)
{
    QLabel::setParent(parent);
    healthBar->setParent(parent);
    healthBarBack->setParent(parent);
    removeButton->setParent(parent);
    //treasureButton->setParent(parent);
    if(haveChangeDirectButton){
        changeDirectButton->setParent(parent);
    }
}

void Tower::setAttackMovie()
{
    towerMovie = new QMovie(atkPath);
    setMovie(towerMovie);
    setScaledContents(true);
    towerMovie->start();
}

void Tower::setStationaryLabel()
{
    if(towerMovie!=NULL){
        delete towerMovie;
        towerMovie=NULL;
    }
    QPixmap pix(stationaryPath);
    setScaledContents(false);
    setPixmap(pix);
    setVisible(true);
}

void Tower::setCanClicked(bool b)
{
    canClicked=b;
}

void Tower::drawBloodLine()
{
    healthBar->setGeometry(xPos,yPos-25,healthBarLength,5);
    healthBarBack->setGeometry(xPos,yPos-25,healthBarLength*double(double(curHealth)/double(maxHealth)),5);
    healthBar->setVisible(true);
    healthBarBack->setVisible(true);
}

void Tower::update()
{
    drawBloodLine();
}

void Tower::deleteBloodBar()
{
    delete healthBar;
    delete healthBarBack;
}

int Tower::getTowerNumber() const
{
    return towerNumber;
}

int Tower::getHealth() const
{
    return curHealth;
}

int Tower::getXPos() const
{
    return xPos;
}

int Tower::getYPos() const
{
    return yPos;
}
