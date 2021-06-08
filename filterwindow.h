#ifndef FILTERWINDOW_H
#define FILTERWINDOW_H

#include <QMainWindow>

#include <opencv2/opencv.hpp>

namespace Ui {
class FilterWindow;
}

class FilterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FilterWindow(QWidget *parent = nullptr);
    ~FilterWindow();

private:
    Ui::FilterWindow *ui;


    enum FilterType
    {
        FT_BoxBlur,
        FT_Blur,
        FT_GaussianBlur,
        FT_MedianBlur,
        FT_BilateralFilter,
    };

    FilterType m_FilterType;

    int m_w = 3, m_h = 3;



private slots:

    void FilterW(int w = 3);
    void FilterH(int h = 3);

    void UpdateAction();

    void BoxBlur();         // 方波
    void Blur();            // 均值
    void GaussianBlur();    // 高斯
    void MedianBlur();      // 中值
    void BilateralFilter(); // 双边


private:
    cv::Mat m_matRaw;
};

#endif // FILTERWINDOW_H
