#ifndef SELECTLEVEL_H
#define SELECTLEVEL_H

#include <QMainWindow>
#include <QFileDialog>


class SelectLevel : public QMainWindow
{
    Q_OBJECT
public:
    explicit SelectLevel(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*);

signals:

private:
    QString mainPath,iconPath;

};

#endif // SELECTLEVEL_H
