#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialrecognition.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    dialrecognition *dialr=new dialrecognition;
    close();
    dialr->show();
}
