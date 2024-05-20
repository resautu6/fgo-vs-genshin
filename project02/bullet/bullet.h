#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include <QPainter>
#include <QVector2D>
#include <QDebug>
#include <QPicture>

class Bullet : public QObject
{
    Q_OBJECT
public:
    explicit Bullet(QPoint cPos,QPoint tPos, int cata, int num, QObject *parent = nullptr);
    void bulletMove();
    void drawBullet(QPainter *painter) const;
    int getBulletNumber() const;

signals:
    void arriveTarget(int bullet_number);
protected:
    int speed;
    int bulletCata;
    int bulletNumber;
    int wide,height;
    float moveX,moveY;
    float wPix,hPix;
    QPointF curPos;
    QPointF targetPos;
    QString pixPath;
    QPixmap pix;

};

#endif // BULLET_H
