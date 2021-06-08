#ifndef IMAGEROI_H
#define IMAGEROI_H


#include <opencv2/opencv.hpp>

#include <QFileDialog>
#include <QMessageBox>
using namespace cv;

/*
 * 图像操作常常需要获取指定区域的图像，对于OpenCV提供了ROI（感兴趣区域）支持；
 *    图像的叠加和混合也是常用的操作之一。
 * 在图像处理领域中，专业名词感兴趣的区域其实就是选取指定区域的图像。
*/


/// 提取感兴趣区域（Region of interest）有两种方法
/// BasicConceptsAndOperations.h
//  1、构造函数
//  2、括号运算符
void SelectROI();

/// 图像混合（整体混合）
// 计算数组加权和：addWeighted()函数
void AddWeighted();

void ImageROI()
{
    // SelectROI();

    AddWeighted();
}

void SelectROI()
{
    Mat mat = imread("F:/lena.jpg");

    if( mat.data == NULL )
    {
        QMessageBox::warning(nullptr, "错误", "载入图片错误");
        return;
    }

    cv::rectangle(mat, cv::Rect(50, 50, 100, 150), Scalar(0, 0, 255), 3);

    Mat roi(mat, Rect(50, 50, 100, 150));

    imshow("raw", mat);
    imshow("roi", roi);
}

void AddWeighted()
{
    Mat mat = imread("F:/lena.jpg");
    Mat roi = imread("F:/roi.png");

    if( mat.data == NULL )
    {
        QMessageBox::warning(nullptr, "错误", "载入图片错误");
        return;
    }

    addWeighted(mat, 0.6, roi, 0.4, 0, roi);

    imshow("roi", roi);

}


#endif // IMAGEROI_H
