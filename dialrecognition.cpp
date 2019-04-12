#include "dialrecognition.h"
#include"QStyleOption"
#include"QPixmap"
#include "QPainter"
#include "QStyle"
#include "QPaintEvent"
#include"QWidget"
#include "mylineedit.h"
#include <QLineEdit>
#include <QFileDialog>
#include <QDebug>
#include "takephotos.h"
#include <QPixmap>
#include "opencv2/opencv.hpp"
//#include "cv.h"
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/core/core.hpp"
//#include "cv.hpp"
#include<QMessageBox>
#include"getthresholdvalue.h"
//#include <opencv\cv.h>
#include "recganddraw.h"
#include "getline.h"
#include "math.h"
#include <QLayoutItem>

//#include <iostream>
//using namespace std;
using namespace cv;
int egvalue=0;
int HistogramBins = 256;
float HistogramRange1[2]={0,255};
float *HistogramRange[1]={&HistogramRange1[0]};
int selfvalue;
int v3;
float getavg[5]={150.5,150.5,150.5,150.5,150.5};
typedef
enum {back,object} entropy_state;

QString filename;


#define cvQueryHistValue_1D( hist, idx0 ) \
    ((float)cvGetReal1D( (hist)->bins, (idx0)))

void  MaxEntropy(IplImage *src,IplImage *dst);


void dialrecognition::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
dialrecognition::dialrecognition(QWidget *parent):
    QDialog(parent)
{
    this-> setObjectName("backg");
    this->setStyleSheet("#backg{border-image:url(:/img/back.jpg)}");
    setWindowTitle(QString::fromLocal8Bit("仪表盘识别系统"));
    setWindowFlags(Qt::WindowFullscreenButtonHint);
    setWindowState(Qt::WindowMaximized);
    setMinimumSize(900,400);

    grid=new QGridLayout();
    grid->addWidget(picinsert(),0,0);
    grid->addWidget(vedioinsert(),0,1);
    grid->setColumnStretch(0,16);
    grid->setColumnStretch(1,8);
    this->setLayout(grid);
}

QGroupBox *dialrecognition::picinsert()
{
//这部分为获取图片作为输入，进行灰度、二值化、霍夫圆检测、霍夫直线检测，在原始图上画出检测的圆与直线并计算读数
//该部分主要由四个Horizontal构成每个Horizontal中含一个子Horizontal和一个Vertical
    QGroupBox *picpart=new QGroupBox(QString::fromLocal8Bit("图片导入"));
     picpart->setStyleSheet("color:white");

     QVBoxLayout *mainlayout=new QVBoxLayout;
//第一部分Horizontal的子Horizontal含两个显示图片（原，灰度）的label，Vertical含两个LineEdit,三个button
//开始
     picinsert_0=new QHBoxLayout;
     picinsert_1=new QHBoxLayout;
     play_1=new QVBoxLayout;

//以下部分之按钮，点击打开摄像头并拍照，将图片保存至指定位置，同时将路径传递给filepath
     QPushButton *takephoto=new QPushButton;
     takephoto->setText(QString::fromLocal8Bit("拍照获取图片"));
     takephoto->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset}" // 按键本色
                              "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                             "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
     connect(takephoto,SIGNAL(clicked(bool)),this,SLOT(takephoto()));

     QPushButton *editphoto=new QPushButton;
     editphoto->setText(QString::fromLocal8Bit("导入图片"));
     editphoto->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset}" // 按键本色
                              "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                             "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
     connect(editphoto,SIGNAL(clicked(bool)),this,SLOT(editphotos()));

     QPushButton *getgraypic=new QPushButton(QString::fromLocal8Bit("获取灰度图片"));
     getgraypic->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset}" // 按键本色
                               "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                              "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
     connect(getgraypic,SIGNAL(clicked(bool)),this,SLOT(getgraypics()));
//LINEEDIT点击弹出路径选择框，选择后路径保存在filepath中，并在此显示出
     editphotolineeidt=new mylineedit(this);
     editphotolineeidt->setMinimumHeight(30);
     editphotolineeidt->setPlaceholderText(QString::fromLocal8Bit("请选择图片"));
     editphotolineeidt->setStyleSheet("color:gray");
     connect(editphotolineeidt,SIGNAL(clicked()),this,SLOT(handlelineeditclicked()));


     originpic=new QLabel;
     graypic=new QLabel;

     originpic->setStyleSheet("background-color:white");
     originpic->setScaledContents(false);
     graypic->setStyleSheet("background-color:white");
     graypic->setScaledContents(false);


     QLabel *title1=new QLabel(QString::fromLocal8Bit("**原或灰度图获取**"));
     title1->setStyleSheet("font:14pt;font:bold;background-color:transparent");
     title1->setAlignment(Qt::AlignCenter);
     QLabel *title1_2=new QLabel(QString::fromLocal8Bit("****************"));
     title1_2->setStyleSheet("font:14pt;font:bold;background-color:transparent");
     title1_2->setAlignment(Qt::AlignCenter);

     play_1->addWidget(title1);
     play_1->addStretch(2);
     play_1->addWidget(editphotolineeidt);
     play_1->addStretch(1);
     play_1->addSpacing(1);
     play_1->addWidget(takephoto);
     play_1->addWidget(editphoto);
     play_1->addStretch(1);
     play_1->addWidget(getgraypic);
     play_1->addStretch(2);
     play_1->addWidget(title1_2);

     picinsert_1->addWidget(originpic);
     picinsert_1->addWidget(graypic);
     picinsert_0->addLayout(picinsert_1,10);
     picinsert_0->addLayout(play_1,5);
//结束

//第二部分
//开始
     picinsert_0_2=new QHBoxLayout;
     picinsert_1_2=new QHBoxLayout;
     play_1_2=new QVBoxLayout;

     QGridLayout *play_2_show=new QGridLayout;

     twovaluecycle=new QLabel;
     twovalueline=new QLabel;
     QLabel *thresh_show_label=new QLabel(QString::fromLocal8Bit("阀值参考"));
     thresh_show_label->setStyleSheet("background-color:transparent;color:white");
     thresh_show_label->setAlignment(Qt::AlignCenter);
     QLabel *title2=new QLabel(QString::fromLocal8Bit("**图形二值化处理**"));
     title2->setStyleSheet("font:9pt;font:bold;background-color:transparent");
     title2->setAlignment(Qt::AlignCenter);


     QLabel *title2_2=new QLabel(QString::fromLocal8Bit("****************"));
     title2_2->setStyleSheet("font:14pt;font:bold;background-color:transparent");
     title2_2->setAlignment(Qt::AlignCenter);


     twovaluecycle->setStyleSheet("background-color:white");
     twovaluecycle->setScaledContents(false);
     twovalueline->setStyleSheet("background-color:white");
     twovalueline->setScaledContents(false);

     twovalue_1=new QLineEdit;
     twovalue_2=new QLineEdit;
     twovalue_3=new QComboBox;

     thresh_show=new QLineEdit;
     thresh_show->setStyleSheet("color:white;background-color:transparent");
     thresh_show->setAlignment(Qt::AlignCenter);
     thresh_show->setReadOnly(true);


     twovalue_1->setPlaceholderText(QString::fromLocal8Bit("请输入阀值"));
     twovalue_1->setStyleSheet("color:gray");
