#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"selectlevel.h"

#include <QMainWindow>
#include <qstring.h>
#include <QPainter>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent*);
    SelectLevel *selectLevel;
private:
    Ui::MainWindow *ui;
    QString mainPath,buttonPath,iconPath;
};
#endif // MAINWINDOW_H
