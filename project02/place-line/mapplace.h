#ifndef MAPPLACE_H
#define MAPPLACE_H

#include <QObject>
#include <QVector>
#include <QLabel>
#include <QString>
#include <QDebug>
#include <QWidget>
#include <QPixmap>
class Tower;
class Enemy;
class MapPlace : public QLabel
{
    Q_OBJECT
public:
    explicit MapPlace()=default;
    explicit MapPlace(int x1,int y1,int place_cata,int r, int c);
    int xSide() const;
    int ySide() const;
    int getCata() const;
    int getRPos() const;
    int getCPos() const;
    virtual bool haveGroundEnemy() const{return false;};
    virtual bool haveFlyEnemy() const{return false;};
    virtual bool haveTower() const;
    virtual bool haveEnemy() const{return false;};
    virtual void addEnemy(Enemy* e){};
    virtual void removeEnemy(int enemy_id){};
    virtual Enemy* getEnemy() const {return NULL;};
    virtual Enemy* getFlyEnemy() const {return NULL;};
    virtual Enemy* getGroundEnemy() const{return NULL;};
    virtual QVector<Enemy*>* getAllEnemy(){return NULL;};
    void setNextPlace(MapPlace* mp);
    void setLastPlace(MapPlace* mp);
    void setMapPiture(QString pix_path);
    void setNeedShow(bool b);
    void addTower(Tower* t);
    void showAttackRange();
    void hideAttackRange();
    void removeTower();
    void showCata();
    void hideCata();
    void mouseReleaseEvent(QMouseEvent *event);
    void drawMapPlace(QPainter *painter) const;
    Tower* getTower();
    void setTowerLabel(QString label_path);
    void setTowerLabelCata(int cata);
    void setParentScene(QWidget *parents);
    MapPlace* getNextMapPlace();
    MapPlace* getLastMapPlace();
signals:
    void hasPutTower(MapPlace* place,int towerLabelCata);
    void hasAddTower();
    void hasAddEnemy();
protected:
    int x,y,cata,rPos,cPos;
    int towerLabelCata;
    bool isTransparent;
    bool needShow;
    QString ntransparent,transparent;
    QString backPath;
    Tower* tower;
    QString towerLabelPath;
    MapPlace *nextPlace;
    MapPlace *lastPlace;
    bool canBeChose;
    QWidget *p;
    QPixmap pix;
};

class RoadPlace : public MapPlace
{
     Q_OBJECT
public:
    explicit RoadPlace(int x1,int y1,int place_cata,int r, int c);   
    bool haveEnemy() const;
    bool haveFlyEnemy() const;
    bool haveGroundEnemy() const;
    void addEnemy(Enemy* e);
    void removeEnemy(int enemy_id);
    Enemy* getEnemy() const;
    Enemy* getFlyEnemy() const;
    Enemy* getGroundEnemy() const;
    QVector<Enemy*>* getAllEnemy();
    void setCanChose();
private:
    QVector<Enemy*> enemys;
};

class RemotePlace:public MapPlace
{
     Q_OBJECT
public:
    explicit RemotePlace(int x1,int y1,int place_cata,int r, int c);
    void setPlace(RoadPlace* mp);
    void setCanChose();

private:


};

#endif // MAPPLACE_H