//     twovalue_1->setMinimumHeight(30);
     twovalue_2->setPlaceholderText(QString::fromLocal8Bit("请输入最大值"));
     twovalue_2->setStyleSheet("color:gray");
//     twovalue_2->setMinimumHeight(30);

     twovalue_3->setCurrentText(QString::fromLocal8Bit("请选择阀值类型"));
//     twovalue_3->setMinimumHeight(30);

     twovalue_3->addItem(tr("CV_THRESH_BINARY=0"));
     twovalue_3->addItem(tr("CV_THRESH_BINARY_INV=1"));
     twovalue_3->addItem(tr("CV_THRESH_TRUNC=2"));
     twovalue_3->addItem(tr("CV_THRESH_TOZERO=3"));
     twovalue_3->addItem(tr("CV_THRESH_TOZERO_INV=4"));
     twovalue_3->addItem(tr("CV_THRESH_MASK=7"));
     twovalue_3->addItem(tr("CV_THRESH_OTSU=8"));
     twovalue_3->addItem(tr("CV_THRESH_TRIANGLE=16"));
     twovalue_3->setStyleSheet("color:balck");

     QPushButton *gettwovalue=new QPushButton(QString::fromLocal8Bit("阈值获取"));
//     gettwovalue->setMinimumHeight(50);
     gettwovalue->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset;}" // 按键本色
                                "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                               "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
 connect(gettwovalue,SIGNAL(clicked(bool)),this,SLOT(gettvself()));

     QPushButton *gettwovaluecycle=new QPushButton;
    gettwovaluecycle->setText(QString::fromLocal8Bit("指定阈值-圆"));
    gettwovaluecycle->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset}" // 按键本色
                                    "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                                   "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
    connect(gettwovaluecycle,SIGNAL(clicked(bool)),this,SLOT(bt2_0_0()));


     QPushButton*gettwovalueline=new QPushButton;
     gettwovalueline->setText(QString::fromLocal8Bit("指定阀值-线"));
     gettwovalueline->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset}" // 按键本色
                                    "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                                   "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
     connect(gettwovalueline,SIGNAL(clicked(bool)),this,SLOT(bt2_0_1()));


//     QPushButton *gettwovaluecycle_1=new QPushButton;
//    gettwovaluecycle_1->setText(QString::fromLocal8Bit("全局阀一-圆"));
//    gettwovaluecycle_1->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset}" // 按键本色
//                                      "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
//                                     "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
//     connect(gettwovaluecycle_1,SIGNAL(clicked(bool)),this,SLOT(bt2_1_0()));
//     gettwovaluecycle_1->setDisabled(true);



//     QPushButton*gettwovalueline_1=new QPushButton;
//     gettwovalueline_1->setText(QString::fromLocal8Bit("全局阀一-线"));
//     gettwovalueline_1->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset}" // 按键本色
//                                      "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
//                                     "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
//      connect(gettwovalueline_1,SIGNAL(clicked(bool)),this,SLOT(bt2_1_1()));
//      gettwovalueline_1->setDisabled(true);


//     QPushButton *gettwovaluecycle_2=new QPushButton;
//    gettwovaluecycle_2->setText(QString::fromLocal8Bit("全局阀二-圆"));
//    gettwovaluecycle_2->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset}" // 按键本色
//                                      "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
//                                     "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
//     connect(gettwovaluecycle_2,SIGNAL(clicked(bool)),this,SLOT(bt2_2_0()));
//     gettwovaluecycle_2->setDisabled(true);



