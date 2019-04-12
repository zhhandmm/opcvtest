#ifndef DIALRECOGNITION_H
#define DIALRECOGNITION_H
#include <QDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QTableWidget>
#include<QPaintEvent>
#include<QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include  "mylineedit.h"
#include <QLineEdit>
#include <QComboBox>
#include<QCamera>
#include<QCameraImageCapture>
#include <QCameraViewfinder>
#include <QCameraInfo>
#include <QList>
#include <QStackedLayout>
#include <QTimer>
#include <QTextEdit>
#include <QTime>
//extern int thisisatest;
extern QString filename;

class dialrecognition:public QDialog
{
    Q_OBJECT
public:
    explicit dialrecognition(QWidget *parent=nullptr);
float getD(float,float,float,float,float);
float jisuan(float,float,float,float);

QPushButton * stopall;
QPushButton*startcam;
QGroupBox *vediopart;
QGridLayout *grid;
QGridLayout *vediopartgrid;
QLabel *originpic;
QLabel *graypic;

QHBoxLayout *picinsert_0;
QHBoxLayout *picinsert_1;
QVBoxLayout *play_1;

mylineedit *insertpath;
QLineEdit *grayvalue;
mylineedit *editphotolineeidt;
/////////////////////////////////////////
QLabel *twovaluecycle;
QLabel *twovalueline;

QHBoxLayout *picinsert_0_2;
QHBoxLayout *picinsert_1_2;
QVBoxLayout *play_1_2;

QLineEdit *twovalue_1;
QLineEdit *twovalue_2;
//     QLineEdit *twovalue_3;
QLineEdit *thresh_show;

QComboBox *twovalue_3;


//////////////////////////////////////
QLabel *drawcycle;
QLabel *drawline;
QLineEdit*valueshow;

QLineEdit *startdegree;
QLineEdit *enddegree;
QLineEdit *startvalue;
QLineEdit*endvalue;



///////////
QPushButton *begintest;
QPushButton *endtest;
QStackedLayout *changeshow;

QLabel *infor1;
QLabel *infor2;
QTextEdit *infor11;
QTextEdit *infor22;

QTimer *testTimer;

protected:

void paintEvent(QPaintEvent *event);


private slots:
void handlelineeditclicked();
void takephoto();
void editphotos();
void getgraypics();

void bt2_0_0();
void bt2_0_1();
void bt2_1_0();
void bt2_1_1();
void bt2_2_0();
void bt2_2_1();
void bt2_3_0();
void bt2_3_1();
void bt2_4_0();
void bt2_4_1();

void gettvself();
void play3_1();

void testbutton();
void usevedio();
void beginbt();
void holdonbt();
void stopbt();
void timerDone();
void processCapturedImage(int ,const QImage &);
void justfortest();
void stopallwindow();
private:
     QGroupBox *picinsert();
     QGroupBox *vedioinsert();

     QCamera *camerav;//系统摄像头设备
     QCameraViewfinder *cameraViewFinderv;//摄像头取景器部件
     QCameraImageCapture *cameraImageCapturev;//截图部件

};

#endif // DIALRECOGNITION_H
