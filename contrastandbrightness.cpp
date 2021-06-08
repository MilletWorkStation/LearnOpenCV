#include "contrastandbrightness.h"
#include "ui_contrastandbrightness.h"
#include "common.h"

#include <QDockWidget>
#include <QLabel>
#include <QMessageBox>


ContrastAndBrightness::ContrastAndBrightness(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ContrastAndBrightness)
{
    ui->setupUi(this);

    m_RawMat = cv::imread("F:/lena.jpg");

    if( m_RawMat.data == NULL )
    {
        QMessageBox::warning(nullptr, "错误", "载入图片错误");
    }

    ui->contrastSlider->setRange(-50, 50);
    ui->brightnessSlider->setRange(-125, 125);


    LabelDisplayMat(ui->labelRaw, m_RawMat);
    LabelDisplayMat(ui->labelAfter, m_RawMat);

}

ContrastAndBrightness::~ContrastAndBrightness()
{
    delete ui;
}


cv::Mat ContrastAndBrightness::Change(const double alpha, const int beta)
{

    // 参数 a (a >0) 被称为增益,用来控制图像的对比度 (0 - 1)
    // 参数 b 被称为偏置,用来控制图像的亮度 (0 - 255)

    cv::Mat dst = m_RawMat.clone();

    int height = m_RawMat.rows;
    int width = m_RawMat.cols;

    for (int row = 0; row < height;row++)
    {
        for (int col = 0; col < width;col++)
        {
            if (m_RawMat.channels()==3)
            {
                float b = m_RawMat.at<cv::Vec3b>(row, col)[0];//blue
                float g = m_RawMat.at<cv::Vec3b>(row, col)[1];//green
                float r = m_RawMat.at<cv::Vec3b>(row, col)[2];//red

                //output
                dst.at<cv::Vec3b>(row, col)[0] = cv::saturate_cast<uchar>(b*alpha + beta);
                dst.at<cv::Vec3b>(row, col)[1] = cv::saturate_cast<uchar>(g*alpha + beta);
                dst.at<cv::Vec3b>(row, col)[2] = cv::saturate_cast<uchar>(r*alpha + beta);
            }
            else if (m_RawMat.channels()==1)
            {
                float gray = m_RawMat.at<uchar>(row,col);
                dst.at<uchar>(row, col) = cv::saturate_cast<uchar>(gray*alpha+beta);
            }
        }
    }

    return dst;
}

void ContrastAndBrightness::on_contrastSlider_valueChanged(int value)
{
    cv::Mat mat = Change(float(ui->contrastSlider->value())/float(ui->contrastSlider->maximum()), ui->brightnessSlider->value());
    LabelDisplayMat(ui->labelAfter, mat);

    QString str = QString("%1%").arg(float(ui->contrastSlider->value())/float(ui->contrastSlider->maximum()) * 100);
    ui->contrasPercent->setText(str);

}

void ContrastAndBrightness::on_brightnessSlider_valueChanged(int value)
{
    cv::Mat mat = Change(float(ui->contrastSlider->value())/float(ui->contrastSlider->maximum()), ui->brightnessSlider->value());
    LabelDisplayMat(ui->labelAfter, mat);

    QString str = QString("%1%").arg(float(ui->brightnessSlider->value())/float(ui->brightnessSlider->maximum()) * 100);
    ui->brightnessPercent->setText(str);
}