//     QPushButton*gettwovalueline_2=new QPushButton;
//     gettwovalueline_2->setText(QString::fromLocal8Bit("全局阀二-线"));
//     gettwovalueline_2->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset}" // 按键本色
//                                      "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
//                                     "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
//      connect(gettwovalueline_2,SIGNAL(clicked(bool)),this,SLOT(bt2_2_1()));
//    gettwovalueline_2->setDisabled(true);



     QPushButton *gettwovaluecycle_3=new QPushButton;
    gettwovaluecycle_3->setText(QString::fromLocal8Bit("阈值自处理一"));
    gettwovaluecycle_3->setStyleSheet("QToolTip{border:1px solid rgb(118, 118, 118); background-color:transparent; color:#484848; font-size:12px;}"
                                      "QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset}" // 按键本色
                                      "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                                     "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
    gettwovaluecycle_3->setToolTip("局部阈值自适应处理");
     connect(gettwovaluecycle_3,SIGNAL(clicked(bool)),this,SLOT(bt2_3_0()));

//     QPushButton*gettwovalueline_3=new QPushButton;
//     gettwovalueline_3->setText(QString::fromLocal8Bit("局部阀值-线"));
//     gettwovalueline_3->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset}" // 按键本色
//                                      "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
//                                     "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
//      connect(gettwovalueline_3,SIGNAL(clicked(bool)),this,SLOT(bt2_3_1()));

     QPushButton *gettwovaluecycle_4=new QPushButton;
    gettwovaluecycle_4->setText(QString::fromLocal8Bit("阈值自处理二"));
    gettwovaluecycle_4->setStyleSheet("QToolTip{border:1px solid rgb(118, 118, 118); background-color:transparent; color:#484848; font-size:12px;}"
                "QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset}" // 按键本色
                                      "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                                     "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
    gettwovaluecycle_4->setToolTip("自算阈值获取处理图片");
     connect(gettwovaluecycle_4,SIGNAL(clicked(bool)),this,SLOT(bt2_4_0()));

//     QPushButton*gettwovalueline_4=new QPushButton;
//     gettwovalueline_4->setText(QString::fromLocal8Bit("自算阀值-线"));
//     gettwovalueline_4->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset}" // 按键本色
//                                      "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
//                                     "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
//      connect(gettwovalueline_4,SIGNAL(clicked(bool)),this,SLOT(bt2_4_1()));



     play_2_show->addWidget(gettwovalue,0,0,1,2);
     play_2_show->addWidget(twovalue_1,1,0);
     play_2_show->addWidget(twovalue_2,1,1);
     play_2_show->addWidget(twovalue_3,2,0,1,2);
     play_2_show->addWidget(gettwovaluecycle,3,0);
     play_2_show->addWidget(gettwovalueline,3,1);


//     play_2_show->addWidget(gettwovaluecycle_1,3,0);
//     play_2_show->addWidget(gettwovalueline_1,3,1);
//     play_2_show->addWidget(gettwovaluecycle_2,4,0);
//     play_2_show->addWidget(gettwovalueline_2,4,1);
     play_2_show->addWidget(gettwovaluecycle_3,4,0,1,1);
//     play_2_show->addWidget(gettwovalueline_3,5,1);
     play_2_show->addWidget(gettwovaluecycle_4,4,1,1,1);
//     play_2_show->addWidget(gettwovalueline_4,5,1);

     play_2_show->addWidget(thresh_show_label,6,0);
     play_2_show->addWidget(thresh_show,6,1);




     play_1_2->addWidget(title2);
     play_1_2->addStretch(0);
     play_1_2->addLayout(play_2_show);
     play_1_2->addStretch(0);
//     play_1_2->addWidget(title2_2);
     picinsert_1_2->addWidget(twovaluecycle);
     picinsert_1_2->addWidget(twovalueline);
     picinsert_0_2->addLayout(picinsert_1_2,10);
     picinsert_0_2->addLayout(play_1_2,5);





 //结束
//第三部分
//开始
     QHBoxLayout *picinsert_2=new QHBoxLayout;
     QHBoxLayout *picinsert_2_0=new QHBoxLayout;
     QVBoxLayout *play_3=new QVBoxLayout;

     QLabel *title3=new QLabel(QString::fromLocal8Bit("**标准化结果输出**"));
     title3->setStyleSheet("font:9pt;font:bold;background-color:transparent");
     title3->setAlignment(Qt::AlignCenter);
     QLabel *title3_2=new QLabel(QString::fromLocal8Bit("****************"));
     title3_2->setStyleSheet("font:9pt;font:bold;background-color:transparent");
     title3_2->setAlignment(Qt::AlignCenter);


     QPushButton *getcyclepic=new QPushButton(QString::fromLocal8Bit("识别并标记圆"));
     getcyclepic->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;bold;color:white;background-color:transparent;border-style: outset}" // 按键本色
                                "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                               "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
     getcyclepic->setFixedHeight(32);
     connect(getcyclepic,SIGNAL(clicked(bool)),this,SLOT(play3_1()));

     QPushButton *getvalue=new QPushButton(QString::fromLocal8Bit("获取结果"));
     getvalue->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;bold;color:white;background-color:transparent;border-style: outset}" // 按键本色
                             "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                            "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
     getvalue->setFixedHeight(32);
     connect(getvalue,SIGNAL(clicked(bool)),this,SLOT(testbutton()));


     QPushButton *fortest=new QPushButton(QString::fromLocal8Bit("片段测试按钮"));
     fortest->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;bold;color:white;background-color:transparent;border-style: outset}" // 按键本色
                           "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                          "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
     fortest->setFixedHeight(32);
     connect(fortest,SIGNAL(clicked(bool)),this,SLOT(justfortest()));



     drawcycle=new QLabel;
     drawline=new QLabel;
     drawcycle->setStyleSheet("background-color:white");
     drawline->setStyleSheet("background-color:white");

     valueshow=new QLineEdit;
//     valueshow->setMinimumHeight(40);
     valueshow->setStyleSheet("color:black");
     valueshow->setAlignment(Qt::AlignCenter);
     valueshow->setReadOnly(true);


     startdegree=new QLineEdit;
     startdegree->setPlaceholderText(QString::fromLocal8Bit("起始角度"));
     startdegree->setStyleSheet("color:gray");
//     startdegree->setMinimumHeight(30);
     enddegree=new QLineEdit;
     enddegree->setPlaceholderText(QString::fromLocal8Bit("终止角度"));
     enddegree->setStyleSheet("color:gray");
//     enddegree->setMinimumHeight(30);
     startvalue=new QLineEdit;
     startvalue->setPlaceholderText(QString::fromLocal8Bit("起始值"));
     startvalue->setStyleSheet("color:gray");
//     startvalue->setMinimumHeight(30);
     endvalue=new QLineEdit;
     endvalue->setPlaceholderText(QString::fromLocal8Bit("末端值"));
     endvalue->setStyleSheet("color:gray");
//     endvalue->setMinimumHeight(30);


     QGridLayout *play3_value_4=new QGridLayout;
     play3_value_4->addWidget(startdegree,0,0);
     play3_value_4->addWidget(enddegree,0,1);
     play3_value_4->addWidget(startvalue,1,0);
     play3_value_4->addWidget(endvalue,1,1);


     play_3->addWidget(title3);
//     play_3->addStretch(1);
     play_3->addWidget(getcyclepic);
//     play_3->addStretch(0);
//     play_3->addWidget(getlinepic);
//     play_3->addStretch(1);
     play_3->addLayout(play3_value_4);
     play_3->addWidget(getvalue);
     play_3->addWidget(valueshow);
//     play_3->addWidget(fortest);
//     play_3->addStretch(1);
//     play_3->addWidget(title3_2);


     picinsert_2_0->addWidget(drawcycle);
     picinsert_2_0->addWidget(drawline);
     picinsert_2->addLayout(picinsert_2_0,10);
     picinsert_2->addLayout(play_3,5);




