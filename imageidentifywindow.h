#ifndef IMAGEIDENTIFYWINDOW_H
#define IMAGEIDENTIFYWINDOW_H
#include <QMainWindow>
#include <QSplitter>
#include <QStackedWidget>

#include <opencv2/opencv.hpp>
#include <QRadioButton>

#include <QListWidget>
#include <QStackedWidget>
#include <QLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QPixmap>
#include <QPushButton>
#include <QComboBox>
#include <QSplitter>
#include <QWidget>
#include <QLabel>
#include <QDialog>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QRadioButton>
#include <QStringLiteral>
#include <QTextBrowser>
#include <QSpacerItem>


namespace Ui {
class ImageIdentifyWindow;
}

class ImageIdentifyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageIdentifyWindow(QWidget *parent = nullptr);
    ~ImageIdentifyWindow();


private slots:
    // Threshold
    void OnThresholdTypesChanged();
    void OnThresholdSliderChanged(int value);
    void OnThresholdParamChanged();

    // Canny
    void OnCannyLowSliderChanged(int value);
    void OnCannyHighSliderChanged(int value);
    void OnCannyKenalSliderChanged(int value);
    void OnCannyParamChanged();

    // Sobel
    void OnSobelKenalSizeChanged();
    void OnSobelParamChanged();

    // resize
    void OnResizeTypeChanged();
    void OnResizeXSliderChanged(int value);
    void OnResizeYSliderChanged(int value);
    void OnResizeParamChanged();

    // Hough
    void OnHoughTypeChanged();
    void OnHoughRhoChanged(int value);
    void OnHoughThetaChanged(int value);
    void OnHoughThresholdChanged(int value);
    void OnHoughParamChanged();




private:
    void AddThresholdStackWidget(QStackedWidget * p);
    void AddCannyStackWidget(QStackedWidget * p);
    void AddSobelStackWidget(QStackedWidget * p);
    void AddResizeStackWidget(QStackedWidget * p);
    void AddHoughStackWidget(QStackedWidget * p);


    QDialog * m_pThresholdWidget = nullptr;
    QDialog * m_pCannyWidget = nullptr;
    QDialog * m_pSobelWidget = nullptr;
    QDialog * m_pResizeWidget = nullptr;
    QDialog * m_pPyrUpWidget = nullptr;
    QDialog * m_pPyrDownWidget = nullptr;
    QDialog * m_pHoughWidget = nullptr;


private:
    Ui::ImageIdentifyWindow *ui;

    cv::Mat m_matRaw;

    // Threshold
    int m_nThresholdSliderValue;
    QLabel * m_pThresholdLabelAfter = nullptr;
    QLabel * m_pThresholdLabelPersent = nullptr;
    QLabel * m_pThresholdLabelDesc = nullptr;
    cv::ThresholdTypes m_ThresholdTypes = cv::THRESH_BINARY;

    // canny
    float m_fCannyHighScale = 2.0;
    int m_nCannyLowSliderValue = 1;
    int m_nCannyKenalSliderValue =  3;
    QLabel * m_pCannyLabelAfter = nullptr;
    QLabel * m_pLowSliderLabel = nullptr;
    QLabel * m_pHighSliderLabel = nullptr;
    QLabel * m_pKenalSliderLabel = nullptr;

    // Sobel
    int m_nSobelKSize = 1;
    QLabel * m_pSobelLabelAll = nullptr;
    QLabel * m_pSobelLabelX = nullptr;
    QLabel * m_pSobelLabelY = nullptr;

    // Resize
    int m_nResizeType = 0;   // 0:resize  1:pyrup  1:pyrdown
    int m_nResizeXScale = 5;
    int m_nResizeYScale = 5;
    QLabel * m_pResizeLabelAfter = nullptr;
    QLabel * m_pSliderXLabel = nullptr;
    QLabel * m_pSliderYLabel = nullptr;

    // Hough
    int m_nHoughType = 0;                   // 0:HoughLines[SHT]标准变换   1: HoughLines[MSHT]多尺度变换  2：HoughLinesP[PPHT] 累计概率
    double m_dHoughRho = 1.0;               // 以像素为单位的距离精度。另一种形容方式是直线搜索时的进步尺寸的单位半径。
    double m_dHoughTheta = 3.1415926;             // 以弧度为单位的角度精度。另一种形容方式是直线搜索时的进步尺寸的单位角度。
    double m_dHoughThreshold = 10;
    QLabel * m_pHoughRhoLabel = nullptr;
    QLabel * m_pHoughThetaLabel = nullptr;
    QLabel * m_pHoughThresholdLabel = nullptr;
    QLabel * m_pHoughLabelAfter = nullptr;

};

#endif // IMAGEIDENTIFYWINDOW_H
