#ifndef CONTRASTANDBRIGHTNESS_H
#define CONTRASTANDBRIGHTNESS_H

#include <QMainWindow>

#include <opencv2/opencv.hpp>


namespace Ui {
class ContrastAndBrightness;
}

class ContrastAndBrightness : public QMainWindow
{
    Q_OBJECT

public:
    explicit ContrastAndBrightness(QWidget *parent = nullptr);
    ~ContrastAndBrightness();

private slots:
    void on_contrastSlider_valueChanged(int value);

    void on_brightnessSlider_valueChanged(int value);

private:
    Ui::ContrastAndBrightness *ui;
    cv::Mat Change(const double alpha, const int beta);
    cv::Mat m_RawMat;
};

#endif // CONTRASTANDBRIGHTNESS_H