//结束

     mainlayout->addLayout(picinsert_0,1);
     mainlayout->addLayout(picinsert_0_2,1);
     mainlayout->addLayout(picinsert_2,1);

     picpart->setLayout(mainlayout);

     return picpart;
}
QGroupBox*dialrecognition::vedioinsert()
{

    vediopart=new QGroupBox(QString::fromLocal8Bit("视频读取"));
    vediopart->setStyleSheet("color:white");
   startcam=new QPushButton(QString::fromLocal8Bit("打开摄像头"));
    startcam->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset;font:26pt}" // 按键本色
                        "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                       "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
    startcam->setMinimumHeight(80);
    connect(startcam,SIGNAL(clicked(bool)),this,SLOT(usevedio()));

    stopall=new QPushButton("关闭系统");
    stopall->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset;font:26pt}" // 按键本色
                           "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                          "QPushButton:pressed{background-color:gray; border-style: inset }");   // 鼠标按下的色彩
    stopall->setMinimumHeight(80);
    connect(stopall,SIGNAL(clicked(bool)),this,SLOT(stopallwindow()));





    vediopartgrid=new QGridLayout;
    vediopartgrid->addWidget(startcam,0,0);
    vediopartgrid->addWidget(stopall,0,1);

    vediopart->setLayout(vediopartgrid);
    return vediopart;

}
void dialrecognition::stopallwindow()
{
    close();
}
void dialrecognition::usevedio()
{
    testTimer=new QTimer;
    qDebug()<<"ok";
    camerav=new QCamera("/dev/video0",this);
   //设置拍照像素为800x600,以防止图片质量过高，计算过慢
    begintest=new QPushButton(QString::fromLocal8Bit("开始识别"));
    begintest->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset;font:26pt}" // 按键本色
                             "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                             "QPushButton:pressed{background-color:gray; border-style: inset }");
    begintest->setMinimumHeight(60);
    connect(begintest,SIGNAL(clicked(bool)),this,SLOT(beginbt()));

    endtest=new QPushButton(QString::fromLocal8Bit("停止识别"));
    endtest->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset;font:26pt}" // 按键本色
                           "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                           "QPushButton:pressed{background-color:gray; border-style: inset }");
    endtest->setMinimumHeight(60);
    connect(endtest,SIGNAL(clicked(bool)),this,SLOT(stopbt()));

    QPushButton *holdon=new QPushButton(QString::fromLocal8Bit("暂停"));
    holdon->setStyleSheet("QPushButton{border:2px groove gray; border-radius:10px;padding:6px 4px;color:white;background-color:transparent;border-style: outset;font:26pt}" // 按键本色
                          "QPushButton:hover{background-color:green; color:white}"  // 鼠标停放时的色彩
                          "QPushButton:pressed{background-color:gray; border-style: inset }");
    holdon->setMinimumHeight(60);
    connect(holdon,SIGNAL(clicked(bool)),this,SLOT(holdonbt()));

    QCameraViewfinderSettings set;
    set.setResolution(800,600);
    camerav->setViewfinderSettings(set);
    cameraViewFinderv = new QCameraViewfinder();
    cameraImageCapturev = new QCameraImageCapture(camerav);
    startcam->close();
    stopall->close();



    infor22=new QTextEdit;
    infor22->setStyleSheet("background-color:white;color:black;font:12pt");
    infor22->document()->setMaximumBlockCount(12);
    infor22->setReadOnly(true);
    infor11=new QTextEdit;
    infor11->setStyleSheet("background-color:white;color:black;font:12pt");
    infor11->document()->setMaximumBlockCount(15);
    infor11->setReadOnly(true);
//以下一段if，先判断系统是否支持向内存存放，后将拍到的图片放置在内存中
    if(cameraImageCapturev->isCaptureDestinationSupported(QCameraImageCapture::CaptureToBuffer))
    {
        cameraImageCapturev->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
        qDebug()<<cameraImageCapturev->supportedBufferFormats();
        cameraImageCapturev->setBufferFormat(QVideoFrame::PixelFormat::Format_Jpeg);
        qDebug()<<cameraImageCapturev->supportedResolutions(cameraImageCapturev->encodingSettings());

        QImageEncoderSettings imageset;
        imageset.setResolution(800,600);
        cameraImageCapturev->setEncodingSettings(imageset);
        connect(cameraImageCapturev,&QCameraImageCapture::imageCaptured,this,&dialrecognition::processCapturedImage);
    }
    //以下连接函数为连接定时器函数
    connect(testTimer,SIGNAL(timeout()),this,SLOT(timerDone()));

    vediopartgrid->addWidget(cameraViewFinderv,0,0,2,6);
    vediopartgrid->addWidget(begintest,2,0,1,2);
    vediopartgrid->addWidget(holdon,2,2,1,2);
    vediopartgrid->addWidget(endtest,2,4,1,2);
    vediopartgrid->addWidget(infor11,4,0,1,3);
    vediopartgrid->addWidget(infor22,4,3,1,3);
    vediopartgrid->setRowStretch(0,5);
    vediopartgrid->setRowStretch(2,2);
    vediopartgrid->setRowStretch(4,3);
//    cameraImageCapturev->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    camerav->setCaptureMode(QCamera::CaptureStillImage);
    camerav->setViewfinder(cameraViewFinderv);
    camerav->start();//启动摄像头
    vediopart->setLayout(vediopartgrid);
}
//以下四个部分分别为开始识别、暂停、停止按钮及定时函数
void dialrecognition::beginbt()
{
     testTimer->start(10);
}

void dialrecognition::holdonbt()
{
    if(testTimer->isActive())
    testTimer->stop();
}

void dialrecognition::stopbt()
{
    if(testTimer->isActive())
    testTimer->stop();
    camerav->stop();

    QLayoutItem *child;
    while(( child=vediopartgrid->takeAt(0))!=0)
    {
        if(child->widget())
        {
            child->widget()->setParent(NULL);
        }
        delete child;
    }
    vediopartgrid->setRowStretch(0,1);
    vediopartgrid->setRowStretch(2,1);
    vediopartgrid->setRowStretch(4,1);
    vediopartgrid->addWidget(startcam,2,0);
    vediopartgrid->addWidget(stopall,2,1);
    startcam->show();
    stopall->show();
    vediopart->setLayout(vediopartgrid);

}

