#include "towerthread.h"
#include "game_engine/gameengine.h"
#include "tower/meleetower.h"
#include "tower/tower.h"
#include "tower/towerlabel.h"

TowerThread::TowerThread(GameEngine* g):
    gameEngine(g),deleteCountMax(10),deleteCount(0),deleteTimerDuration(0),isActive(true)
{
    setAutoDelete(true);
}

void TowerThread::run()
{
    connect(gameEngine,&GameEngine::creatTower,[&](int tower_number){
        for(auto &ele:gameEngine->towers){
            if(ele->getTowerNumber()==tower_number){

                connect(ele,&Tower::haveDeath,[&](int tower_number){
                    removeTower(tower_number);
                });
            }
        }
    });
    timer1=new QTimer(this);
    timer1->setInterval(50);
    timer1->start();
    connect(timer1,&QTimer::timeout,this,[&](){
        if(isActive)
        {
            for(auto &ele:gameEngine->towers){
                ele->doAction();
                ele->update();
            }
        }
    });
    timer2=new QTimer(this);
    timer2->setInterval(200);
    connect(timer2,&QTimer::timeout,this,[&](){
        if(deleteCount==deleteCountMax){
            Tower* temp=deleteTowers.dequeue();
            temp->deleteBloodBar();
            delete temp;
            deleteCount=0;
            deleteTimerDuration--;
            qDebug()<<"析构1个防御塔,剩余待析构"<<deleteTowers.size();
        }
        if(deleteTimerDuration>0){
            deleteCount++;
        }

    });
    timer2->start();
    exec();
}

void TowerThread::removeTower(int tower_number)
{
    Tower *temp;
    for(auto it=gameEngine->towers.begin();it < gameEngine->towers.end();it++){

        if((*it)->getTowerNumber()==tower_number){
          temp=(*it);
          gameEngine->towers.erase(it);
          deleteTowers.enqueue(temp);
          deleteTimerDuration++;
          break;
        }
    }

}

void TowerThread::controlTimer()
{
    if(isActive){
        isActive=false;
        for(auto &ele:gameEngine->towers){
            ele->controlMovie();
        }
    }
    else{
        for(auto &ele:gameEngine->towers){
            ele->controlMovie();
        }
        isActive=true;
    }
}

