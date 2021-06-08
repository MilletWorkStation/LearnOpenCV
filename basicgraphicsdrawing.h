#ifndef BASICGRAPHICSDRAWING_H
#define BASICGRAPHICSDRAWING_H

#include <opencv2/opencv.hpp>

#include <QFileDialog>
#include <QMessageBox>
using namespace cv;

/*
 * 基础图形绘制
 * 1、绘制直线：line
 * 2、绘制椭圆：ellipse
 * 3、绘制矩形：rectangle
 * 4、绘制圆形：circle
 * 5、绘制多边形填充：fillPoly
*/


void Draw();

// 顺时针 90°
void Tanspose();

///
// enum RotateFlags {
//     ROTATE_90_CLOCKWISE = 0,         //Rotate 90 degrees clockwise
//     ROTATE_180 = 1,                   //Rotate 180 degrees clockwise
//     ROTATE_90_COUNTERCLOCKWISE = 2, //Rotate 270 degrees clockwise
// };

void Rotate(const int flag);

// 参数一：输入mat
// 参数二：输出mat
// 参数只有3种值，小于0，等于0，大于0，分别对应xy轴翻转、x轴翻转、y轴翻转。
void Flip(const int flag);

//
// 该函数为缩放函数，特别注意：函数缩放会失真，尤其是以缩放后的图像继续缩放会原来大小，所以笔者建议，缩放用缓存，缓存存放原图，每次以原图缩放。
//
// CV_EXPORTS_W void resize( InputArray src, OutputArray dst,Size dsize, double fx = 0, double fy = 0,int interpolation = INTER_LINEAR );
//      参数一：输入mat
//      参数二：输出mat
//      参数三：缩放后的大小
//      参数四：x缩放比例，一般默认0就可以
//      参数五：y缩放比例，一般默认0就可以
//      参数六：差值，一般默认即可
//  enum InterpolationFlags{
//      INTER_NEAREST           = 0,	// 最临近差值
//      INTER_LINEAR            = 1,   // 双线性差值
//      INTER_CUBIC             = 2,   // 双立方差值
//      INTER_AREA              = 3,
//      INTER_LANCZOS4          = 4,  // 附近像素及原像素加权取值
//      INTER_LINEAR_EXACT      = 5,
//      INTER_MAX               = 7,
//      WARP_FILL_OUTLIERS      = 8,
//      WARP_INVERSE_MAP        = 16
//  };

void Resize(const int flag);

void BasicGraphicsDrawing()
{

    //Draw();

    // Rotate(ROTATE_90_CLOCKWISE);

    // Rotate(ROTATE_180);

    // Rotate(ROTATE_90_COUNTERCLOCKWISE);

//    Flip(-1);

//    Flip(0);

//    Flip(1);

//    Resize(cv::INTER_NEAREST);
    Resize(cv::INTER_LINEAR);
//    Resize(cv::INTER_CUBIC);
//    Resize(cv::INTER_AREA);
//    Resize(cv::INTER_LANCZOS4);
//    Resize(cv::INTER_LINEAR_EXACT);
//    Resize(cv::INTER_MAX);
//    Resize(cv::WARP_FILL_OUTLIERS);
//    Resize(cv::WARP_INVERSE_MAP);

}



void Draw()
{
    cv::Mat mat(400, 400, CV_8UC3, cv::Scalar());

    mat = cv::Scalar();
    cv::putText(mat, "Hello world!!!", cv::Point(0, 200), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 0, 255));

    // line
    cv::line(mat, cv::Point(30, 30) , cv::Point(370, 30) , cv::Scalar(255, 255, 255), 1);
    cv::line(mat, cv::Point(370, 30), cv::Point(370, 370), cv::Scalar(255, 255, 255), 1);
    cv::line(mat, cv::Point(370, 370), cv::Point(30, 370), cv::Scalar(255, 255, 255), 1);
    cv::line(mat, cv::Point(30, 370), cv::Point(30 , 30), cv::Scalar(255, 255, 255), 1);

    // ellipse
    cv::ellipse(mat, cv::Point(50+1, 50+1) , cv::Size(10, 20), 45.0, 0.0, 360.0,
                cv::Scalar(0, 0, 255), 1);
    cv::ellipse(mat, cv::Point(50+1, 350-1), cv::Size(10, 20), 135.0, 0.0, 360.0,
                cv::Scalar(0, 0, 255), -1);
    cv::ellipse(mat, cv::Point(350-1, 50+1), cv::Size(20, 10), 45.0, 0.0, 360.0,
                cv::Scalar(0, 0, 255), 1);
    cv::ellipse(mat, cv::Point(350-1, 350-1), cv::Size(20, 10), 135.0, 0.0, 360.0,
                cv::Scalar(0, 0, 255), -1);

    // rectangle
    cv::rectangle(mat, cv::Rect(100   , 100   , 20, 20), cv::Scalar(0, 255, 0));
    cv::rectangle(mat, cv::Rect(100   , 300-20, 20, 20), cv::Scalar(0, 255, 0));
    cv::rectangle(mat, cv::Rect(300-20, 100   , 20, 20), cv::Scalar(0, 255, 0), -1);
    cv::rectangle(mat, cv::Rect(300-20, 300-20, 20, 20), cv::Scalar(0, 255, 0), -1);

    // circle
    cv::circle(mat, cv::Point(200, 200), 10, cv::Scalar(200, 200, 200), -1);
    cv::circle(mat, cv::Point(200, 200), 20, cv::Scalar(200, 200, 200), 1);
    cv::circle(mat, cv::Point(200, 200), 30, cv::Scalar(200, 200, 200), 2);
    cv::circle(mat, cv::Point(200, 200), 40, cv::Scalar(200, 200, 200), 3);
    cv::circle(mat, cv::Point(200, 200), 50, cv::Scalar(200, 200, 200), 4);

    // fillPoly
    cv::Point rootPoints[1][4];
    rootPoints[0][0] = cv::Point(200, 150);
    rootPoints[0][1] = cv::Point(250, 200);
    rootPoints[0][2] = cv::Point(200, 250);
    rootPoints[0][3] = cv::Point(150, 200);
    const cv::Point * ppt[1] = { rootPoints[0] };
    const int npt[] = {4};
    cv::fillPoly(mat, ppt, npt, 1, cv::Scalar(0, 255, 255));

    cv::imshow("1", mat);
}

