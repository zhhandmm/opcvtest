#ifndef TAKEPHOTOS_H
#define TAKEPHOTOS_H
#include<QApplication>
#include <QWidget>
#include <QDialog>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QPushButton>
#include <QLabel>
#include <QImage>
class takephotos:public QDialog
{
    Q_OBJECT
public:
    takephotos(QWidget *parent=0);
    ~takephotos();

private slots:
    void captureBtnResponded();
    void saveBtnResponded();
    void exitBtnResponded();
    void cameraImageCaptured(int id, QImage image);
private:
//    QCamera * m_pCamera;//读取摄像头
//    QCameraViewfinder* m_pViewfinder;//渲染摄像头
//    QCameraImageCapture*m_pImageCapture;//获取摄像头当前帧

    QCamera *camera;//系统摄像头设备
    QCameraViewfinder *cameraViewFinder;//摄像头取景器部件
    QCameraImageCapture *cameraImageCapture;//截图部件

    QPushButton *captureBtn;
    QPushButton *saveBtn;
    QPushButton *exitBtn;
    QLabel *displayLabel;


};

#endif // TAKEPHOTOS_H
