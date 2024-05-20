#include "enemythread.h"
#include "enemy/enemy.h"
#include "enemy/enemys.h"
#include "gamescene.h"
#include "game_engine/gameengine.h"

EnemyThread::EnemyThread(GameEngine* g):
    deleteCountMax(10),enemyNumber(1),gameEngine(g),deleteCount(0),deleteTimerDuration(0),\
    creatEnemyInterval(200),creatEnemyCount(0),isActive(true),isGameOver(false)
{
    setAutoDelete(true);
}

void EnemyThread::run()
{
    setNextEnemy();
    timer1=new QTimer(this);
    timer1->setInterval(50);
    connect(timer1,&QTimer::timeout,this,[&](){
        if(isActive)
        {
            for(auto &ele : gameEngine->enemys){
                if(ele->getCanDoAction())
                {
                    ele->update();
                    ele->doAction();

                }
            }
            creatEnemyCount++;
            if(creatEnemyCount==creatEnemyInterval && !isGameOver){
                creatEnemy();
                setNextEnemy();
                creatEnemyCount=0;
            }
        }
    });
    timer1->start();


    timer2=new QTimer(this);
    timer2->setInterval(100);
    connect(timer2,&QTimer::timeout,this,[&](){
        if(deleteCount==deleteCountMax){
            delete deleteEnemys.dequeue();
            deleteCount=0;
            deleteTimerDuration--;
            qDebug()<<"析构1个敌人";
        }
        if(deleteTimerDuration>0){
            deleteCount++;
        }

    });
    timer2->start();

    exec();
}

void EnemyThread::setNextEnemy()
{
    QPair<int,QString> *temp=gameEngine->enemyCreateList->pop();
    creatEnemyInterval=temp->first;
    nextEnemys=temp->second;
    delete temp;
    if(gameEngine->enemyCreateList->empty()){
        emit beginJudgeVictory();
        isGameOver=true;
    }
}

void EnemyThread::creatEnemy()
{
    std::string temp=nextEnemys.toStdString();
    for(int i=0;i<nextEnemys.length();i+=2){
        for(int j=0;j<temp[i+1]-'0';j++){
            switch(temp[i]-'0'){
            case 1:creatPlentyEnemy();break;
            case 2:creatRemoteEnemy();break;
            case 3:creatAddHealthEnemy();break;
            case 4:creatAirEnemy();break;
            case 5:creatAfraidWarEnemy();break;
            default:qDebug()<<"error";break;
            }
        }
    }
}
void EnemyThread::creatPlentyEnemy()
{
    int intTemp=QRandomGenerator::global()->bounded(gameEngine->groundBeginPlace->size());
    Enemy *temp=new PlentyQqr(enemyNumber++);
    temp->setPlaces(gameEngine->places);
    temp->setPlace((*(gameEngine->groundBeginPlace))[intTemp]);
    temp->setTargetPlace();
    temp->setParent(gameEngine->gameScene);
    temp->drawEnemy();

    connect(temp,&Enemy::haveDeath,this,[&](int enemy_id,int val){
        gameEngine->changeStoneNumber(val);
        removeEnemy(enemy_id);
        //qDebug()<<"移除成功";
    });

    connect(temp,&Enemy::damage,this,[&](int enemy_id){
        gameEngine->damageBase();
        removeEnemy(enemy_id);
    });

    connect(temp,&Enemy::shotBullet,this,[=](QPoint cur,QPoint target,int cata){
        emit creatBullet(cur,target,cata);
    });

    gameEngine->enemys.emplace_back(temp);
}

void EnemyThread::creatRemoteEnemy()
{
    int intTemp=QRandomGenerator::global()->bounded(gameEngine->groundBeginPlace->size());
    Enemy *temp=new ArrowQqr(enemyNumber++);
    temp->setPlaces(gameEngine->places);
    temp->setPlace((*(gameEngine->groundBeginPlace))[intTemp]);
    temp->setTargetPlace();
    temp->setParent(gameEngine->gameScene);
    temp->drawEnemy();

    connect(temp,&Enemy::haveDeath,this,[&](int enemy_id,int val){
        gameEngine->changeStoneNumber(val);
        removeEnemy(enemy_id);
        //qDebug()<<"移除成功";
    });

    connect(temp,&Enemy::damage,this,[&](int enemy_id){
        gameEngine->damageBase();
        removeEnemy(enemy_id);
    });

    connect(temp,&Enemy::shotBullet,this,[=](QPoint cur,QPoint target,int cata){
        emit creatBullet(cur,target,cata);
    });

    gameEngine->enemys.emplace_back(temp);
}

