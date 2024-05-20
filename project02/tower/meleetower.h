#ifndef MELEETOWER_H
#define MELEETOWER_H
#include "tower/tower.h"

#include <QString>
#include <QObject>
#include <QRandomGenerator>
class Enemy;
class MeleeTower : public Tower
{
    Q_OBJECT
public:

    explicit MeleeTower();
    virtual void getTarget();
    virtual void doAction();
    virtual void doAttack();
    virtual void showAttackRange();
    virtual void hideAttackRange();
signals:
protected:
    QString atkPath1,atkPath2,atkPath3;

};

class Artoria: public MeleeTower
{
    Q_OBJECT
public:
    explicit Artoria(int tower_id);
    void getTarget();
    void doAction();
    void doAttack();
    void showAttackRange();
    void hideAttackRange();
    void changeDirect();
    void addPlace(MapPlace* mp);
private:
    QVector<MapPlace*> targetPlace;
};

class Meltryllis: public MeleeTower
{
    Q_OBJECT
public:
    explicit Meltryllis(int tower_id);
    void getTarget();
    void doAction();
    void doAttack();
    void showAttackRange();
    void hideAttackRange();
    void changeDirect();
    void addPlace(MapPlace* mp);
private:
    QVector<MapPlace*> targetPlace;
    int bulletCata;
};

class Maxiu: public Tower
{
    Q_OBJECT
public:
    explicit Maxiu(int tower_id);
    void getTarget(){return;};
    void doAction(){return;};
    void doAttack(){return;};
};

class Avenger:public MeleeTower
{
    Q_OBJECT
public:
    explicit Avenger(int tower_id);
    void getTarget();
    void doAction();
    void doAttack();
    void showAttackRange();
    void hideAttackRange();
    bool getCanBeAttack();
private:
    QVector<Enemy*> *targets;
};

#endif // MELEETOWER_H
