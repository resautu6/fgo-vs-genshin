#include "meleetower.h"
#include "enemy/enemy.h"
#include "place-line/mapplace.h"
MeleeTower::MeleeTower()
{

}

void MeleeTower::getTarget(){

    if(pos==NULL || !pos->haveEnemy()) return;
    else if(pos->haveEnemy()){
        target=pos->getEnemy();
            connect(target,&Enemy::haveDeath,this,[&](){
                target=NULL;
                atkCount=0;
            });
            connect(target,&Enemy::haveMoveToNextPlace,this,[&](){
            getTarget();
        });
    }
}

void MeleeTower::doAction()
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

void MeleeTower::doAttack()
{
    atkCount++;
    if(atkCount==towerInterval)
    {
        getTarget();
        if(target!=NULL)
        {
            if(target->getHealth()<=atk){
                Q_ASSERT(target);
                target->reduceHealth(atk);
                target=NULL;
            }
            else{target->reduceHealth(atk);}
        }


        atkCount=0;
    }
}

void MeleeTower::showAttackRange()
{
    pos->showCata();
}

void MeleeTower::hideAttackRange()
{
    pos->hideCata();
}

Artoria::Artoria(int tower_id)
{
    towerNumber=tower_id;
    stationaryPath = ":/tower/object/artonia/Artoria.png";
    atkPath=":/tower/object/artonia/artoria_attack.gif";
    atk=3;
    maxHealth=25;
    curHealth=25;
    xOffest=20;
    yOffest=-28;
    towerInterval=25;
    canChangeDirect=true;
    haveChangeDirectButton=true;
    changeDirectButton=new IconButton(":/image/changeDirect.png",1.2,1.2);
    removeButton->setVisible(false);
    removeButton->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    connect(changeDirectButton,&QPushButton::clicked,this,[&](){
        changeDirectButton->setMyIcon(":/image/changeDirect2.png");
        hideAttackRange();
        changeDirect();
        showAttackRange();
    });
}

void Artoria::getTarget()
{
    bool getEnemy=false;
    if(pos==NULL) return;
    else{
        for(auto &ele:targetPlace){
            if(ele->haveGroundEnemy()){
                target=ele->getGroundEnemy();
                getEnemy=true;
                break;
            }
        }
    }
    if(getEnemy){
        Q_ASSERT(target!=NULL);
        connect(target,&Enemy::haveDeath,this,[&](){
            target=NULL;
            atkCount=0;
        });
        connect(target,&Enemy::haveMoveToNextPlace,this,[&](){
            getTarget();
        });
    }
    else{
        target=NULL;
    }
}

void Artoria::doAction()
{
    MeleeTower::doAction();
}

void Artoria::doAttack()
{
    MeleeTower::doAttack();
}

void Artoria::showAttackRange()
{
    for(auto &ele:targetPlace){
        ele->showAttackRange();
    }
}

void Artoria::hideAttackRange()
{
    for(auto &ele:targetPlace){
        ele->hideAttackRange();
    }
}

void Artoria::changeDirect()
{
    stationaryPath = ":/tower/object/artonia/ArtoriaFanzhuan.png";
    atkPath=":/tower/object/artonia/artoria_attackFanzhuan.gif";
    xOffest=0;
    canChangeDirect=false;
    targetPlace.clear();
    targetPlace<<pos;
    if(pos->getNextMapPlace()!=NULL){
        targetPlace<<pos->getNextMapPlace();
    }
    drawTower(pos->xSide(),pos->ySide());
}

void Artoria::addPlace(MapPlace *mp)
{
    Tower::addPlace(mp);
    targetPlace<<mp;
    if(mp->getLastMapPlace()!=NULL){
        targetPlace<<mp->getLastMapPlace();
    }
}


Meltryllis::Meltryllis(int tower_id):bulletCata(5)
{
    towerNumber=tower_id;
    stationaryPath = ":/tower/object/meltryllis/stationary.png";
    atkPath=":/tower/object/meltryllis/attack.gif";
    atk=3;
    maxHealth=20;
    curHealth=20;
    xOffest=-10;
    yOffest=-25;
    canChangeDirect=true;
    haveChangeDirectButton=true;
    changeDirectButton=new IconButton(":/image/changeDirect.png",1.2,1.2);
    removeButton->setVisible(false);
    removeButton->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    connect(changeDirectButton,&QPushButton::clicked,this,[&](){
        changeDirectButton->setMyIcon(":/image/changeDirect2.png");
        hideAttackRange();
        changeDirect();
        showAttackRange();
    });
}

