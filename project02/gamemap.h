#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "gamescene.h"
#include "place-line/mapplace.h"

#include <QObject>
#include <QString>
#include <QVector>

class GameMap : public QObject
{
    Q_OBJECT
public:
    friend class GameScene;
    explicit GameMap(QObject *parent = nullptr);
    GameMap(int no);

signals:

private:
    QString map_path;
    int num;
    QVector<MapPlace*> _line;
};

#endif // GAMEMAP_H
