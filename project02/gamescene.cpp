#include "gamescene.h"
#include "gamemap.h"
#include "config_file/dataconfig.h"
#include "place-line/mapplace.h"
#include "tower/towerlabel.h"
#include "game_engine/gameengine.h"

GameScene::GameScene(QWidget *parent)
    : QMainWindow{parent}
{
    setFixedSize(1280,750);
    //setAttribute(Qt::WA_DeleteOnClose);
}
GameScene:: GameScene(int no)
    :num(no),isChoseTower(false),overGame(false),towerClicking(false),stoneNumber(25),gameEngine(NULL),isPlayingMusic(true)

{
    //setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(1280,750);
    iconPath=":/image/icon_of_exe.png";
    setWindowIcon(QIcon(iconPath));
    setWindowTitle(("fate grand order VS genshin"));

    DataConfig* dataconfig=new DataConfig(no);
    //recieve the data from the config class
    connect(dataconfig,&DataConfig::sendCreatEnemyList,this,[&](QStack<QPair<int,QString>*> tmp){
        createEnemyList=tmp;
    });
    connect(dataconfig,&DataConfig::sendMapPath,this,[&](QString pa,QString rp){
        mapPath=pa;
        roadPath=rp;
    });
    connect(dataconfig,&DataConfig::sendMap,this,[&](auto v,auto gbp,auto fbp){
        this->_map=v;
        this->groundBeginPlace=gbp;
        this->flyBeginPlace=fbp;
    });
    dataconfig->sendInform();
    setPlaceParent();

    //show the catagory of the place
//    QPushButton *show_cata=new QPushButton(this);
//    show_cata->resize(80,50);
//    show_cata->move(1200,700);
//    show_cata->setText(QString("show cata"));
//    connect(show_cata,&QPushButton::clicked,this,[=](){
//        if(!isShowCata) {
//            showAllPlaceCata();
//            isShowCata=true;
//        }
//        else {
//            hideAllPlaceCata();
//            isShowCata=false;
//        }
//    });
    isShowCata=false;

    // show the road line
//    QPushButton *show_road=new QPushButton(this);
//    roadLine=new QLabel(this);
//    QPixmap road_pix(roadPath);
//    roadLine->setGeometry(390,170,640,500);
//    roadLine->setScaledContents(true);
//    roadLine->setAttribute(Qt::WA_TransparentForMouseEvents,true);
//    roadLine->setPixmap(road_pix);
//    roadLine->setVisible(false);
//    show_road->resize(80,50);
//    show_road->move(1100,700);
//    show_road->setText(QString("show road"));
//    connect(show_road,&QPushButton::clicked,this,&GameScene::showRoadLine);
    isShowRoadLine=false;

    setTowerLabels();

    //start gameengine
    gameEngine=new GameEngine(&_map,&towerLabels,&groundBeginPlace,&flyBeginPlace,&createEnemyList,25,this);

    //create musicplayer
    //musicPlayer=new QMediaPlayer(this);
    playMusic();

    //Label of stone number
    QLabel *stoneNumber=new QLabel(this);
    QPixmap stonePix(":/image/stone_bord.png");
    stoneNumber->setGeometry(110,40,125,110);
    stoneNumber->setScaledContents(true);
    stoneNumber->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    stoneNumber->setPixmap(stonePix);

    showStoneNumber = new QLabel(this);
    showStoneNumber->setAlignment(Qt::AlignCenter);
    QFont font1 ("仿宋", 13 ,75);
    showStoneNumber->setFont(font1);
    showStoneNumber->setNum(25);
    showStoneNumber->setGeometry(145,131,40,20);
    showStoneNumber->setAttribute(Qt::WA_TransparentForMouseEvents,true);

    //Label of base blood
    QLabel *baseBlood=new QLabel(this);
    QPixmap basePix(":/image/base_blood.png");
    baseBlood->setGeometry(230,60,130,70);
    baseBlood->setScaledContents(true);
    baseBlood->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    baseBlood->setPixmap(basePix);

    showStoneBlood = new QLabel(this);
    showStoneBlood->setAlignment(Qt::AlignCenter);
    QFont font2 ("HGB8X_CNKI", 13 ,75);
    showStoneBlood->setFont(font1);
    showStoneBlood->setNum(10);
    showStoneBlood->setGeometry(274,106,40,20);
    showStoneBlood->setAttribute(Qt::WA_TransparentForMouseEvents,true);

    soundButton=new IconButton(":/image/sound.png",1,1,":/image/blackSound.png",true);
    soundButton->setParent(this);
    soundButton->move(1000,0);
    connect(soundButton,&IconButton::clicked,this,[&](){
        if(isPlayingMusic){
            soundButton->changeIcon();
            pauseMusic();
            isPlayingMusic=false;
        }
        else{
            soundButton->changeIcon();
            startMusic();
            isPlayingMusic=true;
        }
    });


}
void GameScene::paintEvent(QPaintEvent*){
    QPainter painter(this);
    QPixmap pix;
    pix.load(mapPath);
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    for(auto &ele1:_map){
        for(auto &ele2:ele1){
            ele2->drawMapPlace(&painter);
        }
    }
    if(gameEngine){
        gameEngine->drawScene(&painter);
    }
}

