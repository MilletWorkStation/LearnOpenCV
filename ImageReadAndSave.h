#ifndef CV02IMAGEREADANDSAVE_H
#define CV02IMAGEREADANDSAVE_H



/*
 * 1、读取图片
*/


#include <opencv2/opencv.hpp>

#include <QFileDialog>
#include <QMessageBox>
using namespace cv;


/*
 * OpenCV读取图片文件
 * 读取图像文件，使用的imread()。
 *
 * imread()函数返回的是Mat对象，如果读取函数文件失败，会返回一个空矩阵（常量cv::Mat::data变量的值是NULL）。
 *
 * Mat imread(const string& filename, int flags=1 )
 * filename：是被读取或者保存的图像文件名；
 * flag：参数值有三种情况：
 *  flag>0，该函数返回3通道图像，如果磁盘上的图像文件是单通道的灰度图像，则会被强制转为3通道；
 *  flag=0，该函数返回单通道图像，如果磁盘的图像文件是多通道图像，则会被强制转为单通道；
 *  flag<0，则函数不对图像进行通道转换。
 *
 * imread()函数支持多种文件格式，且该函数是根据图像文件的内容来确定文
 *
 * 件格式，而不是根据文件的扩展名来确定。
 *
 * 支持的图像格式：bmp、dib、jpg、jpeg、jpe、png、pbm、pgm、ppm、sr、ras、tiff、tif、exr、jp2。
 * (需要对应的库支持，开发使用的库要编译时添加进去，然后编译生成opencv库)
*/
void ImageReadAndSave();
void ImageRead(const int flag);
void ImageSave();


void ImageReadAndSave()
{
    ImageRead(-1);
    ImageRead(0);
    ImageRead(1);

//    ImageSave();
}

/// 读取，保存文件
////////////////////////////////////////////////////////////////
///
void ImageRead(const int flag)
{
//    QString filename = QFileDialog::getOpenFileName(nullptr, "open image", ".", "Image file(*.png *.jpg *.bmp)");

    //Mat mat = imread("qrc:/Image/lena.jpg", flag);

    //Mat mat = imread("./lena.jpg", flag);

    //Mat mat = imread(":/Image/lena.jpg", flag);

    Mat mat = imread("F:/lena.jpg", flag);

    if( mat.data == NULL )
    {
        QMessageBox::warning(nullptr, "错误", "载入图片错误");
        return;
    }

    //imshow(filename.toLocal8Bit().data(), mat);

    QString strDesc;
    if( flag < 0 )
        strDesc = "函数不对图像进行通道转换";
    if( flag == 0 )
        strDesc = "函数返回单通道图像，如果磁盘的图像文件是多通道图像，则会被强制转为单通道";
    if( flag > 0 )
        strDesc = "函数返回3通道图像，如果磁盘上的图像文件是单通道的灰度图像，则会被强制转为3通道";

    QString strTitle = QString("lena(%1:%2).jpg").arg(flag).arg(strDesc);

    //namedWindow(strTitle.toLocal8Bit().data(), WINDOW_FULLSCREEN);

    imshow(strTitle.toLocal8Bit().data() , mat );
    resizeWindow(strTitle.toLocal8Bit().data(), 800, 800);
}

void ImageSave()
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

    imshow("save mat", img);

    QString filename = QFileDialog::getSaveFileName(nullptr, "save image", ".", "Image file(*.png *.jpg *.bmp)");
    imwrite(filename.toLocal8Bit().data(), img);
}


#endif // CV02IMAGEREADANDSAVE_H
