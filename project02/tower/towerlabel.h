#ifndef TOWERLABEL_H
#define TOWERLABEL_H

#include <QObject>
#include <QLabel>
#include <QString>
#include <QDebug>
class GameScene;
class Tower;
class TowerLabel : public QLabel
{
    Q_OBJECT
public:
    explicit TowerLabel(int towerid);
    void mouseReleaseEvent(QMouseEvent *event);
    void setNoIsChosen();
    void setParent(GameScene *parent);
    static Tower* makeTower(int tower_id,int tower_label_cata);
    int getLabelCata();
    int getCost();
signals:
    void sendTowerLabel(QString label_path,int label_cata,int tower_place_cata);
    void cancelChoseTower();
private:
    QString labelPath;
    QString pressLabelPath;
    QString labelInformation;
    GameScene *gameScene;
    int towerId,labelCata;
    int towerPlaceCata;
    int cost;
    bool isChosen;
};

#endif // TOWERLABEL_H
