#include "enemy.h"
#include "place-line/mapplace.h"
#include "tower/tower.h"

Enemy::Enemy():
    xOffset(0),yOffset(0),curHealth(1),maxHealth(1),atk(1),speed(10),atkCount(0),enemyInterval(30),healthBarLength(100),\
    canMove(true),canDamage(false),canDoAction(true),flashCount(0),canVisible(true),hasChangeMovie(false),\
    hasPauseMovie(false),canFly(false),enemyMovie(NULL),val(3)
{
    flashTimer=new QTimer(this);
    flashTimer->setInterval(200);
    flashTimer->start();
    connect(flashTimer,&QTimer::timeout,this,[&](){
        if(flashCount>0){
            flashCount--;
            if(canVisible){
                setVisible(false);
                canVisible=false;
            }
            else{
                setVisible(true);
                canVisible=true;
            }
        }
    });

    healthBar=new QLabel;
    healthBarBack=new QLabel;
    healthBar->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    healthBarBack->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    healthBar->setStyleSheet("QLabel{border:1px solid rgb(0,0,0);}");
    healthBarBack->setStyleSheet("QLabel{background-color:rgb(255,0,0);}");
    valPath=":/image/stone+3.png";
}

void Enemy::reduceHealth(int h)
{
    flashCount+=2;
    curHealth-=h;
    qDebug()<<"敌人减少了"<<h<<"滴血, 剩余血量"<<curHealth<<",其编号为"<<enemyNumber;
    if(curHealth<=0){
        flashCount=0;
        flashTimer->stop();
        clear();
        setVisible(true);
        resize(65,31);
        QPixmap pix(valPath);
        setPixmap(pix);
        QTimer::singleShot(800,this,[=](){
            setVisible(false);
        });
        setDeath();
        qDebug()<<"敌人死了，序号"<<enemyNumber;
    }
}

void Enemy::doAction()
{

    if(canDoAction)
    {
        if(pos->getCata()==6){
            canDamage=false;
            canMove=false;
            canDoAction=false;
            pos->removeEnemy(enemyNumber);
            qDebug()<<"到达终点";
            setVisible(false);
            delete healthBar;
            delete healthBarBack;
            emit damage(enemyNumber);

        }
        else
        {
            targetSelect();
            if(canMove){doMove();}
            if(canDamage){
                doAttack();
            }
        }
    }
}

void Enemy::doAttack()
{
    atkCount++;
    if(atkCount==enemyInterval)
    {
        targetSelect();
        if(target!=NULL){
            target->ReduceHealth(atk);
            atkCount=0;
        }
        else{
            return;
        }

    }
}

void Enemy::drawEnemy()
{
    //QPixmap pix(stationaryPath);
    //setGeometry(curX+xOffset,curY+yOffset,85,99);
    setGeometry(curX+xOffset,curY+yOffset,60,75);
    healthBarLength=this->width();
    //setPixmap(pix);
    setVisible(true);
    setAttribute(Qt::WA_TransparentForMouseEvents,true);
    setMoveMovie();
    drawBloodLine();
}

void Enemy::doMove()
{
    if(curX>=targetX-20 && curX<=targetX+20 && curY>=targetY-2 && curY<=targetY+2 && pos->getCata()!=6){
        pos->removeEnemy(enemyNumber);
        pos=pos->getNextMapPlace();
        pos->addEnemy(this);
        emit haveMoveToNextPlace(enemyNumber);
        if(!pos->haveTower()){
            canMove=true;canDamage=false;
        }
        setTargetPlace();
    }

    curX+=moveX*speed;
    curY+=moveY*speed;
}

void Enemy::targetSelect()
{
    if(pos->haveTower()){
        target=pos->getTower();
        connect(target,&Tower::haveDeath,this,[&](){
            target=NULL;
        });
    }
    else{
        target=NULL;
    }
}

void Enemy::addHealth(int x)
{
    curHealth=min(curHealth+x,maxHealth);
    qDebug()<<"敌人增加了"<<x<<"滴血, 剩余血量"<<curHealth<<",其编号为:"<<enemyNumber;
}

void Enemy::update()
{
    //qDebug()<<"函数被调用";
    move(curX+xOffset,curY+yOffset);
    drawBloodLine();

}

void Enemy::controlMovie()
{
    if(!hasPauseMovie){
        enemyMovie->setPaused(true);
        hasPauseMovie=true;
    }
    else{
        enemyMovie->setPaused(false);
        hasPauseMovie=false;
    }
}

void Enemy::setDeath()
{
    //setVisible(false);
    canDamage=false;
    canMove=false;
    canDoAction=false;
    pos->removeEnemy(enemyNumber);
    delete healthBar;
    delete healthBarBack;
    emit haveDeath(enemyNumber,val);
    qDebug()<<"怪物死了";
}

void Enemy::setTargetPlace()
{
    targetX=pos->getNextMapPlace()->xSide();
    targetY=pos->getNextMapPlace()->ySide();
    QVector2D *targetVector = new QVector2D(targetX-curX,targetY-curY);
    targetVector->normalize();
    moveX=targetVector->x();
    moveY=targetVector->y();

    delete targetVector;
}

void Enemy::setPlace(MapPlace *p)
{
    pos=p;
    curX=pos->xSide();
    curY=pos->ySide();
}

void Enemy::setPlaces(QVector<QVector<MapPlace *> > *pls)
{
    places=pls;
}

void Enemy::setMoveMovie()
{
    if(enemyMovie!=NULL){
        delete enemyMovie;
    }
    enemyMovie = new QMovie(movePath);
    setMovie(enemyMovie);
    setScaledContents(true);
    enemyMovie->start();
}

