#ifndef IMAGESPLITANDMERGE_H
#define IMAGESPLITANDMERGE_H

#include <opencv2/opencv.hpp>

#include <QFileDialog>
#include <QMessageBox>
using namespace cv;


/// 图像颜色通道分离和图像颜色多通道混合
//  分离通道函数：splite()
//  混合通道函数：merge()


void Split();

void Merge();


void ImageSplitAndMerge()
{
    //Split();

    Merge();
}

void Split()
{
    Mat mat = imread("F:/lena.jpg");

    if( mat.data == NULL )
    {
        QMessageBox::warning(nullptr, "错误", "载入图片错误");
        return;
    }

    Mat splitMat[3];
    split(mat, splitMat);


    imshow("raw", mat);
    imshow("splitMat0", splitMat[0]);
    imshow("splitMat1", splitMat[1]);
    imshow("splitMat2", splitMat[2]);


}

void Merge()
{
    Mat mat = imread("F:/lena.jpg");

    if( mat.data == NULL )
    {
        QMessageBox::warning(nullptr, "错误", "载入图片错误");
        return;
    }

    Mat splitMat[3];
    split(mat, splitMat);

    imshow("raw", mat);
    imshow("splitMat0", splitMat[0]);
    imshow("splitMat1", splitMat[1]);
    imshow("splitMat2", splitMat[2]);

    Mat mergeMat;
    merge(splitMat, 3, mergeMat);

    imshow("mergeMat", mergeMat);
}

#endif // IMAGESPLITANDMERGE_H
