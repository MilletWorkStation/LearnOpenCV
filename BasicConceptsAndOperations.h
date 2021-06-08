#ifndef VISITPIEXLINMAT_H
#define VISITPIEXLINMAT_H

/*
 * 基本概念和操作
 * 1、访问像素
 * 2、选择单行/单列
 * 3、选择指定区域（Region of interest）
 * 4、取对角线
 *
 * cv::Mat表达式所支持的运算。
 * 下面的列表中使用 A 和 B 表示 Mat 类型的对象，使用s表示Scalar对象，alpha 表示double值。
 *
 * 加法，减法，取负：A+B， A-B， A+s， A-s， s+A， s-A， -A
 * 缩放取值范围：A * alpha
 * 矩阵对应元素的乘法和除法：A.mul(B)，A/B，alpha/A
 * 矩阵乘法：A*B（注意此处是矩阵乘法，而不是矩阵对应元素相乘）
 * 矩阵转置：A.t()
 * 矩阵求逆和求伪逆：A.inv()
 * 矩阵比较运算：A cmpop B，A cmpop alpha，alpha cmpop A。此处cmpop
 * 可以是>，>=，==，!=，<=，<。如果条件成立，则结果矩阵（8U类型矩阵）的对应元素被置为255；否则置0
 * 矩阵位逻辑运算： A logicop B， A logicop s， s logicop A， ~A，此处 logicop
 * 可以是&，|和^
 * 矩阵对应元素的最大值和最小值：min(A, B)，min(A, alpha)，max(A, B)，max(A, alpha)
 * 矩阵中元素的绝对值：abs(A)
 * 叉积和点积：A.cross(B)，A.dot(B)
 *
 *
*/


#include <opencv2/opencv.hpp>
using namespace cv;



/// 访问像素的 3种方式
//  1、at()
//  2、使用迭代器
//  3、通过数据指针

void VisitPixel();
void VisitPixelByAt();
void VisitPixelByIterator();
void VisitPixelByPointer();


/// 图像局部操作
//  1、访问单行
//  2、访问列

void SelectRowOrCol();
void SelectRow();
void SelectCol();


/// 选择指定区域 2种方式
//  1、使用构造函数
//  2、使用括号运算符

void SelectRegion();
void SelectRegionByConstructor();
void SelectRegionByBracketOperator();

/// 取对角线元素
//  diag()
//      参数d=0时，表示取主对角线；
//      当参数d>0是，表示取主对角线下方的次对线，如当d=1 时，表示取主对角线下方，且紧贴主多角线的元素；
//      当参数d<0时，示取主对角线上方的次对角线。如同 row()和 col()函数， diag()函数也不进行内存复制操作，其复杂度也是O(1)。
void Diag();

///////////////////////////////////////////////////////////////
void BasicConceptsAndOperations()
{
    //VisitPixel();

    //SelectRowOrCol();

    //SelectRegion();

    Diag();
}


///////////////////////////      实现        //////////////////////////////////////////////////


/// 访问像素
////////////////////////////////////////////////////////////////
///
void VisitPixel()
{

    VisitPixelByAt();
    VisitPixelByIterator();
    VisitPixelByPointer();
}

void VisitPixelByAt()
{
    cv::Mat grayim(600, 800, CV_8UC1);
    cv::Mat colorim(600, 800, CV_8UC3);
    // 遍历所有像素，并设置像素值
    for( int i = 0; i < grayim.rows; ++i)
    {
        for( int j = 0; j < grayim.cols; ++j )
            grayim.at<uchar>(i,j) = (i+j)%255;
    }


    // 遍历所有像素，并设置像素值
    for( int i = 0; i < colorim.rows; ++i)
    {
        for( int j = 0; j < colorim.cols; ++j )
        {
            cv::Vec3b pixel;
            // 注意：opencv通道顺序，BGR，非RGB
            pixel[0] = i%255;  // Blue
            pixel[1] = j%255;  // Green
            pixel[2] = 0;      // Red
            colorim.at<Vec3b>(i,j) = pixel;
        }
    }

    imshow("at-gray", grayim);
    imshow("at-color", colorim);
}

void VisitPixelByIterator()
{
    cv::Mat grayim(600, 800, CV_8UC1);
    cv::Mat colorim(600, 800, CV_8UC3);

    // 遍历所有像素，并设置像素值
    cv::MatIterator_<uchar> grayit, grayend;
    for(grayit = grayim.begin<uchar>(), grayend = grayim.end<uchar>();
        grayit != grayend; ++grayit)
    {
        *grayit = rand()%255;
    }
    // 遍历所有像素，并设置像素值
    cv::MatIterator_<cv::Vec3b> colorit, colorend;
    for(colorit = colorim.begin<cv::Vec3b>(), colorend = colorim.end<cv::Vec3b>();
        colorit != colorend; ++colorit)
    {
        (*colorit)[0] = rand()%255; // Blue
        (*colorit)[1] = rand()%255; // Green
        (*colorit)[2] = rand()%255; // Red
    }


    imshow("iterator-gray", grayim);
    imshow("iterator-color", colorim);

}

