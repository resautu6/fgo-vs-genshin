#include "dataconfig.h"
#include "place-line/mapline.h"

#include <QDebug>

DataConfig::DataConfig(QObject *parent)
    : QObject{parent}
{

}
DataConfig::DataConfig(int no):number(no)
{
    //catagory of place:
    //0:antthing can not be placed(barrier)
    //1:road which can placed own melee power
    //2:road for flying enemy
    //3:place can placed own romote unit
    //4:begin of the enemy
    //5:end of the enemy
    //6:road but can not place own power
    //7:begining of fly enemys
    QVector<int>intervals{200,200,200,300,300,400,300,500,300,500};
    QVector<QString> enemyCatas{"11","11","1141","1221","132141","31214151","1222","415121","4252","1121314151"};
    makeCreateEnemyList(intervals,enemyCatas);
    switch(no){
    case 0:{
        mapPath=":/image/auto level/autoMap.png";
        roadPath="-1";
        levelPath=QFileDialog::getOpenFileName(NULL,"选择关卡文件","","(*.txt)");
        qDebug()<<levelPath;
        readLevelFile();
        setBeginPlace();
        break;
    }
    case 1:{
        mapPath=":/level01/map01.png";
        roadPath=":/level01/roadline.png";
        QStringList nextPlace;
        nextPlace <<"2827"<<"2726"<<"2625"<<"2515"<<"1505"<<"0504" \
                  <<"0403"<<"0313"<<"1323"<<"2322"<<"2221"<<"2111"<<"1110"<<"1000"\
                  <<"3837"<<"3736"<<"3635"<<"3534"<<"3424"<<"2423"\
                  <<"4847"<<"4746"<<"4645"<<"4555"<<"5565"<<"6564"<<"6463"\
                  <<"6353"<<"5343"<<"4342"<<"4241"<<"4151"<<"5150"<<"5060";
        int temp[7][9]{{5,0,3,1,1,1,3,0,0},
                       {6,1,3,1,0,1,3,3,0},
                       {0,1,1,1,2,1,1,1,4},
                       {0,0,0,0,2,2,2,2,7},
                       {0,1,1,1,2,1,1,1,4},
                       {6,1,0,1,0,1,3,3,0},
                       {5,3,3,1,1,1,3,0,0}};
        makePlace(temp);
        linkPlace(nextPlace);
        groundBeginPlace<<_map[2][8]<<_map[4][8];
        flyBeginPlace<<_map[3][8];
        break;
    }
    default: qDebug()<<"map config error";break;
    }

}
void DataConfig::makePlace(int (*a)[9]){
    QVector<MapPlace*> temp;
    for(int i=0;i<7;i++){
        temp.clear();
        for(int j=0;j<9;j++){
            if(a[i][j]==1 || a[i][j]==2 || a[i][j]==6){
                RoadPlace *pt=new RoadPlace(365+75*j,141+79*i,a[i][j],i,j);
                //pt->setGeometry(363+73*i,145+73*j,73,73);
                pt->setGeometry(365+75*j,141+79*i,75,79);
                temp << pt;
            }
            else if(a[i][j]==3){
                RemotePlace *pt=new RemotePlace(365+75*j,141+79*i,a[i][j],i,j);
                //pt->setGeometry(363+73*i,145+73*j,73,73);
                pt->setGeometry(365+75*j,141+79*i,75,79);
                temp << pt;
            }
            else{
                MapPlace *pt=new MapPlace(365+75*j,141+79*i,a[i][j],i,j);
                //pt->setGeometry(363+73*i,145+73*j,73,73);
                pt->setGeometry(365+75*j,141+79*i,75,79);
                temp << pt;
            }
        }
        _map.push_back(temp);
    }
}

