#include<QtDebug>
#include<QPropertyAnimation>
#include "iconbutton.h"

IconButton::IconButton(QString path,qreal w,qreal h,QString path2,bool change):click_change(change),w(w),h(h),isChange(false)
{
    iconPath=path;
    if(path2!=""){
    this->path2 = path2;
    click_change=true;
    }
    setMyIcon(iconPath);
}
void IconButton::setMyIcon(QString mypath){
    QPixmap map;
    bool YES = map.load(mypath);//加载路径
    if( !YES )
    {
        qDebug() << "图片加载失败";
        return;
    }
    this->setFixedSize(map.width()*w,map.height()*h);//设置按钮的固定大小为图片大小

    this->setStyleSheet("QPushButton{border:0px;}");//设置按钮为图片的形状

    this->setIcon(map);//设置图标

    this->setIconSize(QSize(map.width()*w,map.height()*h));//设置图片在按钮上的显示大小
}
void IconButton::changeIcon(){
    if(!isChange){
        setMyIcon(path2);
        isChange=true;
    }
    else{
        setMyIcon(iconPath);
        isChange=false;
    }
}
void IconButton::zoomDown(){
    QPropertyAnimation* animation = new QPropertyAnimation(this,"geometry");
    animation->setDuration(250);
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    animation->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}
void IconButton::zoomUp(){
    QPropertyAnimation* animation = new QPropertyAnimation(this,"geometry");
    animation->setDuration(250);
    animation->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    animation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}
