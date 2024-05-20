#include "remotetower.h"
#include "place-line/mapplace.h"
#include "enemy/enemy.h"
RemoteTower::RemoteTower()
{

}

void RemoteTower::getTarget()
{
    for(auto &ele:roundPlace){
        if(ele->haveEnemy()){
            target=ele->getEnemy();
            connect(target,&Enemy::haveDeath,this,[&](){
                target=NULL;
                atkCount=0;
            });
            connect(target,&Enemy::haveMoveToNextPlace,this,[&](){
                getTarget();
            });
            return;
        }
    }
    target=NULL;
}

void RemoteTower::doAction()
{
    if(pos!=NULL)
    {
        if(target!=NULL){
            if(!hasSetMovie){
                setAttackMovie();
                hasSetMovie=true;
            }
            doAttack();
        }
        else{
            if(hasSetMovie){
                setStationaryLabel();
                hasSetMovie=false;
            }
            getTarget();
        }
    }
}

void RemoteTower::doAttack()
{
    atkCount++;
    if(atkCount==towerInterval)
    {
        getTarget();
        if(target!=NULL)
        {
            if(target->getHealth()<=atk){
                target->reduceHealth(atk);
                target=NULL;
            }
            else{target->reduceHealth(atk);}
        }
        atkCount=0;
    }
}

void RemoteTower::showAttackRange()
{
    for(auto &ele:roundPlace){
        ele->showAttackRange();
    }
}

void RemoteTower::hideAttackRange()
{
    for(auto &ele:roundPlace){
        ele->hideAttackRange();
    }
}

void RemoteTower::addPlace(MapPlace *mp)
{
    Tower::addPlace(mp);
    setRoundPlace(mp);

}

void RemoteTower::setRoundPlace(MapPlace *mp)
{
    rPos=mp->getRPos();
    cPos=mp->getCPos();
    if(rPos==0){
        if(cPos==0){
            roundPlace<<(*places)[rPos][cPos]<<(*places)[rPos+1][cPos]<<(*places)[rPos][cPos+1]<<(*places)[rPos+1][cPos+1];
        }
        else if(cPos==8){
            roundPlace<<(*places)[rPos+1][cPos-1]<<(*places)[rPos+1][cPos]<<(*places)[rPos][cPos-1]<<(*places)[rPos][cPos];
        }
        else{
            for(int j=cPos-1;j<=cPos+1;j++){
                for(int i=rPos;i<=rPos+1;i++){
                    roundPlace<<(*places)[i][j];
                }
            }
        }
    }
    else if(rPos==6){
        if(cPos==0){
            roundPlace<<(*places)[rPos][cPos]<<(*places)[rPos-1][cPos]<<(*places)[rPos][cPos+1]<<(*places)[rPos-1][cPos+1];
        }
        else if(cPos==8){
            roundPlace<<(*places)[rPos-1][cPos-1]<<(*places)[rPos-1][cPos]<<(*places)[rPos][cPos-1]<<(*places)[rPos][cPos];
        }
        else{
            for(int j=cPos-1;j<=cPos+1;j++){
                for(int i=rPos;i>=rPos-1;i--){
                    roundPlace<<(*places)[i][j];
                }
            }
        }
    }
    else if(cPos==0){
        for(int j=cPos;j<=cPos+1;j++){
            for(int i=rPos-1;i<=rPos+1;i++){
                roundPlace<<(*places)[i][j];
            }
        }
    }
    else if(cPos==8){
        for(int j=cPos-1;j<=cPos;j++){
            for(int i=rPos-1;i<=rPos+1;i++){
                roundPlace<<(*places)[i][j];
            }
        }
    }
    else{
        for(int j=cPos-1;j<=cPos+1;j++){
            for(int i=rPos-1;i<=rPos+1;i++){
                roundPlace<<(*places)[i][j];
            }
        }
    }
}

