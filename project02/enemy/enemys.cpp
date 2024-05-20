#include "enemys.h"
#include "tower/tower.h"
#include "place-line/mapplace.h"
PlentyQqr::PlentyQqr(int enemy_number)
{
    enemyNumber=enemy_number;
    //stationaryPath=":/enemys/object/plenty_qqr/plentyqqr(test).png";
    AttackPath=":/enemys/object/plenty_qqr/plentqqr_attack.gif";
    movePath=":/enemys/object/plenty_qqr/plentyqqr.gif";
    atk=2;
    maxHealth=15;
    curHealth=15;
    speed=1;
    yOffset=-10;
}

void PlentyQqr::doAttack()
{
    atkCount++;
    if(atkCount==enemyInterval)
    {
        targetSelect();
        if(pos->haveTower() && target!=NULL){
            if(target->getHealth()<=atk){
                pos->getTower()->ReduceHealth(atk);
                target=NULL;
            }
            else{
                pos->getTower()->ReduceHealth(atk);
            }

        }

        atkCount=0;
    }
}

FloatingElf::FloatingElf(int enemy_number)
{
    enemyNumber=enemy_number;
    AttackPath=":/enemys/object/floating/floating.gif";
    movePath=":/enemys/object/floating/floating.gif";
    atk=3;
    maxHealth=10;
    curHealth=10;
    speed=2;
    yOffset=-20;
    enemyInterval=50;
    canFly=true;
    val=5;
    valPath=":/image/stone+5.png";
}

void FloatingElf::doAttack()
{
    atkCount++;
    if(atkCount==enemyInterval-6){
        targetSelect();
        emit shotBullet(QPoint(curX,curY),QPoint(target->getXPos(),target->getYPos()),2);
    }
    if(atkCount==enemyInterval)
    {
        targetSelect();
        if(target!=NULL){
            if(target->getHealth()<=atk){
                target->ReduceHealth(atk);
                target=NULL;
            }
            else{
                target->ReduceHealth(atk);
            }

        }

        atkCount=0;
    }
}

void FloatingElf::targetSelect()
{
    RemoteEnemy::targetSelect();
}

ArrowQqr::ArrowQqr(int enemy_number)
{
    enemyNumber=enemy_number;
    AttackPath=":/enemys/object/arrow__qqr/arrow_attack.gif";
    movePath=":/enemys/object/arrow__qqr/arrow_move.gif";
    atk=4;
    maxHealth=10;
    curHealth=10;
    speed=1;
    yOffset=-20;
    enemyInterval=48;
    atkCount=45;

}

void ArrowQqr::doAttack()
{
    atkCount++;
    if(atkCount==enemyInterval-3){
        targetSelect();
        emit shotBullet(QPoint(curX,curY),QPoint(target->getXPos(),target->getYPos()),1);
    }
    if(atkCount==enemyInterval)
    {
        targetSelect();
        if(target!=NULL){

            if(target->getHealth()<=atk){
                target->ReduceHealth(atk);
                target=NULL;
            }
            else{
                target->ReduceHealth(atk);
            }

        }

        atkCount=0;
    }
}

void ArrowQqr::targetSelect()
{
    RemoteEnemy::targetSelect();
}

PaiMeng::PaiMeng(int enemy_number)
{
    enemyNumber=enemy_number;
    AttackPath=":/enemys/object/paimeng/pm.gif";
    movePath=":/enemys/object/paimeng/pm.gif";
    atk=2;
    maxHealth=10;
    curHealth=10;
    speed=2;
    yOffset=-20;
    enemyInterval=50;
    canFly=true;
    val=5;
    valPath=":/image/stone+5.png";
}

void PaiMeng::doAttack()
{
    atkCount++;
    if(atkCount==enemyInterval-6){
        targetSelect();
        emit shotBullet(QPoint(curX,curY),QPoint(target->getXPos(),target->getYPos()),2);
    }
    if(atkCount==enemyInterval)
    {
        targetSelect();
        if(target!=NULL){
            if(target->getHealth()<=atk){
                target->ReduceHealth(atk);
                target=NULL;
            }
            else{
                target->ReduceHealth(atk);
            }

        }

        atkCount=0;
    }
}

void PaiMeng::targetSelect()
{
    if(pos!=NULL)
    {
        for(auto &ele : roundPlace){
            if(ele->haveTower()){
                canDamage=true;
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


AddBloodQqr::AddBloodQqr(int enemy_number)
{
    enemyNumber=enemy_number;
    AttackPath=":/enemys/object/addBloodQqr/addBlood.gif";
    movePath=":/enemys/object/addBloodQqr/addBlood.gif";
    atk=2;
    maxHealth=25;
    curHealth=25;
    speed=1;
    yOffset=-20;
    enemyInterval=80;
    canFly=false;
    val=5;
    valPath=":/image/stone+5.png";
    ownPlus=new QLabel;
    QPixmap pix(":/image/circlePlus.png");
    ownPlus->setPixmap(pix);
    ownPlus->setAttribute(Qt::WA_TransparentForMouseEvents,true);
}

AddBloodQqr::~AddBloodQqr()
{
    delete ownPlus;
}

void AddBloodQqr::doAttack()
{
    atkCount++;
    if(atkCount==10){
        ownPlus->setVisible(false);
    }
    else if(atkCount==70){
        ownPlus->setGeometry(curX+25,curY-40,20,20);
        ownPlus->setVisible(true);
    }
    if(atkCount==enemyInterval){
        if(pos!=NULL && pos->haveEnemy())
        {
            QVector<Enemy*>*targets=pos->getAllEnemy();
            for(auto &ele: (*targets)){
                QPixmap pix(":/image/doublePlus.png");
                QLabel *doublePlus=new QLabel(parent);
                doublePlus->setPixmap(pix);
                doublePlus->setGeometry(ele->getCurX()+25,ele->getCurY()-20,20,12);
                doublePlus->setVisible(true);
                doublePlus->setAttribute(Qt::WA_TransparentForMouseEvents,true);
                doublePluses<<doublePlus;
                ele->addHealth(atk);
            }
            QTimer::singleShot(800,this,[&](){
                for(auto &ele:doublePluses){
                    QLabel*tmp=ele;
                    delete tmp;
                }
                doublePluses.clear();
            });
        }
        atkCount=0;
    }
}

void AddBloodQqr::doAction()
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
            if(pos->haveTower()){
                canMove=false;
            }
            else{
                canMove=true;
            }
            if(canMove){doMove();ownPlus->move(curX+25,curY-40);}
             doAttack();
        }
    }
}

void AddBloodQqr::setParent(QWidget *parent)
{
    GroundEnemy::setParent(parent);
    this->parent=parent;
    ownPlus->setParent(parent);
    ownPlus->setVisible(false);
}

