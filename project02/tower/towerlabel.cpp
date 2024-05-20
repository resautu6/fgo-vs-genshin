#include "towerlabel.h"
#include "tower/tower.h"
#include "tower/meleetower.h"
#include "tower/remotetower.h"
#include "gamescene.h"

TowerLabel::TowerLabel(int towerid):towerId(towerid)
{
    switch(towerid){
    case 1:{
        labelPath=":/tower/object/artonia/artoriaLabel.png";
        pressLabelPath=":/tower/object/artonia/Artoria.png";
        labelCata=1;
        towerPlaceCata=1;
        cost=10;
        labelInformation="阿尔托莉雅：可近战攻击地方地面单位";
        break;
    }
    case 2:{
        labelPath=":/tower/object/gilgamesh/gilgameshLabel.png";
        pressLabelPath=":/tower/object/gilgamesh/gilgamesh.png";
        labelCata=2;
        towerPlaceCata=3;
        cost=15;
        labelInformation="吉尔伽美什：可远程攻击敌方地面单位";
        break;
    }
    case 3:{
        labelPath=":/tower/object/meltryllis/meltryllisLabel.png";
        pressLabelPath=":/tower/object/meltryllis/stationary.png";
        labelCata=3;
        towerPlaceCata=1;
        cost=20;
        labelInformation="meltryllis：可近战攻击敌方地面和飞行单位";
        break;
    }
    case 4:{
        labelPath=":/tower/object/emiya/emiyaLabel.png";
        pressLabelPath=":/tower/object/emiya/archer_stationary.png";
        labelCata=4;
        towerPlaceCata=3;
        cost=15;
        labelInformation="卫宫：可远程攻击敌方地面和飞行单位";
        break;
    }
    case 5:{
        labelPath=":/tower/object/maxiu/maxiuLabel.png";
        pressLabelPath=":/tower/object/maxiu/maxiu.png";
        labelCata=5;
        towerPlaceCata=1;
        cost=10;
        labelInformation="玛修：较高血量，防御性，不能攻击";
        break;
    }
    case 6:{
        labelPath=":/tower/object/Avenger/AvengerLabel.png";
        pressLabelPath=":/tower/object/Avenger/Avenger.png";
        labelCata=6;
        towerPlaceCata=1;
        cost=5;
        labelInformation="安格拉曼钮：无法被攻击，可以造成低额伤害，有极低秒杀敌方";
        break;
    }
    case 7:{
        labelPath=":/tower/object/merlin/MerlinLabel.png";
        pressLabelPath=":/tower/object/merlin/Merlin.png";
        labelCata=7;
        towerPlaceCata=4;
        cost=30;
        labelInformation="梅林：周围无法放置己方单位的格子内随机召唤芙芙，为周围己方单位回复生命值";
        break;
    }
    default: qDebug()<<"card chose have an error";break;
    }
    QPixmap road_pix(labelPath);
    this->setScaledContents(true);
    this->setPixmap(road_pix);
    isChosen=false;

    this->setMouseTracking(true);
    this->setToolTip(this->labelInformation);

}
void TowerLabel::mouseReleaseEvent(QMouseEvent *event){
    if(!isChosen)
    {
        if(gameScene->getStoneNumber()>=cost)
        {
            QPixmap labelCursor(pressLabelPath);
            QCursor cCursor(labelCursor);
            setCursor(cCursor);
            isChosen=true;
            emit sendTowerLabel(pressLabelPath,labelCata,towerPlaceCata);
        }
    }
    else{
        QCursor cCursor(Qt::ArrowCursor);
        setCursor(cCursor);
        isChosen=false;
        emit cancelChoseTower();
    }

}

void TowerLabel::setNoIsChosen()
{
    isChosen=false;
    QCursor cCursor(Qt::ArrowCursor);
    setCursor(cCursor);
    isChosen=false;
}

void TowerLabel::setParent(GameScene *parent)
{
    QLabel::setParent(parent);
    gameScene=parent;
}

Tower *TowerLabel::makeTower(int tower_id,int tower_label_cata)
{
    switch(tower_label_cata){
    case 1:{Tower* temp=new Artoria(tower_id);return temp;break;}
    case 2:{Tower* temp=new Gilgamesh(tower_id);return temp;break;}
    case 3:{Tower* temp=new Meltryllis(tower_id);return temp;break;}
    case 4:{Tower* temp=new Emiya(tower_id);return temp;break;}
    case 5:{Tower* temp=new Maxiu(tower_id);return temp;break;}
    case 6:{Tower* temp=new Avenger(tower_id);return temp;break;}
    case 7:{Tower* temp=new Merlin(tower_id);return temp;break;}
    default:break;
    }
    return NULL;
}

int TowerLabel::getLabelCata()
{
    return labelCata;
}

int TowerLabel::getCost()
{
    return cost;
}