Gilgamesh::Gilgamesh(int tower_id)
{
    towerNumber=tower_id;
    stationaryPath = ":/tower/object/gilgamesh/gilgamesh.png";
    atkPath=":/tower/object/gilgamesh/gilgamesh_attack.gif";
    atk=5;
    maxHealth=10;
    curHealth=10;
    xOffest=-12;
    yOffest=-25;
    towerInterval=50;
    atkCount=25;
}

void Gilgamesh::doAction()
{
    RemoteTower::doAction();
}

void Gilgamesh::doAttack()
{
    atkCount++;
    if(atkCount==towerInterval-6){
        getTarget();
        emit shotBullet(QPoint(xPos,yPos),QPoint(target->getCurX(),target->getCurY()),3);
    }
    if(atkCount==towerInterval)
    {

        if(target!=NULL)
        {
            getTarget();
            if(target->getHealth()<=atk){
                target->reduceHealth(atk);
                target=NULL;
            }
            else{target->reduceHealth(atk);}
        }
        atkCount=0;
    }
}

void Gilgamesh::getTarget()
{
    for(auto &ele:roundPlace){
        if(ele->haveGroundEnemy()){
            target=ele->getGroundEnemy();
            connect(target,&Enemy::haveDeath,this,[&](){
                target=NULL;
                atkCount=0;
            });
            connect(target,&Enemy::haveMoveToNextPlace,this,[&](){
                getTarget();
            });
            return;
        }
    }
    target=NULL;
}

Emiya::Emiya(int tower_id)
{
    towerNumber=tower_id;
    stationaryPath = ":/tower/object/emiya/archer_stationary.png";
    atkPath=":/tower/object/emiya/aecher_attack.gif";
    atk=4;
    maxHealth=10;
    curHealth=10;
    xOffest=10;
    yOffest=-25;
    towerInterval=60;
    atkCount=30;
}

void Emiya::doAction()
{
    RemoteTower::doAction();
}

void Emiya::doAttack()
{
    atkCount++;
    if(atkCount==towerInterval-6){
        getTarget();
        emit shotBullet(QPoint(xPos,yPos),QPoint(target->getCurX(),target->getCurY()),4);
    }
    if(atkCount==towerInterval)
    {
        getTarget();
        if(target!=NULL)
        {
            if(target->getHealth()<=atk){
                target->reduceHealth(atk);
                target=NULL;
            }
            else{target->reduceHealth(atk);}
        }
        atkCount=0;
    }
}

Fufu::Fufu():beginAction(false)
{
    towerNumber=-1;
    xOffest=10;
    yOffest=-15;
    stationaryPath = ":/tower/object/merlin/fufu.png";
    atkPath=":/tower/object/merlin/fufu.png";
    actionFlag=":/image/circlePlus.png";
    improveFlag=":/image/doublePlus.png";
    atk=3;
    canClicked=false;
    ownPlus=new QLabel;
    QPixmap pix(":/image/circlePlus.png");
    ownPlus->setPixmap(pix);
    ownPlus->setAttribute(Qt::WA_TransparentForMouseEvents,true);
}

void Fufu::doAttack()
{
    if(beginAction)
    {
        for(auto &ele:roundPlace){
            if(ele->haveTower()){
                QPixmap pix(":/image/doublePlus.png");
                QLabel *doublePlus=new QLabel(parent);
                doublePlus->setPixmap(pix);
                doublePlus->setGeometry(ele->getTower()->getXPos()+10,ele->getTower()->getYPos()-20,20,12);
                doublePlus->setVisible(true);
                doublePlus->setAttribute(Qt::WA_TransparentForMouseEvents,true);
                doublePluses<<doublePlus;
                ele->getTower()->addHealth(atk);

            }
        }
        QTimer::singleShot(800,this,[&](){
            for(auto &ele:doublePluses){
                QLabel*tmp=ele;
                delete tmp;
            }
            doublePluses.clear();
        });
    }
}

