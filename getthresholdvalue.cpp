#include "getthresholdvalue.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include"dialrecognition.h"
using namespace cv;
int threshold_value = 0;
int threshold_type = 1;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;
Mat src, src_gray, dst;
const char* window_name ="阈值查看器";
const char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
//const char* trackbar_type = "Type: \n 0: Binary ; 1: Binary Inverted ; \n 2: Truncate ; 3: To Zero ; 4: To Zero Inverted";
const char* trackbar_value = "Value";
void Threshold_Demo( int, void* );

getthresholdvalue::getthresholdvalue(QWidget *parent):
    QDialog(parent)
{

    qDebug()<<filename;
    QPixmap test;

    if(!test.load("pic/graydpic.jpg"))
    {
        QMessageBox::warning(0,QString::fromLocal8Bit("图片打开失败"),QString::fromLocal8Bit("图片不存在或路径有误"));
    }
    else if(filename=="")
    {
        QMessageBox::information(0,QString::fromLocal8Bit("提醒"),QString::fromLocal8Bit("请导入原图片"));
    }
        else
    {

        src = imread( "pic/graydpic.jpg" ); // Load an image
        cv::resize(src,src,Size(600,450),0,0,INTER_LINEAR);
        cvtColor( src, src_gray, COLOR_BGR2GRAY ); // Convert the image to Gray
        namedWindow( window_name, WINDOW_AUTOSIZE ); // Create a window to display results
        Threshold_Demo( 0, 0 );
        createTrackbar( trackbar_type,
                        window_name, &threshold_type,
                        max_type, Threshold_Demo ); // Create Trackbar to choose type of Threshold
        createTrackbar( trackbar_value,
                        window_name, &threshold_value,
                        max_value, Threshold_Demo ); // Create Trackbar to choose Threshold value
//        Threshold_Demo( 0, 0 ); // Call the function to initialize
        for(;;)
        {
            char c = (char)waitKey( 20 );
            if( c == 27 )
            { break; }
        }
        waitKey(0);
    }
}
void Threshold_Demo( int, void* )
{
  /* 0: Binary
     1: Binary Inverted
     2: Threshold Truncated
     3: Threshold to Zero
     4: Threshold to Zero Inverted
   */
  threshold( src_gray, dst, threshold_value, max_BINARY_value,threshold_type );
  imshow( window_name, dst );
}
