#include "gameengine.h"
#include "gamescene.h"
#include <tower/tower.h>
#include <tower/towerlabel.h>
#include "tower/meleetower.h"
#include <enemy/enemy.h>
#include <place-line/mapplace.h>
#include <game_engine/enemythread.h>
#include <game_engine/towerthread.h>
#include "enemy/enemys.h"
#include <game_engine/bulletthread.h>
#include <bullet/bullet.h>

GameEngine::GameEngine(QVector<QVector<MapPlace*>>* _map,QVector<TowerLabel*>* tower_labels,\
                       QVector<MapPlace*>* gbp, QVector<MapPlace*>* fbp,QStack<QPair<int,QString>*>* enemyList,\
                       int stone_num, GameScene *parent)
    :places(_map),towerLabels(tower_labels),groundBeginPlace(gbp),flyBeginPlace(fbp),towerNumber(1),enemyNumber(1),\
    bulletNumber(1),baseBlood(10),stoneNumber(stone_num),budget(0),towerClicked(false),bulletThread(NULL),\
    enemyCreateList(enemyList),gamePause(false)
{
    setParent(parent);
    gameScene=parent;
    for(auto &ele:*towerLabels){
        connect(ele,&TowerLabel::sendTowerLabel,gameScene,&GameScene::setChoseTower);
        connect(ele,&TowerLabel::sendTowerLabel,gameScene,[=](){
            budget=ele->getCost();
            towerCanNotClick();
        });
        connect(ele,&TowerLabel::cancelChoseTower,gameScene,&GameScene::cancelChoseTower);
        connect(ele,&TowerLabel::cancelChoseTower,gameScene,[=](){
            budget=0;
            towerCanClick();
        });
    }
    for(auto &ele1:(*places)){
        for(auto &ele2 : ele1){
            if(ele2->getCata()==1 || ele2->getCata()==2 || ele2->getCata()==3)
            {
                connect(ele2,&MapPlace::hasPutTower,[&](MapPlace* place,int tower_label_cata){
                    //qDebug()<<towerNumber;
                    changeStoneNumber(-budget);
                    Tower* temp=TowerLabel::makeTower(towerNumber++,tower_label_cata);
                    temp->setParent(gameScene);
                    if(tower_label_cata==2 || tower_label_cata==4 || tower_label_cata==7){
                        temp->setMapPlace(places);
                    }
                    temp->addPlace(place);
                    place->addTower(temp);
                    connect(temp,&Tower::shotBullet,this,[&](QPoint curPos,QPoint targetPos,int cata){
                        Bullet *bul=new Bullet(curPos,targetPos,cata,bulletNumber++,gameScene);
                        bullets<<bul;
                        emit creatBullet(bul->getBulletNumber());
                    });
                    connect(temp,&Tower::hasBeenClick,this,[=](int tower_number){
                        towerClicked=true;
                        pauseGame();
                        gameScene->setTowerClicking(true);
                        towerCanNotClick(tower_number);
                        towerLabelCanClick();
                    });
                    connect(temp,&Tower::hasCancelClick,this,[=](){
                        towerClicked=false;
                        continueGame();
                        gameScene->setTowerClicking(false);
                        towerCanClick();
                        towerLabelCanClick();
                    });
                    towers<<temp;
                    emit creatTower(temp->getTowerNumber());
                    gameScene->cancelChoseTower();
                    towerCanClick();
                    setTowerLabelNoChosen();
                    budget=0;
                });
            }
        }
    }

//    timer1=new QTimer(this);
//    timer1->setInterval(1000);
//    connect(timer1,&QTimer::timeout,this,[&](){
//        this->creatEnemy();
//        //timer1->stop();
//    });
//    timer1->start();


//    timer2=new QTimer(this);
//    timer2->setInterval(5);
//    connect(timer2,&QTimer::timeout,this,[&](){
//        for(auto &ele : enemys){
//            if(ele->getCanDoAction())
//            {
//                ele->doAction();
//                ele->update();
//            }
//        }
//    });
//    timer2->start();

    addStone=new QTimer(this);
    addStone->setInterval(3000);
    connect(addStone,&QTimer::timeout,this,[&](){
        if(!gamePause){
            changeStoneNumber(2);
        }
    });
    addStone->start();

    QThreadPool::globalInstance()->setMaxThreadCount(10);
    enemyThread=new EnemyThread(this);
    connect(enemyThread,&EnemyThread::creatBullet,this,[&](QPoint curPos,QPoint targetPos,int cata){
        Bullet *temp=new Bullet(curPos,targetPos,cata,bulletNumber++,gameScene);
        bullets<<temp;
        emit creatBullet(temp->getBulletNumber());
    });
    connect(enemyThread,&EnemyThread::beginJudgeVictory,this,[&](){
        judgeVictory=new QTimer(this);
        judgeVictory->setInterval(1500);
        judgeVictory->start();
        connect(judgeVictory,&QTimer::timeout,this,[&](){
            if(enemys.empty() && enemyCreateList->empty()){
                qDebug()<<"胜利";
                showVectory();
                judgeVictory->stop();
            }
        });

    });
    QThreadPool::globalInstance()->start(enemyThread);

    towerThread=new TowerThread(this);
    QThreadPool::globalInstance()->start(towerThread);

    bulletThread=new BulletThread(this);
    QThreadPool::globalInstance()->start(bulletThread);

    connect(this,&GameEngine::destroyed,[&](){
        enemyThread->quit();
        enemyThread->wait();
        enemyThread->deleteLater();
    });
    connect(this,&GameEngine::destroyed,[&](){
        towerThread->quit();
        towerThread->wait();
        towerThread->deleteLater();
    });
    connect(this,&GameEngine::destroyed,[&](){
        bulletThread->quit();
        bulletThread->wait();
        bulletThread->deleteLater();
    });
}


