#include "mapplace.h"
#include "tower/tower.h"
#include "enemy/enemy.h"

MapPlace::MapPlace(int x1,int y1,int place_cata,int r, int c)
    :x(x1),y(y1),cata(place_cata),rPos(r),cPos(c),canBeChose(false),needShow(false),towerLabelCata(0)
{
    tower=NULL;
    nextPlace=NULL;
    lastPlace=NULL;
    transparent="background:transparent";
    switch(place_cata){
    case 0:ntransparent = "QLabel{background-color:rgb(255,255,255);border:1px solid rgb(0,0,0);}";break;
    case 1:ntransparent = "QLabel{background-color:rgb(74,245,160);border:1px solid rgb(0,0,0);}";break;
    case 2:ntransparent = "QLabel{background-color:rgb(0,255,0);border:1px solid rgb(0,0,0);}";break;
    case 3:ntransparent = "QLabel{background-color:rgb(255,255,0);border:1px solid rgb(0,0,0);}";break;
    case 4:ntransparent = "QLabel{background-color:rgb(0,0,255);border:1px solid rgb(0,0,0);}";break;
    case 5:ntransparent = "QLabel{background-color:rgb(255,0,0);border:1px solid rgb(0,0,0);}";break;
    case 6:ntransparent = "QLabel{background-color:rgb(255,255,255);border:1px solid rgb(0,0,0);}";break;
    case 7:ntransparent = "QLabel{background-color:rgb(0,0,255);border:1px solid rgb(0,0,0);}";break;
    default:qDebug() <<"catagory for place error!!!";break;
    }
    backPath=":/image/auto level/emptyFrame.png";
    isTransparent=true;
    setAttribute(Qt::WA_TransparentForMouseEvents,true);
}
int MapPlace::xSide() const{return x;}
int MapPlace::ySide() const{return y;}
int MapPlace::getCata() const{return cata;}

int MapPlace::getRPos() const
{
    return rPos;
}

int MapPlace::getCPos() const
{
    return cPos;
}

bool MapPlace::haveTower() const{
    if(tower!=NULL && tower->getCanBeAttack()){
        return true;
    }
    return false;
}

void MapPlace::setNextPlace(MapPlace *mp)
{
    nextPlace=mp;
}

void MapPlace::setLastPlace(MapPlace *mp)
{
    lastPlace=mp;
}

void MapPlace::setMapPiture(QString pix_path)
{
    backPath=pix_path;
}

void MapPlace::setNeedShow(bool b)
{
    needShow=b;
    pix=QPixmap(backPath);
}

//添加防御塔
void MapPlace::addTower(Tower* t){
    tower=t;
    t->drawTower(this->x,this->y);
}

void MapPlace::showAttackRange()
{
    setStyleSheet("QLabel{background-color:rgb(255,128,0);border:1px solid rgb(0,0,0);}");
}

void MapPlace::hideAttackRange()
{
    setStyleSheet(transparent);
}

//移除防御塔
void MapPlace::removeTower(){tower=NULL;}

//展示格子类型并显示
void MapPlace::showCata(){
    if(isTransparent) {
        setStyleSheet(ntransparent);
        setAttribute(Qt::WA_TransparentForMouseEvents,false);
        isTransparent=false;
        canBeChose=true;
    }
}

//隐藏格子类型
void MapPlace::hideCata(){
    if(!isTransparent) {
        setAttribute(Qt::WA_TransparentForMouseEvents,true);
        setStyleSheet(transparent);
        isTransparent=true;
        canBeChose=false;
        towerLabelCata=0;
    }
}

//重写鼠标事件
void MapPlace::mouseReleaseEvent(QMouseEvent *event){
    if(canBeChose){
        emit hasPutTower(this,towerLabelCata);
    }
}

void MapPlace::drawMapPlace(QPainter *painter) const
{
    if(needShow){
        painter->drawPixmap(QPoint(x,y),pix);
    }
}

Tower *MapPlace::getTower()
{
    if(haveTower()){return tower;}
    return NULL;
}

//获取塔的图片路径
void MapPlace::setTowerLabel(QString label_path){
    towerLabelPath=label_path;
}

//设置目前所能放塔的类型
void MapPlace::setTowerLabelCata(int cata){
    towerLabelCata=cata;
}

//设置父亲
void MapPlace::setParentScene(QWidget *parents)
{
    p=parents;
}

MapPlace *MapPlace::getNextMapPlace()
{
    return nextPlace;
}

MapPlace *MapPlace::getLastMapPlace()
{
    return lastPlace;
}


RoadPlace:: RoadPlace(int x1,int y1,int place_cata,int r, int c):MapPlace(x1,y1,place_cata,r,c){

}
bool RoadPlace::haveEnemy() const{
    return !enemys.empty();
}

bool RoadPlace::haveFlyEnemy() const
{
    for(auto &ele : enemys){
        if(ele->getCanFly()) return true;
    }
    return false;
}

bool RoadPlace::haveGroundEnemy() const
{
    for(auto &ele : enemys){
        if(!ele->getCanFly()) return true;
    }
    return false;
}
void RoadPlace::addEnemy(Enemy *e){
    enemys << e;
    emit hasAddEnemy();
}
void RoadPlace::removeEnemy(int emeny_id){
    for(auto it=enemys.begin();it<enemys.end();it++){
        if((*it)->getEnemyNumber()==emeny_id){
            enemys.erase((it));
            break;
        }
    }
}

Enemy *RoadPlace::getEnemy() const
{
    if(haveEnemy()){return enemys[0];}
    return NULL;
}

Enemy *RoadPlace::getFlyEnemy() const
{
    for(auto &ele:enemys){
        if(ele->getCanFly()) return ele;
    }
    return NULL;
}

Enemy *RoadPlace::getGroundEnemy() const
{
    for(auto &ele:enemys){
        if(!ele->getCanFly()) return ele;
    }
    return NULL;
}

QVector<Enemy *> *RoadPlace::getAllEnemy()
{
    return &enemys;
}

//设置目前是否能被选择
void RoadPlace::setCanChose(){
    canBeChose=true;
}

RemotePlace:: RemotePlace(int x1,int y1,int place_cata,int r, int c):MapPlace(x1,y1,place_cata,r,c){

}

//设置是否能被选择
void RemotePlace::setCanChose(){
    canBeChose=true;

}