void Meltryllis::getTarget()
{
    if(pos==NULL) return;
    for(auto &ele:targetPlace){
        if(ele->haveEnemy()){
            target=ele->getEnemy();
            connect(target,&Enemy::haveDeath,this,[&](){
                target=NULL;
                atkCount=0;
            });
            connect(target,&Enemy::haveMoveToNextPlace,this,[&](){
                target=NULL;
                getTarget();
            });
            return;
            break;
        }
    }
    target=NULL;
}

void Meltryllis::doAction()
{
    MeleeTower::doAction();
}

void Meltryllis::doAttack()
{
    atkCount++;
    if(atkCount==towerInterval)
    {
        getTarget();
        if(target!=NULL)
        {
            emit shotBullet(QPoint(xPos,yPos),QPoint(target->getCurX(),target->getCurY()),bulletCata);
            if(target->getHealth()<=atk){
                Q_ASSERT(target);
                target->reduceHealth(atk);
                target=NULL;
            }
            else{target->reduceHealth(atk);}
        }


        atkCount=0;
    }
}

void Meltryllis::showAttackRange()
{
    for(auto &ele:targetPlace){
        ele->showAttackRange();
    }
}

void Meltryllis::hideAttackRange()
{
    for(auto &ele:targetPlace){
        ele->hideAttackRange();
    }
}

void Meltryllis::changeDirect()
{
    stationaryPath = ":/tower/object/meltryllis/stationaryFanzhuan.png";
    atkPath=":/tower/object/meltryllis/attackFanzhuan.gif";
    canChangeDirect=false;
    bulletCata=6;
    targetPlace.clear();
    MapPlace *temp=pos;
    for(int i=0;i<=2;i++){
        targetPlace<<temp;
        temp=temp->getNextMapPlace();
        if(temp==NULL) break;
    }
    drawTower(pos->xSide(),pos->ySide());
}

void Meltryllis::addPlace(MapPlace *mp)
{
    Tower::addPlace(mp);
    MapPlace *temp=mp;
    for(int i=0;i<=2;i++){
        targetPlace<<temp;
        temp=temp->getLastMapPlace();
        if(temp==NULL) break;
    }
}

Maxiu::Maxiu(int tower_id)
{
    towerNumber=tower_id;
    stationaryPath = ":/tower/object/maxiu/maxiu.png";
    atkPath=":/tower/object/maxiu/maxiu.png";
    atk=0;
    maxHealth=60;
    curHealth=60;
    xOffest=0;
    yOffest=-25;
}

Avenger::Avenger(int tower_id)
{
    towerNumber=tower_id;
    stationaryPath = ":/tower/object/Avenger/Avenger.png";
    atkPath=":/tower/object/Avenger/Avenger.png";
    atk=1;
    maxHealth=1;
    curHealth=1;
    xOffest=0;
    yOffest=-25;
    towerInterval=50;
    targets=NULL;
}

void Avenger::getTarget()
{
    if(pos==NULL){ targets=NULL;return;}
    targets=pos->getAllEnemy();
    return;
}

void Avenger::doAction()
{
    if(pos!=NULL)
    {
        if(targets!=NULL){
            doAttack();
        }
        else{
            getTarget();
        }
    }
}

void Avenger::doAttack()
{
    atkCount++;
    if(atkCount==towerInterval)
    {
        if(targets!=NULL){
            Q_ASSERT(targets);
            for(auto &ele:(*targets)){
                if(QRandomGenerator::global()->bounded(100)==1){
                    ele->reduceHealth(999);
                }
                else{ele->reduceHealth(atk);}
            }
        }
        atkCount=0;
    }
}

void Avenger::showAttackRange()
{
    pos->showAttackRange();
}

void Avenger::hideAttackRange()
{
    pos->hideAttackRange();
}

bool Avenger::getCanBeAttack()
{
    return false;
}
