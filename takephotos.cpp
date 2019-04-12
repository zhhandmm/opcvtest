#include "takephotos.h"
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
#include "dialrecognition.h"
//#include "mylineedit.h"

takephotos::takephotos(QWidget *parent) :
    QDialog(parent,Qt::WindowTitleHint|Qt::CustomizeWindowHint)
{

    this->resize(1350, 660);
    this->setWindowTitle(QString::fromLocal8Bit("拍照获取图片"));
    camera = new QCamera("/dev/video0",this);

    QCameraViewfinderSettings set;
    set.setResolution(800,600);
    camera->setViewfinderSettings(set);
    cameraViewFinder = new QCameraViewfinder();

    cameraImageCapture = new QCameraImageCapture(camera);
//     qDebug()<<cameraImageCapture->isCaptureDestinationSupported(QCameraImageCapture::CaptureToBuffer);//测试系统是否支持图像输出到缓冲区


    captureBtn = new QPushButton(QString::fromLocal8Bit("拍照"));
    captureBtn->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:black;background-color:transparent;border-style: outset}" // 按键本色
                              "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                             "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
    saveBtn = new QPushButton(QString::fromLocal8Bit("保存"));
    saveBtn->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:black;background-color:transparent;border-style: outset}" // 按键本色
                           "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                          "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
    exitBtn = new QPushButton(QString::fromLocal8Bit("退出"));
    exitBtn->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:black;background-color:transparent;border-style: outset}" // 按键本色
                           "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                          "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩

    displayLabel = new QLabel();
    displayLabel->setFixedSize(160, 120);
    //打开自动平衡收放图片，显示图像大小，自动调节为QLabel的大小。
    displayLabel->setScaledContents(true);
    //部件垂直布局
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(displayLabel);
    rightLayout->addStretch();
    rightLayout->addWidget(captureBtn);
    rightLayout->addWidget(saveBtn);
    rightLayout->addWidget(exitBtn);
    //部件水平布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(cameraViewFinder);
    mainLayout->addLayout(rightLayout);

    connect(captureBtn, SIGNAL(clicked()), this, SLOT(captureBtnResponded()));
    connect(saveBtn, SIGNAL(clicked()), this, SLOT(saveBtnResponded()));
    connect(exitBtn, SIGNAL(clicked()), this, SLOT(exitBtnResponded()));
    connect(cameraImageCapture, SIGNAL(imageCaptured(int,QImage)), this,SLOT(cameraImageCaptured(int,QImage)));

    cameraImageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    camera->setCaptureMode(QCamera::CaptureStillImage);
    camera->setViewfinder(cameraViewFinder);
    camera->start();//启动摄像头

    this->setLayout(mainLayout);/*必须执行这句代码才显现出布局*/
}
takephotos::~takephotos()
{

}
void takephotos::captureBtnResponded()
{
    cameraImageCapture->capture();
}
void takephotos::saveBtnResponded()
{
    const QPixmap *pixmap = displayLabel->pixmap();

    if(pixmap)
    {
    pixmap->save("pic/a.jpg");
    }
    filename="pic/a.jpg";

}
void takephotos::exitBtnResponded()
{
    camera->stop();
    this->close();
}
void takephotos::cameraImageCaptured(int id, QImage image)
{
    displayLabel->setPixmap(QPixmap::fromImage(image));
}
