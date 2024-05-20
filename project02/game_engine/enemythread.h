#ifndef ENEMYTHREAD_H
#define ENEMYTHREAD_H

#include <QObject>
#include <QThread>
#include <QRunnable>
#include <QVector>
#include <QTimer>
#include <QDebug>
#include <QQueue>
#include <QPoint>
#include <QRandomGenerator>
#include <QTime>
#include <QStack>
#include <QPair>
#include <string>

class MapPlace;
class Enemy;
class GameScene;
class GameEngine;
//class EnemyThread : public QObject, public QRunnable
class EnemyThread : public QThread,public QRunnable
{
    Q_OBJECT
public:
    friend class GameEngine;
    explicit EnemyThread(GameEngine* g);
    void run() override;
    void setNextEnemy();
    void creatEnemy();
    void creatPlentyEnemy();
    void creatRemoteEnemy();
    void creatAirEnemy();
    void creatAddHealthEnemy();
    void creatAfraidWarEnemy();
    void removeEnemy(int enemy_id);
    void removeDeathEnemy(int enemy_id);
    void controlTimer();
signals:
    void creatBullet(QPoint curPos,QPoint targetPos,int cata);
    void beginJudgeVictory();
private:
    const int deleteCountMax;
    int enemyNumber;
    int deleteCount;
    int deleteTimerDuration;
    int creatEnemyInterval,creatEnemyCount;
    bool isActive;
    bool isGameOver;
    QString nextEnemys;
    QTimer* timer1, *timer2;
    GameEngine* gameEngine;
    QQueue<Enemy*> deleteEnemys;
};

#endif // ENEMYTHREAD_H