void dialrecognition::timerDone()
{
   cameraImageCapturev->capture();
}
//以下部分通过opencv对图像进行处理工作
void dialrecognition::processCapturedImage(int request_id,const QImage &img)
{
    qDebug()<<"processCapturedImage"<<request_id;
//    qDebug()<<img;
    float kxd=0;
    float valur=0;
    int separation=10;
    int interval=360/separation;
    float temp,sdv,edv,svv,evv;
    float l0=0.0,l1=0.0,l2=0.0,l3=0.0;
    float x_angle,y_angle;
    float x=0,y=0,r=0;

    float **p=new float*[interval];
    for(int i=0;i<interval;i++)
        p[i]=new float[2];

    float **p2=new float*[interval];
    for(int i=0;i<interval;i++)
        p2[i]=new float[2];

    float **txt=new float*[interval];
    for(int i=0;i<interval;i++)
        txt[i]=new float[2];

    Mat testmat,srcGray,src,forline1,srcGray2,forline;
    testmat=Mat(img.height(),img.width(),CV_8UC4,(void*)img.constBits(),img.bytesPerLine());
    src=testmat.clone();
    srcGray2=testmat.clone();
    forline1=testmat.clone();
    cvtColor(testmat,srcGray,6);
    threshold(srcGray,srcGray,twovalue_1->text().toInt(),255,1);
    //    imshow("test",srcGray);
    GaussianBlur(srcGray,srcGray,Size(9,9),2,2);

    std:: vector<Vec3f>circles;
    HoughCircles(srcGray,circles,HOUGH_GRADIENT,1.5,80,200,150,0,0);
    //以下for找出圆点与半径并画出圆与圆心
    for(size_t i=0;i<circles.size();i++)
    {
        Point center(cvRound(circles[i][0]),cvRound(circles[i][1]));
        int radius=cvRound(circles[i][2]);
        circle(src,center,3,Scalar(0,255,0),-1,8,0);//画出所识别圆的圆心
        circle(src,center,radius,Scalar(155,50,255),3,8,0);//画出所识别的圆
        x=center.x;
        y=center.y;
        r=radius;
    }
    qDebug()<<"x:"<<x<<".   "<<"y:"<<y<<".   "<<"r:"<<r<<".";
    QString a=QString::fromLocal8Bit("**********************")+QTime::currentTime().toString("hh:mm:ss")+"\n"+
            QString::fromLocal8Bit("x值为：")+QString::number(x)+
            QString::fromLocal8Bit("  y值为：")+QString::number(y)+
            QString::fromLocal8Bit("  r值为： ")+QString::number(r)+
            QString::fromLocal8Bit(".")+"\n";

    for(int i=0;i<interval;i++)
    {
        for(int j=0;j<2;j++)
        {
            if (j%2==0)
                p[i][j]=x+0.9*r*cos(separation*i*3.14/180);
            else
                p[i][j]=y+0.9*r*sin(separation*i*3.14/180);
        }
    }
    //找出第二个点
    int text_offset_x=20;
    int text_offset_y=5;
    for(int i=0;i<interval;i++)
    {
        for(int j=0;j<2;j++)
        {
            if (j%2==0)
            {
                p2[i][j]=x+r*cos(separation*i*3.14/180);
                txt[i][j]=x-text_offset_x+1.2*r*cos(separation*(i+9)*3.14/180);
            }
            else
            {
                p2[i][j]=y+r*sin(separation*i*3.14/180);
                txt[i][j]=y+text_offset_y+1.2*r*sin(separation*(i+9)*3.14/180);
            }
        }
    }
    //以下为点输出测试
    for(int i=0;i<interval;i++)
    {
        line(src,Point(int(p[i][0]),int(p[i][1])),Point(int(p2[i][0]),int(p2[i][1])),Scalar(0,255,0),2);
        putText(src,std::to_string(i*separation),Point(int(txt[i][0]),int(txt[i][1])),FONT_HERSHEY_SIMPLEX,0.4,Scalar(0,0,255),1,8);

    }
    //     imshow("test",src);
    /////////////////////////////////////////////////////
    infor11->append(a);

    if((x!=0)&&(r!=0))
    {
        int num=0;
        int minLineLengh=20;
        int maxLineGap=0;
        int twovalue2=twovalue_2->text().toInt();

        cvtColor(forline1,forline,6);
        threshold(forline,forline,twovalue2,255,1);
        std::vector<Vec4f>lines;
        HoughLinesP(forline,lines,1,CV_PI/180.0,60,minLineLengh,maxLineGap);
        Scalar color=Scalar(0,255,0);
        for(size_t i=0;i<lines.size();i++)
        {
            Vec4f l=lines[i];
            //               if (((l[1]-ey)/(l[0]-ex))/((l[3]-ey)/(l[2]-ex))<1.01)
            float diff1=jisuan(x,y,l[0],l[1]);
            float diff2=jisuan(x,y,l[2],l[3]);
            if (diff1>diff2)
            {
                temp=diff1;
                diff1=diff2;
                diff2=temp;
            }
            if((diff1<0.25*r) && (diff1>0.15*r)&& (diff2<1*r) && (diff2>0.5*r)&&(getD(x,y,(l[3]-l[1]),(l[0]-l[2]),((l[1]*l[2])-(l[0]*l[3])))<0.5))
            {
                line(src,Point(l[0],l[1]),Point(l[2],l[3]),color,1,LINE_AA);
                //          line(src1,Point(l[0],l[1]),Point(ex,ey),color,1,LINE_AA);
                //                    qDebug()<<getD(x,y,(l[3]-l[1]),(l[0]-l[2]),((l[1]*l[2])-(l[0]*l[3])));//输出圆点到所画线的距离
                //                    qDebug()<<l[0]<<" "<<l[1]<<"        "<<l[2]<<" "<<l[3];//输出说话线的两个点
                //                    &&(getD(x,y,(l[3]-l[1]),(l[0]-l[2]),((l[1]*l[2])-(l[0]*l[3])))<0.5)
                l0=l[0];l1=l[1];l2=l[2];l3=l[3];
                kxd=getD(x,y,(l[3]-l[1]),(l[0]-l[2]),((l[1]*l[2])-(l[0]*l[3])));
                num++;
            }
        }

        if(num==1)
        {
            sdv=startdegree->text().toFloat();
            edv=enddegree->text().toFloat();
            svv=startvalue->text().toFloat();
            evv=endvalue->text().toFloat();

            float dir1=jisuan(x,y,l0,l1);
            float dir2=jisuan(x,y,l2,l3);
            if(dir1>dir2)
            {
                x_angle=l0-x;
                y_angle=y-l1;
            }
            else
            {
                x_angle=l2-x;
                y_angle=y-l3;//注意，这边并不是l3-y，因为图片中纵坐标是向下变大，与平常坐标相反
            }
            float tanvalue=y_angle/x_angle;
            float hudu=atan(tanvalue);
            float jiaodu=hudu*180/CV_PI;
            float finalvalue=0;

            if((x_angle>0)&&(y_angle>0))
                finalvalue=270.0-(jiaodu);
            if((x_angle>0)&&(y_angle<0))
                finalvalue=270.0-(jiaodu);
            if((x_angle<0)&&(y_angle>0))
                finalvalue=90.0-(jiaodu);
            if((x_angle<0)&&(y_angle<0))
                finalvalue=90.0-(jiaodu);
            if((x_angle=0)&&(y_angle>0))
                finalvalue=180;

            valur=((finalvalue-sdv)*(evv-svv)/(edv-sdv)+svv);
//            qDebug()<<"tanvalue:"<<tanvalue<<"       hudu:"<<hudu<<"       jiaodu:"<<jiaodu<<"      finalvalur:"<<finalvalue;
//            qDebug()<<"Final value"<<valur;

            if((egvalue-5<valur)&&(egvalue+5>valur))
            {
//            getavg[4]=getavg[3];
//            getavg[3]=getavg[2];
//            getavg[2]=getavg[1];
//            getavg[1]=getavg[0];
//            getavg[0]=valur;

//            int jishu=0;
//            float qiuhe=0.0;
//            for(int i1=0;i1<5;i1++)
//            {
//                if(getavg[i1]<130)
//                {
//                   qiuhe=qiuhe+getavg[i1];
//                   jishu++;
//                }
//            }



            QString va=QString::number(valur,'f',2);
            QString b=QString::fromLocal8Bit("**********************")+QTime::currentTime().toString("hh:mm:ss")+"\n"+"\n"
                    +QString::fromLocal8Bit("识别结果为：")+va+QString::fromLocal8Bit("  ℃;")+QString::fromLocal8Bit("可信度：")+QString::number((10.0-kxd)*10.0,'f',3)+QString::fromLocal8Bit("  %.")+"\n";

            valueshow->setText(va);
            infor22->append(b);
        }
        }

        for(int i=0;i<interval;i++)
        {
            delete []p[i];
        }
        delete [] p;
        for(int i=0;i<interval;i++)
        {
            delete []p2[i];
        }
        delete [] p2;
        for(int i=0;i<interval;i++)
        {
            delete []txt[i];
        }
        delete []txt;
    }
    waitKey(0);
}