void Fufu::drawBloodLine()
{
    return;
}

void Fufu::appear(MapPlace *mp)
{
    pos=mp;
    drawTower(mp->xSide()+xOffest,mp->ySide()+yOffest-100);
    QPropertyAnimation* animation = new QPropertyAnimation(this,"geometry");
    animation->setDuration(1000);
    animation->setStartValue(QRect(mp->xSide()+xOffest,mp->ySide()+yOffest-100,this->width()*0.25,this->height()*0.25));
    animation->setEndValue(QRect(mp->xSide()+xOffest,mp->ySide()+yOffest,this->width(),this->height()));
    animation->setEasingCurve(QEasingCurve::OutBounce);
    connect(animation,&QPropertyAnimation::finished,this,[&](){
        addPlace(pos);
        ownPlus->setGeometry(xPos+xOffest+10,yPos-30,20,20);
        ownPlus->setVisible(false);
        beginAction=true;
    });
    setVisible(true);
    animation->start();
}

void Fufu::drawTower(int x, int y)
{
    QPixmap pix(stationaryPath);
    setGeometry(x+xOffest,y+yOffest,60,60);
    xPos=x;
    yPos=y;
    setPixmap(pix);
    setVisible(true);
}

void Fufu::setActionPlus(bool ctr)
{
    if(ctr){
        ownPlus->setVisible(true);
    }
    else{
        ownPlus->setVisible(false);
    }
}

void Fufu::setParent(QWidget *parent)
{
    Tower::setParent(parent);
    this->parent=parent;
    ownPlus->setParent(parent);
}

bool Fufu::getCanBeAttack()
{
    return false;
}

void Fufu::dealDeath()
{
    delete ownPlus;
}

Merlin::Merlin(int tower_id)
{
    towerNumber=tower_id;
    stationaryPath = ":/tower/object/merlin/Merlin.png";
    atkPath=":/tower/object/merlin/Merlin.png";
    atk=0;
    maxHealth=25;
    curHealth=25;
    xOffest=-15;
    yOffest=-25;
    towerInterval=60;
    atkCount=0;
    fufu=new Fufu;
    fufu->setVisible(false);
    fufu->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    connect(this,&Tower::haveDeath,this,[&](){
        fufu->setVisible(false);
        fufu->dealDeath();
        towerInterval=99999;
    });
}

Merlin::~Merlin()
{
    delete fufu;
}

void Merlin::doAction()
{
    atkCount++;
    if(atkCount==45){
        fufu->setActionPlus(true);
    }
    if(atkCount==15){
        fufu->setActionPlus(false);
    }
    if(atkCount==towerInterval){
        fufu->doAttack();
        atkCount=0;
    }
}

void Merlin::setParent(QWidget *parent)
{
    Tower::setParent(parent);
    fufu->setParent(parent);
}

void Merlin::showAttackRange()
{
    fufu->showAttackRange();
}

void Merlin::hideAttackRange()
{
    fufu->hideAttackRange();
}

void Merlin::addPlace(MapPlace *mp)
{
    RemoteTower::addPlace(mp);
    QVector<int> tmp;
    for(int i=0;i<roundPlace.size();i++){
        tmp<<i;
    }
    unsigned seed = std::chrono::system_clock::now ().time_since_epoch ().count ();
    std::shuffle(tmp.begin(),tmp.end(),std::default_random_engine(seed));
    for(auto &ele:tmp){
        if(roundPlace[ele]->getCata()==0){
            fufu->appear(roundPlace[ele]);
            return;
        }
    }
    fufu->appear(mp);
}

void Merlin::setMapPlace(QVector<QVector<MapPlace *> > *pls)
{
    Tower::setMapPlace(pls);
    fufu->setMapPlace(pls);
}

void Merlin::setDeath()
{
    Tower::setDeath();
    fufu->setDeath();
}