void DataConfig::linkPlace(QStringList ql)
{
    for(auto &ele:ql){
        std::string s;
        s=ele.toStdString();
        _map[s[0]-'0'][s[1]-'0']->setNextPlace(_map[s[2]-'0'][s[3]-'0']);
        _map[s[2]-'0'][s[3]-'0']->setLastPlace(_map[s[0]-'0'][s[1]-'0']);
    }
}
void DataConfig::sendInform(){
    emit sendMapPath(mapPath,roadPath);
    emit sendMap(_map, groundBeginPlace,flyBeginPlace);
    emit sendCreatEnemyList(enemys);
}

void DataConfig::readLevelFile()
{
    QFile levelFile(levelPath);
    //levelFile.open(QIODevice::ReadOnly);
    if (!levelFile.open(QIODevice::ReadOnly)) {
        Q_ASSERT(0);
    }
    QTextStream in(&levelFile);
    int col,raw;
    in>>raw>>col;
    QVector<MapPlace*> temp;
    for(int i=0;i<raw;i++){
        temp.clear();
        for(int j=0;j<col;j++){
            int tmp=-1;
            in>>tmp;
            if(tmp==1 || tmp==2 || tmp==6){
                RoadPlace *pt=new RoadPlace(365+75*j,141+79*i,tmp,i,j);
                pt->setGeometry(365+75*j,141+79*i,75,79);
                if(tmp==1){
                    pt->setMapPiture(makeRoadBackRound());
                }
                else if(tmp==2){
                    pt->setMapPiture(":/image/auto level/cloud.png");
                }
                else if(tmp==6){
                    pt->setMapPiture(":/image/auto level/extraRoad.png");
                }
                pt->setNeedShow(true);
                temp << pt;
            }
            else if(tmp==3){
                RemotePlace *pt=new RemotePlace(365+75*j,141+79*i,tmp,i,j);
                pt->setGeometry(365+75*j,141+79*i,75,79);
                pt->setMapPiture(":/image/auto level/remote.png");
                pt->setNeedShow(true);
                temp << pt;
            }
            else{
                MapPlace *pt=new MapPlace(365+75*j,141+79*i,tmp,i,j);
                pt->setGeometry(365+75*j,141+79*i,75,79);
                if(tmp==4 || tmp==7){
                    pt->setMapPiture(":/image/auto level/begin.png");
                    pt->setNeedShow(true);
                }
                else if(tmp==5){
                    pt->setMapPiture(":/image/auto level/end.png");
                    pt->setNeedShow(true);
                }
                temp << pt;
            }
        }
        _map.push_back(temp);
    }
    QStringList tmpStringList;
    int tmpStringInt;
    QString tmpString;
    while(1){
        in>>tmpStringInt;
        if(tmpStringInt==-1) break;
        tmpString=QString::number(tmpStringInt);
        tmpStringList<<tmpString;
        tmpString.clear();
    }
    levelFile.close();
    linkPlace(tmpStringList);
}

void DataConfig::setBeginPlace()
{
    for(auto &ele1:_map){
        for(auto &ele2:ele1){
            if(ele2->getCata()==4){
                groundBeginPlace<<ele2;
            }
            else if(ele2->getCata()==7){
                flyBeginPlace<<ele2;
            }
        }
    }
}

//1:PlentyQqr
//2:ArrowQqr
//3:AddHealthQqr
//4:FloatingElf
//5:PaiMeng
void DataConfig::makeCreateEnemyList(QVector<int> intervals, QVector<QString> enemy_catas)
{
    for(int i=intervals.size()-1;i>=0;i--){
        enemys<<makeEnemyPair(intervals.at(i),enemy_catas.at(i));
    }
}

QPair<int, QString>* DataConfig::makeEnemyPair(int interval, QString cataNumber)
{
    QPair<int,QString>* temp=new QPair<int,QString>(interval,cataNumber);
    return temp;
}

QString DataConfig::makeRoadBackRound()
{

    switch(QRandomGenerator::global()->bounded(3)){
    case 0:return ":/image/auto level/road1.png";break;
    case 1:return ":/image/auto level/road2.png";break;
    case 2:return ":/image/auto level/road3.png";break;
    default:Q_ASSERT(0);break;
    }
    return "-1";
}


