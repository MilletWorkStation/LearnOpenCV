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





private:
    void AddThresholdStackWidget(QStackedWidget * p);
    void AddCannyStackWidget(QStackedWidget * p);
    void AddSobelStackWidget(QStackedWidget * p);
    void AddResizeStackWidget(QStackedWidget * p);


    QDialog * m_pThresholdWidget = nullptr;
    QDialog * m_pCannyWidget = nullptr;
    QDialog * m_pSobelWidget = nullptr;
    QDialog * m_pResizeWidget = nullptr;
    QDialog * m_pPyrUpWidget = nullptr;
    QDialog * m_pPyrDownWidget = nullptr;

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

    //

};

#endif // IMAGEIDENTIFYWINDOW_H
