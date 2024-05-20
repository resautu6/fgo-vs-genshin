#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>
#include <QVector>
#include <QThreadPool>
#include <QTimer>
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QMutex>
class GameScene;
class Tower;
class Enemy;
class Bullet;
class TowerLabel;
class MapPlace;
class EnemyThread;
class TowerThread;
class BulletThread;
class GameEngine : public QObject
{
    Q_OBJECT
public:
    friend class EnemyThread;
    friend class TowerThread;
    friend class BulletThread;
    explicit GameEngine(QVector<QVector<MapPlace*>>* _map,QVector<TowerLabel*>* tower_labels,\
    QVector<MapPlace*>* gbp, QVector<MapPlace*>* fbp,QStack<QPair<int,QString>*>* enemyList,int stone_num=0, GameScene *parent = nullptr);
    void setTowerLabelNoChosen();
    void showVectory();
    void showDefeat();
    void creatEnemy();
    void damageBase();
    void pauseGame();
    void continueGame();
    void drawScene(QPainter *painter) const;
    void towerCanNotClick(int tower_number=-1);
    void towerCanClick();
    void towerLabelCanClick();
    void changeStoneNumber(int num);
signals:
    void creatTower(int tower_number);
    void creatBullet(int bullet_number);
private:
    QVector<QVector<MapPlace*>>* places; //map grids
    QVector<Enemy*> enemys;     //enemy list
    QVector<Tower*> towers;     //tower list
    QVector<Bullet*> bullets;   //bullet list
    QVector<TowerLabel*>* towerLabels;      //chose-tower label list
    GameScene* gameScene;       //game main scene
    QVector<MapPlace*>* groundBeginPlace,* flyBeginPlace;     //begin of the road list
    QTimer *timer1, *timer2,*judgeVictory,*addStone;    //timers
    QLabel *defeatLabel,*victoryLabel;  //defeat and victory labels
    EnemyThread* enemyThread;       //enemythread
    TowerThread* towerThread;       //towerthread
    BulletThread* bulletThread;    //bulletThread;
    QPainter * painter;      //painter
    QStack<QPair<int,QString>*> *enemyCreateList;
    int towerNumber;        //the number of towers has been set
    int enemyNumber;        //the number of enemys has been produce
    int bulletNumber;       //the number of bullets has been shot
    int baseBlood;         //base blood
    int stoneNumber;       //money number
    int budget;            //cost of a tower
    bool towerClicked;     //if there is tower been clicked
    bool gamePause;        //if game has been paused
};

#endif // GAMEENGINE_H
