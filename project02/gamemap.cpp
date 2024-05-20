#include "gamemap.h"

GameMap::GameMap(QObject *parent)
    : QObject{parent}
{

}
GameMap::GameMap(int no):num(no)
{
    map_path=":/level01/map01.png";
}
