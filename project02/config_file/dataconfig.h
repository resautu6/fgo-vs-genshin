#ifndef DATACONFIG_H
#define DATACONFIG_H

#include "place-line/mapplace.h"

#include <QObject>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QRandomGenerator>
#include <QStack>
#include <QPair>
class DataConfig : public QObject
{
    Q_OBJECT
public:
    explicit DataConfig(QObject *parent = nullptr);
    explicit DataConfig(int no);
    void makePlace(int a[][9]);
    void linkPlace(QStringList ql);
    void sendInform();
    void readLevelFile();
    void setBeginPlace();
    void makeCreateEnemyList(QVector<int> intervals,QVector<QString> enemy_catas);
    QPair<int,QString>* makeEnemyPair(int interval,QString cataNumber);
    QString makeRoadBackRound();
signals:
    void sendMap(QVector<QVector<MapPlace*>> m,QVector<MapPlace*> gbp,QVector<MapPlace*> fbp);
    void sendMapPath(QString mp,QString rp);
    void sendCreatEnemyList(QStack<QPair<int,QString>*> el);
private:
    QString levelPath;
    QVector<QVector<MapPlace*>> _map;
    QVector<MapPlace*> groundBeginPlace,flyBeginPlace;
    QStack<QPair<int,QString>*> enemys;
    QString mapPath,roadPath;
    int number;
};

#endif // DATACONFIG_H