void GameScene::mouseReleaseEvent(QMouseEvent *event)
{
    if(overGame){
        emit gameClose();
        QTimer::singleShot(500,this,[&](){
            this->close();
        });

    }
    if(towerClicking){
        emit cancelTowerClick();
    }
}

void GameScene::setPlaceParent(){
    for(auto &ele1: _map){
        for(auto &ele2: ele1){
            ele2->setParent(this);
            ele2->setParentScene(this);
        }
    }
}
void GameScene::showAllPlaceCata(){
    for(auto &ele1: _map){
        for(auto &ele2: ele1){
            ele2->showCata();
        }
    }
}
void GameScene::hideAllPlaceCata(){
    for(auto &ele1: _map){
        for(auto &ele2: ele1){
            ele2->hideCata();
        }
    }
}
void GameScene::showRoadLine(){
    if(!isShowRoadLine) {
        roadLine->setVisible(true);
        isShowRoadLine=true;
    }
    else {
        roadLine->setVisible(false);
        isShowRoadLine=false;
    }
}
void GameScene::setTowerLabels(){
//    TowerLabel *temp=new TowerLabel(1);
//    temp->setParent(this);
//    temp->setGeometry(125,160,80,80);
//    towerLabels<<temp;

//    TowerLabel *temp2=new TowerLabel(2);
//    temp2->setParent(this);
//    temp2->setGeometry(125,240,80,80);
//    towerLabels<<temp2;
    for(int i=1;i<=7;i++){
        TowerLabel *temp=new TowerLabel(i);
        temp->setParent(this);
        temp->setGeometry(125,160+80*(i-1),80,80);
        towerLabels<<temp;

    }
}
void GameScene::showPlaceCata(int cata, int tower_place_cata){
    for(auto &ele1:_map){
        for(auto &ele2:ele1){
            if((ele2->getCata()==tower_place_cata || ele2->getCata()==2) && tower_place_cata!=4){
                ele2->showCata();
                ele2->setTowerLabelCata(cata);
            }
            else if(tower_place_cata==4 && ele2->getCata()==3){
                ele2->showCata();
                ele2->setTowerLabelCata(cata);
            }
        }
    }
}
void GameScene::setTowerCursor(QString cursor_path){
    QPixmap labelCursor(cursor_path);
    QCursor cCursor(labelCursor);
    setCursor(cCursor);
}
void GameScene::setDefaultCursor(){
    QCursor cCursor(Qt::ArrowCursor);
    setCursor(cCursor);
    for(auto &ele:towerLabels){
        ele->setAttribute(Qt::WA_TransparentForMouseEvents,false);
    }
}

void GameScene::setChoseTower(QString cursor_path,int label_cata,int tower_place_cata){
    isChoseTower=true;
    setTowerCursor(cursor_path);
    for(auto &ele:towerLabels){
        if(ele->getLabelCata()!=label_cata){
            ele->setNoIsChosen();
            ele->setAttribute(Qt::WA_TransparentForMouseEvents,true);
        }
    }
    showPlaceCata(label_cata,tower_place_cata);
}

void GameScene::setStoneNumber(int num)
{
    stoneNumber=num;
    showStoneNumber->setNum(num);
}

void GameScene::setBaseBlood(int num)
{
    showStoneBlood->setNum(num);
}

void GameScene::setTowerClicking(bool b)
{
    towerClicking=b;
}

void GameScene::cancelChoseTower()
{
    setDefaultCursor();
    hideAllPlaceCata();
}

void GameScene::gameOver()
{
    pauseMusic();
    hideAllPlaceCata();
    setDefaultCursor();
    for(auto &ele:towerLabels){
        ele->setAttribute(Qt::WA_TransparentForMouseEvents,true);
        ele->setNoIsChosen();
    }
    overGame=true;

}

void GameScene::playGameOverMusic(QString music_path)
{
    gameOverMusic=new QSoundEffect(this);
    gameOverMusic->setSource(QUrl::fromLocalFile(music_path));
    gameOverMusic->setLoopCount(1);  //循环次数
    gameOverMusic->setVolume(0.99f); //音量  0~1之间
    gameOverMusic->play();
}

int GameScene::getStoneNumber()
{
    return stoneNumber;
}

void GameScene::playMusic()
{  
    bgmPlayer= new QSoundEffect(this);
    //bgmPlayer->setSource(QUrl::fromLocalFile("qrc:/music_player/music/RADWIMPS.mp3"));
    bgmPlayer->setSource(QUrl::fromLocalFile(":/music_player/music/radwimps.wav"));
    bgmPlayer->setLoopCount(100);  //循环次数
    bgmPlayer->setVolume(0.99f); //音量  0~1之间
    bgmPlayer->play();
//    musicPlayer->setSource(QUrl::fromLocalFile(":/music_player/music/RADWIMPS.mp3"));
//    //musicPlayer->setLoops(QMediaPlayer::Infinite);
//    musicPlayer->setLoops(1);
//    musicPlayer->play();
}

void GameScene::pauseMusic()
{
    //musicPlayer->pause();
    bgmPlayer->setVolume(0);
}

void GameScene::startMusic()
{
    //musicPlayer->play();
    bgmPlayer->setVolume(0.99f);
}