void dialrecognition::handlelineeditclicked()
{
    QString combobox21=twovalue_3->currentText();
    if(combobox21=="CV_THRESH_BINARY=0") v3=0;
    if(combobox21=="CV_THRESH_BINARY_INV=1") v3=1;
   filename.clear();
   QWidget *qwidget=new QWidget();
   filename=QFileDialog::getOpenFileName(qwidget,QString::fromLocal8Bit("选择图片"),"","Image Files(*.jpg *.png)");
   editphotolineeidt->setText(filename);
}
void dialrecognition::takephoto()
{
    editphotolineeidt->clear();
    filename.clear();
    takephotos *w=new takephotos;
    w->show();
}
void dialrecognition::editphotos()
{
    if(filename=="")
        QMessageBox::warning(0,QString::fromLocal8Bit("图片信息有误"),QString::fromLocal8Bit("请拍照或通过有效路径\n添加原图片"),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    else {
        QPixmap src;
        src.load(filename);
        originpic->setScaledContents(true);
        originpic->setMaximumSize(originpic->size());
        originpic->setPixmap(src);

    }


}
void dialrecognition::getgraypics()
{
    //opencv中灰度处理图片，使用函数：cvtColor(InportArray src,OutputArray dst,int code,int dstCn=0)
    //前两个分别表示图片输入、输出，第三个表示颜色空间转换标识符，第四个是图片转换通道数

    Mat Dgray;
    Mat org;
    if(filename=="")
        QMessageBox::warning(0,QString::fromLocal8Bit("图片信息有误"),QString::fromLocal8Bit("原图片未导入\n或路径有误"),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    else
    {
    org=imread(filename.toStdString());
    cvtColor(org,Dgray,CV_BGR2GRAY);
    imwrite("pic/graydpic.jpg",Dgray);
    QPixmap graypicshow;
    graypicshow.load("pic/graydpic.jpg");
    graypic->setScaledContents(true);
    graypic->setMaximumSize(graypic->size());
    graypic->setPixmap(graypicshow);

    waitKey(0);
    }
}
//以下根据twovalue_1获取圆或线的阈值，进行二值化处理
void dialrecognition::bt2_0_0()
{
    QString combobox21=twovalue_3->currentText();
    if(combobox21=="CV_THRESH_BINARY=0") v3=0;
    if(combobox21=="CV_THRESH_BINARY_INV=1") v3=1;
   Mat src2,dst2;
    int v1,v2,v3;
    v1=twovalue_1->text().toInt();
    v2=twovalue_2->text().toInt();

    if(v1==0)
    {
        QMessageBox::warning(0,QString::fromLocal8Bit("注意"),QString::fromLocal8Bit("请输入阀值！"),QMessageBox::Yes|QMessageBox::No);
    }
    else
    {
        v2=255;

        QString combobox21=twovalue_3->currentText();
        if(combobox21=="CV_THRESH_BINARY=0") v3=0;
        if(combobox21=="CV_THRESH_BINARY_INV=1") v3=1;
        if(combobox21=="CV_THRESH_TRUNC=2") v3=2;
        if(combobox21=="CV_THRESH_TOZERO=3") v3=3;
        if(combobox21=="CV_THRESH_TOZERO_INV=4") v3=4;
        if(combobox21=="CV_THRESH_MASK=7") v3=7;
        if(combobox21=="CV_THRESH_OTSU=8") v3=8;
        if(combobox21=="CV_THRESH_TRIANGLE=16") v3=16;
        src2=imread("pic/graydpic.jpg");
        threshold(src2,dst2,v1,v2,v3);
        imwrite("pic/tvc.jpg",dst2);
        QPixmap tvc;
        tvc.load("pic/tvc.jpg");
        twovaluecycle->setMaximumSize(twovaluecycle->size());
        twovaluecycle->setScaledContents(true); 
        twovaluecycle->setPixmap(tvc);




    }

}
void dialrecognition::bt2_0_1()
{
    QString combobox21=twovalue_3->currentText();
    if(combobox21=="CV_THRESH_BINARY=0") v3=0;
    if(combobox21=="CV_THRESH_BINARY_INV=1") v3=1;
    Mat src,dst;
     int v1,v2,v3;
     v1=twovalue_1->text().toInt();
     v2=twovalue_2->text().toInt();

     if(v1==0)
     {
         QMessageBox::warning(0,QString::fromLocal8Bit("注意"),QString::fromLocal8Bit("请输入阀值！"),QMessageBox::Yes|QMessageBox::No);
     }
     else
     {
         if (v2==0)v2=255;

         QString combobox21=twovalue_3->currentText();
         if(combobox21=="CV_THRESH_BINARY=0") v3=0;
         if(combobox21=="CV_THRESH_BINARY_INV=1") v3=1;
         if(combobox21=="CV_THRESH_TRUNC=2") v3=2;
         if(combobox21=="CV_THRESH_TOZERO=3") v3=3;
         if(combobox21=="CV_THRESH_TOZERO_INV=4") v3=4;
         if(combobox21=="CV_THRESH_MASK=7") v3=7;
         if(combobox21=="CV_THRESH_OTSU=8") v3=8;
         if(combobox21=="CV_THRESH_TRIANGLE=16") v3=16;
         src=imread("pic/graydpic.jpg");
         threshold(src,dst,v1,255,v3);
         imwrite("pic/tvl.jpg",dst);
         QPixmap tvl;
         tvl.load("pic/tvl.jpg");
         twovalueline->setMaximumSize(twovaluecycle->size());
         twovalueline->setScaledContents(true);
         twovalueline->setPixmap(tvl);
     }
}

//这部分为空
void dialrecognition::bt2_1_0()
{

}
void dialrecognition::bt2_1_1()
{ }
void dialrecognition::bt2_2_0()
{

}
void dialrecognition::bt2_2_1()
{

}

/////////////局部阈值自适应处理
void dialrecognition::bt2_3_0()
{
if(filename=="")
    QMessageBox::warning(0,QString::fromLocal8Bit("注意！"),QString::fromLocal8Bit("请生成灰度图后再进行此操作"));
else
{
    int blocksize=3;
    int constvalue=4;
    int v3;
//    QString combobox21=twovalue_3->currentText();
//    if(combobox21=="CV_THRESH_BINARY=0") v3=0;
//    if(combobox21=="CV_THRESH_BINARY_INV=1") v3=1;
    const int maxval=255;
    Mat src,dst;
    src=imread("pic/graydpic.jpg",0);
    adaptiveThreshold(src,dst,
                      maxval,1,
                      v3,blocksize,
                      constvalue);
    imwrite("pic/tvc.jpg",dst);
    imwrite("pic/tvl.jpg",dst);
    QPixmap tvc;
    tvc.load("pic/tvc.jpg");
    twovaluecycle->setMaximumSize(twovaluecycle->size());
    twovaluecycle->setScaledContents(true);
    twovaluecycle->setPixmap(tvc);

    QPixmap tvl;
    tvl.load("pic/tvl.jpg");
    twovalueline->setMaximumSize(twovalueline->size());
    twovalueline->setScaledContents(true);
    twovalueline->setPixmap(tvl);
    waitKey(0);
}
}
/////一个空白
void dialrecognition::bt2_3_1()
{

}
void dialrecognition::bt2_4_0()
{
    QString combobox21=twovalue_3->currentText();
    if(combobox21=="CV_THRESH_BINARY=0") v3=0;
    if(combobox21=="CV_THRESH_BINARY_INV=1") v3=1;
    IplImage* src; //声明IplImage指针
    //载入图像
    if(
        (src = cvLoadImage( "pic/graydpic.jpg",0))!= 0 )
    {
        IplImage* dst = cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
        MaxEntropy(src,dst);
//        cvShowImage( "Image", dst );//显示图像


        Mat dst2=cvarrToMat(dst);

        imwrite("pic/tvc.jpg",dst2);
        imwrite("pic/tvl.jpg",dst2);
        QPixmap tvc;
        tvc.load("pic/tvc.jpg");
        twovaluecycle->setMaximumSize(twovaluecycle->size());
        twovaluecycle->setScaledContents(true);
        twovaluecycle->setPixmap(tvc);

        QPixmap tvl;
        tvl.load("pic/tvl.jpg");
        twovalueline->setMaximumSize(twovalueline->size());
        twovalueline->setScaledContents(true);
        twovalueline->setPixmap(tvl);

        QString aa = QString::number(selfvalue, 10);
        thresh_show->setText(aa);
//        qDebug()<<QString(selfvalue);



        cvWaitKey(0); //等待按键
        cvReleaseImage( &src ); //释放图像
        cvReleaseImage( &dst ); //释放图像


    }


}
//一个空白
void dialrecognition::bt2_4_1()
{

}
void dialrecognition::gettvself()//该部分，通过手动调节滑条找到适合二值化的阈值
{
    getthresholdvalue *w=new getthresholdvalue;
    w->show();
    delete []w;
}

//以下为最高熵阈值法
int QueryHistValue_1D(CvHistogram* hist,int i)
{
    return 1;
}
double caculateCurrentEntropy(CvHistogram * Histogram1,int cur_threshold,entropy_state state)
{
    int start,end;
    if(state == back)   {
        start = 0;end = cur_threshold;
    }
    else    {
        start = cur_threshold;end = 256;
    }
    int  total = 0;
    for(int i=start;i<end;i++)   {
        total += (int)cvQueryHistValue_1D(Histogram1,i);
    }
    double cur_entropy = 0.0;
    for(int i=start;i<end;i++){
        if((int)cvQueryHistValue_1D(Histogram1,i)==0)
            continue;
        double percentage = cvQueryHistValue_1D(Histogram1,i)/total;
         cur_entropy += -percentage*logf(percentage);
    }
    return cur_entropy;
}
void  MaxEntropy(IplImage *src,IplImage *dst)
{
    assert(src != NULL);
    assert(src->depth == 8 && dst->depth == 8);
    assert(src->nChannels == 1);
    CvHistogram * hist  = cvCreateHist(1,&HistogramBins,CV_HIST_ARRAY,HistogramRange);
    cvCalcHist(&src,hist);
    double maxentropy = -1.0;
    int max_index = -1;
    for(int i=0;i<HistogramBins;i++) {
        double cur_entropy = caculateCurrentEntropy(hist,i,object)+caculateCurrentEntropy(hist,i,back);
        if(cur_entropy>maxentropy){
            maxentropy = cur_entropy;
            max_index = i;
        }
    }

    cvThreshold(src,dst,(double)max_index,255,v3);
    cvReleaseHist(&hist);
    selfvalue=max_index;
}
//以下分别画出圆、直线
void dialrecognition::play3_1()
{
    recganddraw w;
    w.showl();
    QPixmap tvl;
    tvl.load("pic/drawcircle.jpg");
    drawcycle->setMaximumSize(drawcycle->size());
    drawcycle->setScaledContents(true);
    drawcycle->setPixmap(tvl);
}
//以下部分实际是找线找圆的综合，但只利用了提取出的数据，未在此画图
float dialrecognition::getD(float x,float y,float a,float b,float c)
{
    float fz=a*x+b*y+c;
    if(fz<0)
        fz=fz*(-1);
    float sqrtv=sqrt(a*a+b*b);
    return  (fz/sqrtv);

}
//计算两个点之间的距离
float dialrecognition::jisuan(float x,float y,float x2,float y2)
{
    return sqrt((x2-x)*(x2-x)+(y2-y)*(y2-y));
}

void dialrecognition::testbutton()
{
    int tvalue=0;
    int jishu=0;
    float valur=0;
    qDebug()<<"OK";
    if(filename=="")
        QMessageBox::warning(0,QString::fromLocal8Bit("注意"),QString::fromLocal8Bit("请选择图片"),QMessageBox::Yes);
    else
    {
            float temp,sdv,edv,svv,evv;
            float l0,l1,l2,l3;
            float x_angle,y_angle;
            float x=0,y=0,r=0;
            int separation=10;
            int interval=360/separation;

            float **p=new float*[interval];
            for(int i=0;i<interval;i++)
                p[i]=new float[2];

            float **p2=new float*[interval];
            for(int i=0;i<interval;i++)
                p2[i]=new float[2];

            float **txt=new float*[interval];
            for(int i=0;i<interval;i++)
                txt[i]=new float[2];
            while((jishu!=1)&&(tvalue<200))
            {
            Mat srcGray,srcGray2;
            srcGray=imread("pic/tvc.jpg",0);
            srcGray2=imread(filename.toStdString());
            GaussianBlur(srcGray,srcGray,Size(9,9),2,2);

            std:: vector<Vec3f>circles;
            HoughCircles(srcGray,circles,HOUGH_GRADIENT,1.5,80,200,150,0,0);
            //以下for找出圆点与半径并画出圆与圆心
            for(size_t i=0;i<circles.size();i++)
            {
                Point center(cvRound(circles[i][0]),cvRound(circles[i][1]));
                int radius=cvRound(circles[i][2]);

                circle(srcGray2,center,3,Scalar(0,255,0),-1,8,0);//画出所识别圆的圆心
                circle(srcGray2,center,radius,Scalar(155,50,255),3,8,0);//画出所识别的圆

                x=center.x;
                y=center.y;
                r=radius;
            }

            int minLineLengh=20;
            int maxLineGap=0;
            Mat src,src1;
            src=imread("pic/graydpic.jpg",0);
            threshold(src,src,tvalue,255,1);

            src1=imread(filename.toStdString());
            std::vector<Vec4f>lines;
            HoughLinesP(src,lines,1,CV_PI/180.0,60,minLineLengh,maxLineGap);
            Scalar color=Scalar(0,255,0);
            for(size_t i=0;i<lines.size();i++)
            {
                Vec4f l=lines[i];
                //               if (((l[1]-ey)/(l[0]-ex))/((l[3]-ey)/(l[2]-ex))<1.01)
                float diff1=jisuan(x,y,l[0],l[1]);
                float diff2=jisuan(x,y,l[2],l[3]);
                if (diff1>diff2)
                {
                    temp=diff1;
                    diff1=diff2;
                    diff2=temp;
                }
                if((diff1<0.25*r) && (diff1>0.15*r) && (diff2<1*r) && (diff2>0.5*r)&&(getD(x,y,(l[3]-l[1]),(l[0]-l[2]),((l[1]*l[2])-(l[0]*l[3])))<0.5))
                {
                    line(src1,Point(l[0],l[1]),Point(l[2],l[3]),color,1,LINE_AA);
                    jishu++;
                    //          line(src1,Point(l[0],l[1]),Point(ex,ey),color,1,LINE_AA);
                    qDebug()<<getD(x,y,(l[3]-l[1]),(l[0]-l[2]),((l[1]*l[2])-(l[0]*l[3])));//输出圆点到所画线的距离
                    qDebug()<<l[0]<<" "<<l[1]<<"        "<<l[2]<<" "<<l[3];//输出说话线的两个点
                    imwrite("pic/latestline.jpg",src1);
                    l0=l[0];l1=l[1];l2=l[2];l3=l[3];
                }
            }
            tvalue++;
            qDebug()<<x<<"  "<<y<<" "<<r;
            qDebug()<<QString::fromLocal8Bit("识别出直线：")<<jishu;
            qDebug()<<QString::fromLocal8Bit("此次计算使用的阈值为：")<<tvalue-1;
            qDebug()<<"-----------------------";


            }
            if(jishu==1)
            {
                sdv=startdegree->text().toFloat();
                edv=enddegree->text().toFloat();
                svv=startvalue->text().toFloat();
                evv=endvalue->text().toFloat();

                float dir1=jisuan(x,y,l0,l1);
                float dir2=jisuan(x,y,l2,l3);
                if(dir1>dir2)
                {
                    x_angle=l0-x;
                    y_angle=y-l1;
                }
                else
                {
                    x_angle=l2-x;
                    y_angle=y-l3;//注意，这边并不是l3-y，因为图片中纵坐标是向下变大，与平常坐标相反
                }
                float tanvalue=y_angle/x_angle;
                float hudu=atan(tanvalue);
                float jiaodu=hudu*180/CV_PI;
                float finalvalue=0;

                if((x_angle>0)&&(y_angle>0))
                    finalvalue=270.0-(jiaodu);
                if((x_angle>0)&&(y_angle<0))
                    finalvalue=270.0-(jiaodu);
                if((x_angle<0)&&(y_angle>0))
                    finalvalue=90.0-(jiaodu);
                if((x_angle<0)&&(y_angle<0))
                    finalvalue=90.0-(jiaodu);
                if((x_angle=0)&&(y_angle>0))
                    finalvalue=180;

                //        if(x_angle>0)
                //            finalvalue=270-jiaodu;
                //        else
                //            finalvalue=90-jiaodu;

                valur=((finalvalue-sdv)*(evv-svv)/(edv-sdv)+svv);
                qDebug()<<"tanvalue:"<<tanvalue<<"       hudu:"<<hudu<<"       jiaodu:"<<jiaodu<<"      finalvalur:"<<finalvalue;
                qDebug()<<"Final value"<<valur;
                QString va=QString::number(valur,10.0);
                egvalue=valur;
                valueshow->setText(va);
            }
            else
                QMessageBox::warning(0, QString::fromLocal8Bit("提醒"),QString::fromLocal8Bit("未识别出合适直线，请调整参数"),QMessageBox::Yes);

            for(int i=0;i<interval;i++)
            {
                delete []p[i];
            }
           delete [] p;
             for(int i=0;i<interval;i++)
             {
                 delete []p2[i];
             }
           delete [] p2;
              for(int i=0;i<interval;i++)
              {
                  delete []txt[i];
              }
           delete []txt;
                QPixmap tvl2;
                tvl2.load("pic/latestline.jpg");
                drawline->setMaximumSize(drawline->size());
                drawline->setScaledContents(true);
                drawline->setPixmap(tvl2);


        waitKey(0);

    }
}
void dialrecognition::justfortest()
{

}
