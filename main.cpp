#include "mainwindow.h"
#include <QApplication>
#include "dialrecognition.h"
#include "takephotos.h"
#include "getthresholdvalue.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    MainWindow w;
//    w.setWindowTitle(QString::fromLocal8Bit("目前现有部分"));
//    w.setFixedSize(600,450);
//    w.show();
dialrecognition *w=new dialrecognition();
w->show();


    return a.exec();
}
