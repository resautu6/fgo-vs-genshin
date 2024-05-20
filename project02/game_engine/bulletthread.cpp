#include "bulletthread.h"
#include "bullet/bullet.h"
#include "game_engine/gameengine.h"
#include "gamescene.h"
BulletThread::BulletThread(GameEngine *g):
    isActive(true),gameEngine(g)
{
    setAutoDelete(true);
}

void BulletThread::run()
{
    connect(gameEngine,&GameEngine::creatBullet,this,[&](int bullet_number){
        for(auto &ele:gameEngine->bullets){
            if(ele->getBulletNumber()==bullet_number){
                connect(ele,&Bullet::arriveTarget,this,[&](int num){
                    Bullet *temp;
                    for(auto it=gameEngine->bullets.begin();it<gameEngine->bullets.end();it++){
                        if((*it)->getBulletNumber()==num){
                            temp=(*it);
                            gameEngine->bullets.erase(it);
                            delete temp;
                        }
                    }
                });
            }
        }
    });

    timer=new QTimer(this);
    timer->setInterval(30);
    connect(timer,&QTimer::timeout,this,[=](){
        if(isActive)
        {
            for(auto &ele:gameEngine->bullets){
                ele->bulletMove();
            }
        }
    });
    timer->start();
    exec();
}

void BulletThread::drawBullet(QPainter *painter)
{
    for(auto &ele:gameEngine->bullets){
        ele->drawBullet(painter);
    }
    gameEngine->gameScene->update();
}

void BulletThread::controlTimer()
{
    if(isActive){
        isActive=false;
    }
    else{
        isActive=true;
    }
}