void EnemyThread::creatAirEnemy()
{
    int intTemp=QRandomGenerator::global()->bounded(gameEngine->flyBeginPlace->size());
    Enemy *temp=new FloatingElf(enemyNumber++);
    temp->setPlaces(gameEngine->places);
    temp->setPlace((*(gameEngine->flyBeginPlace))[intTemp]);
    temp->setTargetPlace();
    temp->setParent(gameEngine->gameScene);
    temp->drawEnemy();

    connect(temp,&Enemy::haveDeath,this,[&](int enemy_id,int val){
        gameEngine->changeStoneNumber(val);
        removeEnemy(enemy_id);
    });

    connect(temp,&Enemy::damage,this,[&](int enemy_id){
        gameEngine->damageBase();
        removeEnemy(enemy_id);
    });
    connect(temp,&Enemy::shotBullet,this,[=](QPoint cur,QPoint target,int cata){
        emit creatBullet(cur,target,cata);
    });

    gameEngine->enemys.push_back(temp);
}

void EnemyThread::creatAddHealthEnemy()
{
    int intTemp=QRandomGenerator::global()->bounded(gameEngine->groundBeginPlace->size());
    Enemy *temp=new AddBloodQqr(enemyNumber++);
    temp->setPlaces(gameEngine->places);
    temp->setPlace((*(gameEngine->groundBeginPlace))[intTemp]);
    temp->setTargetPlace();
    temp->setParent(gameEngine->gameScene);
    temp->drawEnemy();

    connect(temp,&Enemy::haveDeath,this,[&](int enemy_id,int val){
        gameEngine->changeStoneNumber(val);
        removeEnemy(enemy_id);
    });

    connect(temp,&Enemy::damage,this,[&](int enemy_id){
        gameEngine->damageBase();
        removeEnemy(enemy_id);
    });

    connect(temp,&Enemy::shotBullet,this,[=](QPoint cur,QPoint target,int cata){
        emit creatBullet(cur,target,cata);
    });

    gameEngine->enemys.emplace_back(temp);
}

void EnemyThread::creatAfraidWarEnemy()
{
    int intTemp=QRandomGenerator::global()->bounded(gameEngine->flyBeginPlace->size());
    //Enemy *temp=new FloatingElf(enemyNumber++);
    Enemy *temp=new PaiMeng(enemyNumber++);
    temp->setPlaces(gameEngine->places);
    temp->setPlace((*(gameEngine->flyBeginPlace))[intTemp]);
    temp->setTargetPlace();
    temp->setParent(gameEngine->gameScene);
    temp->drawEnemy();

    connect(temp,&Enemy::haveDeath,this,[&](int enemy_id,int val){
        gameEngine->changeStoneNumber(val);
        removeEnemy(enemy_id);
    });

    connect(temp,&Enemy::damage,this,[&](int enemy_id){
        gameEngine->damageBase();
        removeEnemy(enemy_id);
    });
    connect(temp,&Enemy::shotBullet,this,[=](QPoint cur,QPoint target,int cata){
        emit creatBullet(cur,target,cata);
    });

    gameEngine->enemys.push_back(temp);
}

void EnemyThread::removeEnemy(int enemy_id)
{
    Enemy *temp;
    for(auto it=gameEngine->enemys.begin();it<gameEngine->enemys.end();it++){
        if((*it)->getEnemyNumber()==enemy_id){
            temp=*it;
            gameEngine->enemys.erase(it);
            deleteEnemys.enqueue(temp);
            deleteTimerDuration++;
            Q_ASSERT(temp);
            break;
        }
    }
}

void EnemyThread::controlTimer()
{
    if(isActive){
        isActive=false;
        for(auto &ele:gameEngine->enemys){
            ele->controlMovie();
        }
    }
    else{
        for(auto &ele:gameEngine->enemys){
            ele->controlMovie();
        }
        isActive=true;
    }
}



