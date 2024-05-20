#ifndef REMOTETOWER_H
#define REMOTETOWER_H

#include <QObject>
#include "tower/tower.h"
#include <QPair>
#include <QVector>
#include <QRandomGenerator>
#include <QPropertyAnimation>
#include <QWidget>
#include <QTimer>


class MapPlace;
class RemoteTower : public Tower
{
    Q_OBJECT
public:

    explicit RemoteTower();
    virtual void getTarget();
    virtual void doAction();
    virtual void doAttack();
    virtual void showAttackRange();
    virtual void hideAttackRange();
    void addPlace(MapPlace *mp);
    void setRoundPlace(MapPlace *mp);
signals:
protected:
    QString atkPath1,atkPath2,atkPath3;
    int rPos,cPos;
    QVector<MapPlace*> roundPlace;
    QVector<QLabel*> doublePluses;
};

class Gilgamesh: public RemoteTower
{
    Q_OBJECT
public:
    explicit Gilgamesh(int tower_id);
    void doAction();
    void doAttack();
    void getTarget();

};

class Emiya:public RemoteTower
{
    Q_OBJECT
public:
    explicit Emiya(int tower_id);
    void doAction();
    void doAttack();
};

class Fufu:public RemoteTower
{
    Q_OBJECT
public:
    explicit Fufu();
    void doAttack();
    void drawBloodLine();
    void appear(MapPlace *mp);
    void drawTower(int x, int y);
    void setActionPlus(bool ctr);
    void setParent(QWidget *parent);
    bool getCanBeAttack();
    void dealDeath();
private:
    QString actionFlag,improveFlag;
    QWidget *parent;
    QLabel *ownPlus;
    bool beginAction;
};

class Merlin:public RemoteTower
{
    Q_OBJECT
public:
    explicit Merlin(int tower_id);
    ~Merlin();
    void doAction();
    void setParent(QWidget *parent);
    void showAttackRange();
    void hideAttackRange();
    void addPlace(MapPlace *mp);
    void setMapPlace(QVector<QVector<MapPlace*>>* pls);
    void setDeath();
private:
    Fufu *fufu;
};


#endif // REMOTETOWER_H