void Tanspose()
{
    Mat mat = imread("F:/lena.jpg");

    if( mat.data == NULL )
    {
        QMessageBox::warning(nullptr, "错误", "载入图片错误");
        return;
    }

    imshow("raw" , mat );

    cv::transpose(mat, mat);
    imshow("transpose" , mat );
    resizeWindow("transpose", 800, 800);
}

void Rotate(const int flag)
{
    Mat mat = imread("F:/lena.jpg");

    if( mat.data == NULL )
    {
        QMessageBox::warning(nullptr, "错误", "载入图片错误");
        return;
    }

    imshow("raw" , mat );

    QString strFlag;
    if( flag == 0)
        strFlag = "ROTATE_90_CLOCKWISE ";
    if( flag == 1)
        strFlag = "ROTATE_180  ";
    if( flag == 2)
        strFlag = "ROTATE_90_COUNTERCLOCKWISE  ";

    QString strTitle = QString("rotate(%1:%2)").arg(flag).arg(strFlag);

    cv::rotate(mat, mat, flag);
    imshow(strTitle.toLocal8Bit().data() , mat );

}

void Flip(const int flag)
{
    Mat mat = imread("F:/lena.jpg");

    if( mat.data == NULL )
    {
        QMessageBox::warning(nullptr, "错误", "载入图片错误");
        return;
    }

    imshow("raw" , mat );

    QString strFlag;
    if( flag < 0)
        strFlag = "xy轴翻转 ";
    if( flag == 0)
        strFlag = "x轴翻转 ";
    if( flag > 0)
        strFlag = "y轴翻转";

    QString strTitle = QString("flip(%1:%2)").arg(flag).arg(strFlag);

    cv::flip(mat, mat, flag);
    imshow(strTitle.toLocal8Bit().data() , mat );

    mat = mat + mat ;

    QString strTitle1 = QString("mat + mat (%1:%2)").arg(flag).arg(strFlag);
    imshow(strTitle1.toLocal8Bit().data() , mat );
}

void Resize(const int flag)
{
    Mat mat = imread("F:/lena.jpg");

    if( mat.data == NULL )
    {
        QMessageBox::warning(nullptr, "错误", "载入图片错误");
        return;
    }

    imshow("raw" , mat );

    QString strFlag;
    if( flag == 0)
        strFlag = "INTER_NEAREST ";
    if( flag == 1)
        strFlag = "INTER_LINEAR  ";
    if( flag == 2)
        strFlag = "INTER_CUBIC  ";
    if( flag == 3)
        strFlag = "INTER_AREA  ";
    if( flag == 4)
        strFlag = "INTER_LANCZOS4  ";
    if( flag == 5)
        strFlag = "INTER_LINEAR_EXACT  ";
    if( flag == 7)
        strFlag = "INTER_MAX  ";
    if( flag == 8)
        strFlag = "WARP_FILL_OUTLIERS  ";
    if( flag == 16)
        strFlag = "WARP_INVERSE_MAP  ";

    QString strTitle = QString("resize(%1:%2)").arg(flag).arg(strFlag);

    Mat matResize;
    cv::resize(mat, matResize, Size(mat.rows / 2, mat.cols / 2), 0, 0, flag);
    imshow(strTitle.toLocal8Bit().data() , matResize );
}


#endif // BASICGRAPHICSDRAWING_H
