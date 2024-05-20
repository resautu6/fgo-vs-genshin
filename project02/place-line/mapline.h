#ifndef MAPLINE_H
#define MAPLINE_H

#include "mapplace.h"

#include <QObject>
#include <QVector>

class MapLine : public QObject
{
    Q_OBJECT
public:
    explicit MapLine(QObject *parent = nullptr);

signals:

private:
    QVector<MapPlace*> _place;
    int _cata;
};

#endif // MAPLINE_H