void Enemy::setAttackMovie()
{
    if(enemyMovie!=NULL){
        delete enemyMovie;
    }
    enemyMovie = new QMovie(AttackPath);
    setMovie(enemyMovie);
    setScaledContents(true);
    enemyMovie->start();
}

void Enemy::drawBloodLine()
{
    healthBar->setGeometry(curX,curY-20,healthBarLength,5);
    healthBarBack->setGeometry(curX,curY-20,healthBarLength*double(double(curHealth)/double(maxHealth)),5);
    healthBar->setVisible(true);
    healthBarBack->setVisible(true);

}

void Enemy::setParent(QWidget *parent)
{
    QLabel::setParent(parent);
    healthBar->setParent(parent);
    healthBarBack->setParent(parent);
}

int Enemy::getHealth() const
{
    return curHealth;
}

int Enemy::getCurX() const
{
    return curX+moveX;
}

int Enemy::getCurY() const
{
    return curY+moveY;
}

bool Enemy::getCanDoAction() const
{
    return canDoAction;
}

bool Enemy::getCanFly() const
{
    return canFly;
}

GroundEnemy::GroundEnemy()
{

}

void GroundEnemy::targetSelect()
{
    if(pos!=NULL && pos->haveTower()){
        canMove=false;canDamage=true;
        if(hasChangeMovie==false){
            setAttackMovie();
        }
        hasChangeMovie=true;
        target=pos->getTower();
        connect(target,&Tower::haveDeath,this,[&](){
            target=NULL;
        });
    }
    else{
        atkCount=0;
        canMove=true;canDamage=false;
        if(hasChangeMovie==true){
            setMoveMovie();
        }
        hasChangeMovie=false;
        target=NULL;
    }
}

AirEnemy::AirEnemy()
{

}

void AirEnemy::targetSelect()
{
    Enemy::targetSelect();
}

RemoteEnemy::RemoteEnemy()
{

}

void RemoteEnemy::targetSelect()
{
    if(pos!=NULL)
    {
        if(pos->haveTower()){
            canMove=false;canDamage=true;
            if(hasChangeMovie==false){
                setAttackMovie();
            }
            hasChangeMovie=true;
            target=pos->getTower();
            connect(target,&Tower::haveDeath,this,[&](){
                target=NULL;
            });
            return;
        }
        for(auto &ele : roundPlace){
            if(ele->haveTower()){
                canMove=false;canDamage=true;
                if(hasChangeMovie==false){
                    setAttackMovie();
                }
                hasChangeMovie=true;
                target=ele->getTower();
                connect(target,&Tower::haveDeath,this,[&](){
                    target=NULL;
                });
                return;
            }
        }
        atkCount=0;
        canMove=true;canDamage=false;
        if(hasChangeMovie==true){
            setMoveMovie();
        }
        hasChangeMovie=false;
        target=NULL;
    }
}

void RemoteEnemy::doMove()
{
    if(curX>=targetX-25 && curX<=targetX+25 && curY>=targetY-2 && curY<=targetY+2 && pos->getCata()!=6){
        pos->removeEnemy(enemyNumber);
        pos=pos->getNextMapPlace();
        pos->addEnemy(this);
        roundPlace.clear();
        setRoundPlace();
        emit haveMoveToNextPlace(enemyNumber);
        if(!pos->haveTower()){
            canMove=true;canDamage=false;
        }
        setTargetPlace();
    }

    curX+=moveX*speed;
    curY+=moveY*speed;
}

void RemoteEnemy::setRoundPlace()
{
    rPos=pos->getRPos();
    cPos=pos->getCPos();
    //qDebug()<<rPos<<cPos;
    if(rPos==0){
        if(cPos==0){
            roundPlace<<(*places)[rPos+1][cPos+1]<<(*places)[rPos][cPos+1]<<(*places)[rPos][cPos]<<(*places)[rPos+1][cPos];
        }
        else if(cPos==8){
            roundPlace<<(*places)[rPos][cPos]<<(*places)[rPos+1][cPos]<<(*places)[rPos+1][cPos-1]<<(*places)[rPos][cPos-1];
        }
        else{
            for(int j=cPos+1;j>=cPos-1;j--){
                for(int i=rPos;i<=rPos+1;i++){
                    roundPlace<<(*places)[i][j];
                }
            }
        }
    }
    else if(rPos==6){
        if(cPos==0){
            roundPlace<<(*places)[rPos][cPos+1]<<(*places)[rPos-1][cPos+1]<<(*places)[rPos][cPos]<<(*places)[rPos-1][cPos];
        }
        else if(cPos==8){
            roundPlace<<(*places)[rPos][cPos]<<(*places)[rPos-1][cPos]<<(*places)[rPos-1][cPos-1]<<(*places)[rPos][cPos-1];
        }
        else{
            for(int j=cPos+1;j>=cPos-1;j--){
                for(int i=rPos;i>=rPos-1;i--){
                    roundPlace<<(*places)[i][j];
                }
            }
        }
    }
    else if(cPos==0){
        for(int j=cPos+1;j>=cPos;j--){
            for(int i=rPos-1;i<=rPos+1;i++){
                roundPlace<<(*places)[i][j];
            }
        }
    }
    else if(cPos==8){
        for(int j=cPos;j>=cPos-1;j--){
            for(int i=rPos-1;i<=rPos+1;i++){
                roundPlace<<(*places)[i][j];
            }
        }
    }
    else{
        for(int j=cPos+1;j>=cPos-1;j--){
            roundPlace<<(*places)[rPos][j];
            roundPlace<<(*places)[rPos-1][j];
            roundPlace<<(*places)[rPos+1][j];
        }
    }

}
