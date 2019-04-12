#include "getline.h"
#include "QDebug"
#include <iostream>
#include "dialrecognition.h"
#include "QMessageBox"
#include "recganddraw.h"
#include "math.h"
using namespace cv;
using namespace std;
getline::getline()
{

}
float getline::getD(float x,float y,float a,float b,float c)
{
    float fz=a*x+b*y+c;
    if(fz<0)
        fz=fz*(-1);
    float sqrtv=sqrt(a*a+b*b);
    return  (fz/sqrtv);

}
//计算两个点之间的距离
float getline::jisuan(float x,float y,float x2,float y2)
{
    return sqrt((x2-x)*(x2-x)+(y2-y)*(y2-y));
}

void getline::getl()
{
    float temp;
    if(filename=="")
        QMessageBox::warning(0,QString::fromLocal8Bit("注意"),QString::fromLocal8Bit("请输入图片"),QMessageBox::Yes,QMessageBox::Yes);
    else
    {
        int minLineLengh=10;
        int maxLineGap=0;
        Mat src,src1;
        src=imread("pic\\tvl.jpg",0);
        src1=imread(filename.toStdString());
        vector<Vec4f>lines;
        HoughLinesP(src,lines,1,CV_PI/180.0,60,minLineLengh,maxLineGap);
        Scalar color=Scalar(0,255,0);
        for(size_t i=0;i<lines.size();i++)
        {
            Vec4f l=lines[i];
            //   if (((l[1]-ey)/(l[0]-ex))/((l[3]-ey)/(l[2]-ex))<1.01)
            float diff1=jisuan(ex,ey,l[0],l[1]);
            float diff2=jisuan(ex,ey,l[2],l[3]);
            if (diff1>diff2)
            {
                temp=diff1;
                diff1=diff2;
                diff2=temp;
            }
            if((diff1<0.25*er) & (diff1>0.15*er) & (diff2<1*er) & (diff2>0.5*er))
            {
                line(src1,Point(l[0],l[1]),Point(l[2],l[3]),color,1,LINE_AA);
                //          line(src1,Point(l[0],l[1]),Point(ex,ey),color,1,LINE_AA);
                qDebug()<<getD(ex,ey,(l[3]-l[1]),(l[0]-l[2]),((l[1]*l[2])-(l[0]*l[3])));//输出圆点到所画线的距离
                qDebug()<<l[0]<<" "<<l[1]<<"        "<<l[2]<<" "<<l[3];//输出说话线的两个点
            }
        }
        qDebug()<<ex<<"  "<<ey<<" "<<er;
        imwrite("pic\\getlinepic.jpg",src1);
//        waitKey(0);
    }
}
