#ifndef ENEMYS_H
#define ENEMYS_H
#include <QWidget>
#include <QObject>
#include "enemy/enemy.h"

class Tower;
class PlentyQqr : public GroundEnemy
{
    Q_OBJECT
public:
    explicit PlentyQqr(int enemy_number);
    void doAttack();
signals:

};

class AddBloodQqr: public GroundEnemy
{
    Q_OBJECT
public:
    explicit AddBloodQqr(int enemy_number);
    ~ AddBloodQqr();
    void doAttack();
    void doAction();
    void setParent(QWidget *parent);
    void dealDeath();
signals:
private:
    QVector<QLabel *> doublePluses;
    QLabel *ownPlus;
    QWidget *parent;
};

class ArrowQqr: public GroundEnemy, public RemoteEnemy
{
    Q_OBJECT
public:
    explicit ArrowQqr(int enemy_number);
    void doAttack();
    void targetSelect();
signals:
};

class FloatingElf : public AirEnemy, public RemoteEnemy
{
    Q_OBJECT
public:
    explicit FloatingElf(int enemy_number);
    void doAttack();
    void targetSelect();
signals:
};

class PaiMeng: public AirEnemy, public RemoteEnemy
{
    Q_OBJECT
public:
    explicit PaiMeng(int enemy_number);
    void doAttack();
    void targetSelect();
signals:
};

#endif // ENEMYS_H
