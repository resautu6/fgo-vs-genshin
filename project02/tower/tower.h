#ifndef TOWER_H
#define TOWER_H

#include <QObject>
#include <QLabel>
#include <QString>
#include <QVector>
#include <QMovie>
#include <QMutex>
#include <math.h>

#include "iconbutton.h"
class Enemy;
class MapPlace;
class Tower : public QLabel
{
    Q_OBJECT
public:
    explicit Tower();
    virtual void ReduceHealth(int h);
    virtual void doAction()=0;
    virtual void doAttack()=0;
    virtual void getTarget()=0;
    virtual void addPlace(MapPlace *mp);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void deleteClickLabel();
    virtual void setParent(QWidget *parent);
    virtual void showAttackRange(){};
    virtual void hideAttackRange(){};
    virtual bool getCanBeAttack(){return true;};
    virtual void drawBloodLine();
    virtual void drawTower(int x,int y);
    virtual void setMapPlace(QVector<QVector<MapPlace*>>* pls);
    virtual void setDeath();
    void addHealth(int x);
    void Remove();
    void controlMovie();
    void setAttackMovie();
    void setStationaryLabel();
    void setCanClicked(bool b);
    void update();
    void deleteBloodBar();
    int getTowerNumber() const;
    int getHealth() const;
    int getXPos() const;
    int getYPos() const;
    MapPlace* getPlacePos() const;
signals:
    void haveDeath(int tower_number);
    void shotBullet(QPoint curPos, QPoint targetPos, int cata);
    void hasBeenClick(int tower_number);
    void hasCancelClick(int tower_number);
protected:
    int towerNumber;
    int curHealth,maxHealth;
    int atk;
    int xPos=0,yPos=0,xOffest=0,yOffest=0;
    int atkCount,towerInterval;
    int healthBarLength;
    bool hasSetMovie,hasPauseMovie;
    bool hasClicked,canClicked;
    bool canChangeDirect,haveChangeDirectButton;
    QString stationaryPath;
    QString atkPath;
    QString treasureLabelPath;
    MapPlace* pos;
    Enemy* target;
    QVector<QVector<MapPlace*>>* places;
    QLabel *healthBarBack, *healthBar;
    IconButton *removeButton, *treasureButton, *changeDirectButton;
    QMovie *towerMovie;
};

#endif // TOWER_H