void VisitPixelByPointer()
{
    cv::Mat grayim(600, 800, CV_8UC1);
    cv::Mat colorim(600, 800, CV_8UC3);

    //遍历所有像素，并设置像素值
    for( int i = 0; i < grayim.rows; ++i)
    {
        //获取第 i 行首像素指针
        uchar * p = grayim.ptr<uchar>(i);
        //对第 i 行的每个像素(byte)操作
        for( int j = 0; j < grayim.cols; ++j )
        p[j] = (i+j)%255;
    }

    //遍历所有像素，并设置像素值
    for( int i = 0; i < colorim.rows; ++i)
    {
        //获取第 i 行首像素指针
        cv::Vec3b * p = colorim.ptr<cv::Vec3b>(i);
        for( int j = 0; j < colorim.cols; ++j )
        {
            p[j][0] = i%255;    //Blue
            p[j][1] = j%255;    //Green
            p[j][2] = 0;        //Red
        }
    }

    imshow("point-gray", grayim);
    imshow("point-color", colorim);
}

/// 局部操作
////////////////////////////////////////////////////////////////
///

void SelectRowOrCol()
{
    SelectRow();
    SelectCol();
}

void SelectRow()
{
    cv::Mat grayim(600, 800, CV_8UC1);
    // 遍历所有像素，并设置像素值
    for( int i = 0; i < grayim.rows; ++i)
    {
        for( int j = 0; j < grayim.cols; ++j )
            grayim.at<uchar>(i,j) = (i+j)%255;
    }

    Mat row = grayim.row(1);

    imshow("row-gray", grayim);
    imshow("row1-gray", row);
}

void SelectCol()
{
    cv::Mat grayim(600, 800, CV_8UC1);
    // 遍历所有像素，并设置像素值
    for( int i = 0; i < grayim.rows; ++i)
    {
        for( int j = 0; j < grayim.cols; ++j )
            grayim.at<uchar>(i,j) = (i+j)%255;
    }

    Mat col = grayim.col(1);

    imshow("col-gray", grayim);
    imshow("col1-gray", col);
}

/// 合适区域
////////////////////////////////////////////////////////////////
///

void SelectRegion()
{
    SelectRegionByConstructor();

    SelectRegionByBracketOperator();
}

void SelectRegionByConstructor()
{
    //创建宽度为 320，高度为 240 的 3 通道图像
    Mat img(Size(800, 240), CV_8UC3);


    // 遍历所有像素，并设置像素值
    for( int i = 0; i < img.rows; ++i)
    {
        for( int j = 0; j < img.cols; ++j )
        {
            cv::Vec3b pixel;
            // 注意：opencv通道顺序，BGR，非RGB
            pixel[0] = i%255;  // Blue
            pixel[1] = j%255;  // Green
            pixel[2] = 0;      // Red
            img.at<Vec3b>(i,j) = pixel;
        }
    }


    //roi 是表示 img 中 Rect(10, 10, 100, 100)区域的对象
    Mat roi(img, Rect(100, 10, 300, 100));

    imshow("img", img);
    imshow("roi", roi);
}

void SelectRegionByBracketOperator()
{
    //创建宽度为 320，高度为 240 的 3 通道图像
    Mat img(Size(800, 240), CV_8UC3);


    // 遍历所有像素，并设置像素值
    for( int i = 0; i < img.rows; ++i)
    {
        for( int j = 0; j < img.cols; ++j )
        {
            cv::Vec3b pixel;
            // 注意：opencv通道顺序，BGR，非RGB
            pixel[0] = i%255;  // Blue
            pixel[1] = j%255;  // Green
            pixel[2] = 0;      // Red
            img.at<Vec3b>(i,j) = pixel;
        }
    }

    //roi 是表示 img 中 Rect(10, 10, 100, 100)区域的对象
    Mat roi = img(Range(10, 100), Range(10, 100));

    imshow("img", img);
    imshow("roi", roi);


//    cv::Point point;
//    cv::Rect rect;
//    cv::Rect rectShift = rect + point;
}
\

/// 对角线
////////////////////////////////////////////////////////////////
///
void Diag()
{
    //创建宽度为 320，高度为 240 的 3 通道图像
    Mat img(Size(800, 240), CV_8UC3);


    // 遍历所有像素，并设置像素值
    for( int i = 0; i < img.rows; ++i)
    {
        for( int j = 0; j < img.cols; ++j )
        {
            cv::Vec3b pixel;
            // 注意：opencv通道顺序，BGR，非RGB
            pixel[0] = i%255;  // Blue
            pixel[1] = j%255;  // Green
            pixel[2] = 0;      // Red
            img.at<Vec3b>(i,j) = pixel;
        }
    }


    //roi 是表示 img 中 Rect(10, 10, 100, 100)区域的对象
    Mat diag = img.diag();

    imshow("img", img);
    imshow("diag", diag);
}
#endif // VISITPIEXLINMAT_H
