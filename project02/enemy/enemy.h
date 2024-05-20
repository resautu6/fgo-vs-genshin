#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QLabel>
#include <QString>
#include <QVector2D>
#include <QTimer>
#include <QDebug>
#include <QMovie>
#include <QPainter>
#include <QPixmap>
#include <math.h>

class MapPlace;
class Tower;
class Enemy : public QLabel
{
    Q_OBJECT
public:
    explicit Enemy();
    virtual void reduceHealth(int h);
    virtual void doAction();
    virtual void doAttack();
    virtual void drawEnemy();
    virtual void doMove();
    virtual void targetSelect();
    virtual void setParent(QWidget *parent);
    void addHealth(int x);
    void update();
    void controlMovie();
    void setDeath();
    void setTargetPlace();
    void setPlace(MapPlace* p);
    void setPlaces(QVector<QVector<MapPlace*>> *pls);
    void setMoveMovie();
    void setAttackMovie();
    void drawBloodLine();
    int getEnemyNumber() const{return enemyNumber;};
    int getHealth() const;
    int getCurX() const;
    int getCurY() const;
    bool getCanDoAction() const;
    bool getCanFly() const;
    MapPlace* getPlacePos() const;
signals:
    void damage(int enemy_number);
    void haveDeath(int enemy_number,int val);
    void haveMoveToNextPlace(int enemy_number);
    void shotBullet(QPoint curPos, QPoint targetPos, int cata);
protected:
    int enemyNumber;
    int curHealth,maxHealth;
    int atk;
    int speed;
    int atkCount,enemyInterval;
    int flashCount;
    float curX,curY,targetX,targetY,moveX,moveY;
    int xOffset,yOffset;
    int healthBarLength;
    int val;
    bool canMove,canDamage,canDoAction,canVisible,hasChangeMovie,hasPauseMovie,canFly;
    QPainter* painter;
    QLabel *healthBarBack, *healthBar;
    QTimer *flashTimer;
    QString movePath;
    QString AttackPath;
    QString valPath;
    MapPlace* pos;
    Tower* target;
    QVector<QVector<MapPlace*>> *places;
    QMovie* enemyMovie;
};

class RemoteEnemy:virtual public Enemy
{
    Q_OBJECT
public:
    RemoteEnemy();
    virtual void targetSelect();
    virtual void doMove();
    void setRoundPlace();
protected:
    int rPos,cPos;
    QVector<MapPlace*> roundPlace;
};

class GroundEnemy:virtual public Enemy
{
    Q_OBJECT
public:
    GroundEnemy();
    virtual void targetSelect();
protected:
    QString atkPath1,atkPath2,atkPath3;
};

class AirEnemy: virtual public Enemy
{
    Q_OBJECT
public:
    AirEnemy();
    virtual void targetSelect();
protected:
    bool _stop;
};

#endif // ENEMY_H
