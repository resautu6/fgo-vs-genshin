#include "bullet.h"

Bullet::Bullet(QPoint cPos, QPoint tPos, int cata, int num, QObject *parent)
    : QObject{parent},bulletCata(cata),curPos(cPos),bulletNumber(num),targetPos(tPos),wide(25),height(25)
{

    switch (bulletCata) {
    case 1:{pixPath=":/enemys/object/arrow__qqr/arrow.png";speed=5;break;}
    case 2:{pixPath=":/enemys/object/floating/bubble.png";speed=5;break;}
    case 3:{pixPath=":/tower/object/gilgamesh/gilCup.png";speed=5;break;}
    case 4:{pixPath=":/tower/object/emiya/archerStone.png";speed=5;curPos+=QPointF(20,0);break;}
    case 5:{pixPath=":/tower/object/meltryllis/wave.png";speed=8;wide=80;height=80;break;}
    case 6:{pixPath=":/tower/object/meltryllis/waveFanzhuan.png";speed=8;wide=80;height=80;break;}
    default:Q_ASSERT(0);break;
    }
    QVector2D targetVector(targetPos-curPos);
    targetVector.normalize();
    moveX=targetVector.x();
    moveY=targetVector.y();

    pix=QPixmap(pixPath);
    pix = pix.scaled(wide, height, Qt::KeepAspectRatio);


}
//bullet move
void Bullet::bulletMove()
{
    if(curPos.x()<=targetPos.x()+4 && curPos.x()>=targetPos.x()-4 && curPos.y()>=targetPos.y()-4 && curPos.y()<=targetPos.y()+4){
        emit arriveTarget(bulletNumber);
    }
    curPos+=QPointF(moveX,moveY)*speed;
}

//draw bullet
void Bullet::drawBullet(QPainter *painter) const
{
    painter->drawPixmap(curPos,pix);
}

int Bullet::getBulletNumber() const
{
    return bulletNumber;
}
