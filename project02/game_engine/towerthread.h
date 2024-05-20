#ifndef TOWERTHREAD_H
#define TOWERTHREAD_H

#include <QObject>
#include <QThread>
#include <QRunnable>
#include <QTimer>
#include <QQueue>
class GameEngine;
class Tower;
class TowerThread : public QThread, public QRunnable
{
    Q_OBJECT
public:
    friend class GameEngine;
    explicit TowerThread(GameEngine* g);
    void run() override;
    void removeTower(int tower_number);
    void controlTimer();
signals:
private:
    const int deleteCountMax;
    int deleteCount;
    int deleteTimerDuration;
    bool isActive;
    QTimer *timer1, *timer2;
    GameEngine* gameEngine;
    QQueue<Tower*> deleteTowers;
};

#endif // TOWERTHREAD_H
