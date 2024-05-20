#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QMainWindow>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include <QPushButton>
#include <QMouseEvent>
#include <QTimer>
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QStack>
#include <QPair>

#include "iconbutton.h"

class MapPlace;
class TowerLabel;
class GameEngine;
class GameScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameScene(QWidget *parent = nullptr);
    explicit GameScene(int no);
    void paintEvent(QPaintEvent*);
    void mouseReleaseEvent(QMouseEvent *event);
    void setPlaceParent();
    void showAllPlaceCata();
    void hideAllPlaceCata();
    void showPlaceCata(int cata,int tower_place_cata);
    void showRoadLine();
    void setTowerLabels();
    void setTowerCursor(QString cursor_path);
    void setDefaultCursor();
    void setChoseTower(QString cursor_path,int label_cata,int tower_place_cata);
    void setStoneNumber(int num);
    void setBaseBlood(int num);
    void setTowerClicking(bool b);
    void cancelChoseTower();
    void gameOver();
    void playMusic();
    void pauseMusic();
    void startMusic();
    void playGameOverMusic(QString music_path);
    int getStoneNumber();
signals:
    void gameClose();
    void cancelTowerClick();
private:
    int num;
    int stoneNumber;
    bool isShowCata,isShowRoadLine,isChoseTower;
    bool overGame;
    bool towerClicking;
    bool isPlayingMusic;
    QString iconPath;
    QString mapPath;
    QString roadPath;
    QLabel *roadLine,*showStoneNumber,*showStoneBlood;
    MapPlace* ml;
    QVector<QVector<MapPlace*>> _map;
    QVector<MapPlace*> groundBeginPlace,flyBeginPlace;
    QVector<TowerLabel*> towerLabels;
    GameEngine * gameEngine;
    QMediaPlayer *musicPlayer;
    QSoundEffect *gameOverMusic, *bgmPlayer;
    QStack<QPair<int,QString>*> createEnemyList;
    IconButton *soundButton;
};

#endif // GAMESCENE_H
