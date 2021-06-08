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
    void OnThresholdTypesChanged();
    void OnThresholdSliderChanged(int value);
    void OnThresholdParamChanged();
private:
    void AddThresholdStackWidget(QStackedWidget * p);
    void AddDoubleThresholdStackWidget(QStackedWidget * p);

private:
    Ui::ImageIdentifyWindow *ui;

    cv::Mat m_matRaw;

    // Threshold
    QLabel * m_pLabelAfter = nullptr;
    QLabel * m_pLabelPersent = nullptr;
    QLabel * m_pLabelDesc = nullptr;
    cv::ThresholdTypes m_ThresholdTypes = cv::THRESH_BINARY;
    int m_nSliderValue;

};

#endif // IMAGEIDENTIFYWINDOW_H
