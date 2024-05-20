#ifndef BULLETTHREAD_H
#define BULLETTHREAD_H

#include <QObject>
#include <QThread>
#include <QRunnable>
#include <QVector>
#include <QTimer>
#include <QDebug>
#include <QPainter>

class GameEngine;
class BulletThread : public QThread,public QRunnable
{
    Q_OBJECT
public:
    explicit BulletThread(GameEngine *g = nullptr);
    void run() override;
    void drawBullet(QPainter* painter);
    void controlTimer();
signals:
private:
    bool isActive;
    GameEngine *gameEngine;
    QTimer *timer;
};

#endif // BULLETTHREAD_H