void GameEngine::setTowerLabelNoChosen()
{
    for(auto &ele:(*towerLabels)){
        ele->setNoIsChosen();
    }
}


void GameEngine::showVectory(){
    addStone->stop();
    delete enemyThread;
    delete towerThread;
    victoryLabel=new QLabel(gameScene);
    QPixmap pix(":/image/victory.png");
    victoryLabel->setScaledContents(true);
    victoryLabel->setGeometry(170,20,pix.width(),pix.height());
    victoryLabel->setPixmap(pix);
    victoryLabel->setVisible(true);
    victoryLabel->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    gameScene->playGameOverMusic(":/music_player/music/victory.wav");
    gameScene->gameOver();
}

void GameEngine::showDefeat()
{
    addStone->stop();
    delete enemyThread;
    delete towerThread;
    defeatLabel=new QLabel(gameScene);
    QPixmap pix(":/image/defeat.png");
    defeatLabel->setScaledContents(true);
    defeatLabel->setGeometry(190,20,pix.width(),pix.height());
    defeatLabel->setPixmap(pix);
    defeatLabel->setVisible(true);
    defeatLabel->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    gameScene->playGameOverMusic(":/music_player/music/failure.wav");
    gameScene->gameOver();

}

void GameEngine::creatEnemy()
{
    Enemy *temp=new FloatingElf(enemyNumber++);
    temp->setPlace((*(groundBeginPlace))[1]);
    temp->setTargetPlace();
    temp->setParent(gameScene);
    temp->drawEnemy();



    enemys.push_back(temp);
}

void GameEngine::damageBase()
{
    baseBlood-=1;
    gameScene->setBaseBlood(baseBlood);
    if(baseBlood<=0){
        showDefeat();
    }
    qDebug()<<"对基地造成伤害1点，目前基地剩余血量"<<baseBlood;
}

void GameEngine::pauseGame()
{
    gamePause=true;
    bulletThread->controlTimer();
    enemyThread->controlTimer();
    towerThread->controlTimer();
}

void GameEngine::continueGame()
{
    gamePause=false;
    bulletThread->controlTimer();
    enemyThread->controlTimer();
    towerThread->controlTimer();
}

void GameEngine::drawScene(QPainter *painter) const
{
    if(bulletThread){
        bulletThread->drawBullet(painter);
    }
}

void GameEngine::towerCanNotClick(int tower_number)
{
    for(auto &ele:towers){
        if(ele->getTowerNumber()!=tower_number){
            ele->setCanClicked(false);
            ele->setAttribute(Qt::WA_TransparentForMouseEvents,true);
        }
    }
}

void GameEngine::towerCanClick()
{
    for(auto &ele:towers){
        ele->setCanClicked(true);
        ele->setAttribute(Qt::WA_TransparentForMouseEvents,false);
    }
}

void GameEngine::towerLabelCanClick()
{
    if(towerClicked){
        for(auto &ele:(*towerLabels)){
            ele->setNoIsChosen();
            ele->setAttribute(Qt::WA_TransparentForMouseEvents,true);
        }
    }
    else{
        for(auto &ele:(*towerLabels)){
            ele->setAttribute(Qt::WA_TransparentForMouseEvents,false);
        }
    }
}

void GameEngine::changeStoneNumber(int num)
{
    QMutex mutex;
    mutex.lock();
    stoneNumber+=num;
    gameScene->setStoneNumber(stoneNumber);
    mutex.unlock();
}
