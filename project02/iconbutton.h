#ifndef ICONBUTTON_H
#define ICONBUTTON_H

#include <QPushButton>
#include <QString>
using namespace std;
class IconButton:public QPushButton
{
public:
    bool click_change;
    explicit IconButton(QString path,qreal w=1,qreal h=1,QString path2="",bool change=false);
    void zoomDown();
    void zoomUp();
    void setMyIcon(QString mypath);
    void changeIcon();
private:
   QString iconPath;
   QString path2;
   qreal w,h;
   bool isChange;
};

#endif // ICONBUTTON_H
