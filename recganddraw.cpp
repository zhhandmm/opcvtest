#include "recganddraw.h"
#include <QDebug>
#include "dialrecognition.h"
#include "math.h"
#include<QMessageBox>
#include <QPixmap>
using namespace std;
using namespace cv;
 float ex;
 float ey;
 float er;
recganddraw::recganddraw()
{

}
void recganddraw::showl()
{
    if(filename=="")
        QMessageBox::warning(0,QString::fromLocal8Bit("注意"),QString::fromLocal8Bit("请选择图片"),QMessageBox::Yes);
    else
    {
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

        Mat srcGray,srcGray2;
        srcGray=imread("pic/tvc.jpg",0);
        srcGray2=imread(filename.toStdString());


        GaussianBlur(srcGray,srcGray,Size(9,9),2,2);

        vector<Vec3f>circles;
        HoughCircles(srcGray,circles,HOUGH_GRADIENT,1.5,80,200,150,0,0);
        //以下for找出圆点与半径并画出圆与圆心
        for(size_t i=0;i<circles.size();i++)
        {
            Point center(cvRound(circles[i][0]),cvRound(circles[i][1]));
            int radius=cvRound(circles[i][2]);

            circle(srcGray2,center,3,Scalar(0,255,0),-1,8,0);
            circle(srcGray2,center,radius,Scalar(155,50,255),3,8,0);

            x=center.x;
            y=center.y;
            r=radius;

            ex=center.x;
            ey=center.y;
            er=radius;
        }

        //以下for找出画线第一个点
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
        int text_offset_x=10;
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
            line(srcGray2,Point(int(p[i][0]),int(p[i][1])),Point(int(p2[i][0]),int(p2[i][1])),Scalar(0,255,0),2);
            putText(srcGray2,to_string(i*separation),Point(int(txt[i][0]),int(txt[i][1])),FONT_HERSHEY_SIMPLEX,0.4,Scalar(0,0,255),1,8);
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
//        imshow("test",srcGray2);
        imwrite("pic/drawcircle.jpg",srcGray2);
        waitKey(0);
    }
}

