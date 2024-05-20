QT       += core gui multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bullet/bullet.cpp \
    config_file/dataconfig.cpp \
    enemy/enemy.cpp \
    enemy/enemys.cpp \
    game_engine/bulletthread.cpp \
    game_engine/enemythread.cpp \
    game_engine/gameengine.cpp \
    game_engine/towerthread.cpp \
    gamescene.cpp \
    iconbutton.cpp \
    main.cpp \
    mainwindow.cpp \
    place-line/mapplace.cpp \
    selectlevel.cpp \
    tower/meleetower.cpp \
    tower/remotetower.cpp \
    tower/tower.cpp \
    tower/towerlabel.cpp

HEADERS += \
    bullet/bullet.h \
    config_file/dataconfig.h \
    enemy/enemy.h \
    enemy/enemys.h \
    game_engine/bulletthread.h \
    game_engine/enemythread.h \
    game_engine/gameengine.h \
    game_engine/towerthread.h \
    gamescene.h \
    iconbutton.h \
    mainwindow.h \
    place-line/mapplace.h \
    selectlevel.h \
    tower/meleetower.h \
    tower/remotetower.h \
    tower/tower.h \
    tower/towerlabel.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res/mainscene.qrc
#RC_ICONS = ""
RC_ICONS = ico.ico
