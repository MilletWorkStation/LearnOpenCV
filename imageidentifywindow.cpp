
#include "imageidentifywindow.h"
#include "ui_imageidentifywindow.h"
#include "common.h"

#include <opencv2/opencv.hpp>




ImageIdentifyWindow::ImageIdentifyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageIdentifyWindow)
{

    ui->setupUi(this);
    this->resize(1200,800);

    m_matRaw = LoadImage();

    // 分割窗体
    QSplitter * pSplitterMain = new QSplitter(this);
    pSplitterMain->resize(this->width(), this->height());
    pSplitterMain->setOrientation(Qt::Horizontal);

    // 左侧列表
    QListWidget * pListWidget = new QListWidget(pSplitterMain);
    pListWidget->resize(this->width(), this->height());
    pListWidget->addItem("Threshold");
    pListWidget->addItem(QString::fromLocal8Bit("Canny"));
    pListWidget->addItem(QString::fromLocal8Bit("Sobel"));
    pListWidget->addItem(QString::fromLocal8Bit("Resize"));
    pListWidget->addItem(QString::fromLocal8Bit("Hough"));

    // 右侧
    QSplitter * pSplitterRight = new QSplitter(pSplitterMain);
    pSplitterRight->setOrientation(Qt::Vertical);
    pSplitterRight->resize(this->width(), this->height());

    {

        QSplitter * pSplitterRightTop = new QSplitter(pSplitterRight);
        // 堆栈窗体
        {
            QStackedWidget * pStackedWidget = new QStackedWidget(pSplitterRightTop);
            pStackedWidget->resize(this->width(), this->height());
            connect(pListWidget, SIGNAL(currentRowChanged(int)), pStackedWidget, SLOT(setCurrentIndex(int)));

            // Threshold
            AddThresholdStackWidget(pStackedWidget);
            // canny
            AddCannyStackWidget(pStackedWidget);
            // Sobel
            AddSobelStackWidget(pStackedWidget);
            // Resize
            AddResizeStackWidget(pStackedWidget);
            // Hough
            AddHoughStackWidget(pStackedWidget);



        }

//        // 按钮
//        {
//            QSplitter * pSplitterRightBottom = new QSplitter(pSplitterRight);
//            QHBoxLayout * pHBoxLayout = new QHBoxLayout(pSplitterRightBottom);
//            QPushButton * pModifyButton = new QPushButton("Ok");
//            QPushButton * pCancleButton = new QPushButton("cancle");
//            pHBoxLayout->addStretch(1);
//            pHBoxLayout->addWidget(pModifyButton);
//            pHBoxLayout->addWidget(pCancleButton);
//        }
    }

    pSplitterMain->setStretchFactor(0, 2 );
    pSplitterMain->setStretchFactor(1, 8 );

}

ImageIdentifyWindow::~ImageIdentifyWindow()
{
    delete ui;
}

void ImageIdentifyWindow::AddThresholdStackWidget(QStackedWidget * pStackedWidget)
{

    if(m_pThresholdWidget != nullptr)
        return;

    m_pThresholdWidget = new QDialog(this);
    m_pThresholdWidget->setWindowTitle("threshold");
    {
        /*
                 * enum ThresholdTypes {
                            THRESH_BINARY     = 0,
                            THRESH_BINARY_INV = 1,
                            THRESH_TRUNC      = 2,
                            THRESH_TOZERO     = 3,
                            THRESH_TOZERO_INV = 4,
                            THRESH_MASK       = 7,
                            THRESH_OTSU       = 8,
                            THRESH_TRIANGLE   = 16
                    };
                */

        QVBoxLayout * pLayoutMain = new QVBoxLayout();

        QGridLayout * pGridLayoutRadioButton = new QGridLayout();

        QRadioButton* pButton_THRESH_BINARY        = new QRadioButton("THRESH_BINARY", m_pThresholdWidget);
        QRadioButton* pButton_THRESH_BINARY_INV    = new QRadioButton("THRESH_BINARY_INV", m_pThresholdWidget);
        QRadioButton* pButton_THRESH_TRUNC         = new QRadioButton("THRESH_TRUNC", m_pThresholdWidget);
        QRadioButton* pButton_THRESH_TOZERO        = new QRadioButton("THRESH_TOZERO", m_pThresholdWidget);
        QRadioButton* pButton_THRESH_TOZERO_INV    = new QRadioButton("THRESH_TOZERO_INV", m_pThresholdWidget);
        QRadioButton* pButton_THRESH_MASK          = new QRadioButton("THRESH_MASK", m_pThresholdWidget);
        QRadioButton* pButton_THRESH_OTSU          = new QRadioButton("THRESH_OTSU", m_pThresholdWidget);
        QRadioButton* pButton_THRESH_TRIANGLE      = new QRadioButton("THRESH_TRIANGLE", m_pThresholdWidget);

        connect(pButton_THRESH_BINARY, &QRadioButton::clicked, this, &ImageIdentifyWindow::OnThresholdTypesChanged);
        connect(pButton_THRESH_BINARY_INV, &QRadioButton::clicked, this, &ImageIdentifyWindow::OnThresholdTypesChanged);
        connect(pButton_THRESH_TRUNC, &QRadioButton::clicked, this, &ImageIdentifyWindow::OnThresholdTypesChanged);
        connect(pButton_THRESH_TOZERO, &QRadioButton::clicked, this, &ImageIdentifyWindow::OnThresholdTypesChanged);
        connect(pButton_THRESH_TOZERO_INV, &QRadioButton::clicked, this, &ImageIdentifyWindow::OnThresholdTypesChanged);
        connect(pButton_THRESH_MASK, &QRadioButton::clicked, this, &ImageIdentifyWindow::OnThresholdTypesChanged);
        connect(pButton_THRESH_OTSU, &QRadioButton::clicked, this, &ImageIdentifyWindow::OnThresholdTypesChanged);
        connect(pButton_THRESH_TRIANGLE, &QRadioButton::clicked, this, &ImageIdentifyWindow::OnThresholdTypesChanged);

        pButton_THRESH_BINARY->setChecked(true);
        pGridLayoutRadioButton->addWidget(pButton_THRESH_BINARY, 0, 0);
        pGridLayoutRadioButton->addWidget(pButton_THRESH_BINARY_INV, 0, 1);
        pGridLayoutRadioButton->addWidget(pButton_THRESH_TRUNC, 0, 2);
        pGridLayoutRadioButton->addWidget(pButton_THRESH_TOZERO, 0, 3);
        pGridLayoutRadioButton->addWidget(pButton_THRESH_TOZERO_INV, 1, 0);
        pGridLayoutRadioButton->addWidget(pButton_THRESH_MASK, 1, 1);
        pGridLayoutRadioButton->addWidget(pButton_THRESH_OTSU, 1, 2);
        pGridLayoutRadioButton->addWidget(pButton_THRESH_TRIANGLE, 1, 3);

        QHBoxLayout * pLayoutSlider = new QHBoxLayout();
        QSlider * pSlider = new QSlider(Qt::Horizontal , m_pThresholdWidget);

        m_pThresholdLabelPersent = new QLabel("125",m_pThresholdWidget);
        pSlider->setRange(0, 255);
        pSlider->setValue(125);
        pLayoutSlider->addWidget(pSlider, 9);
        pLayoutSlider->addWidget(m_pThresholdLabelPersent, 1);
        connect(pSlider, &QSlider::valueChanged, this, &ImageIdentifyWindow::OnThresholdSliderChanged);

        m_pThresholdLabelDesc = new QLabel(m_pThresholdWidget);

        // 图片，左右
        QHBoxLayout * pLayoutImage = new QHBoxLayout();
        QLabel * pLabelRaw = new QLabel(m_pThresholdWidget);
        m_pThresholdLabelAfter = new QLabel(m_pThresholdWidget);
        pLayoutImage->addWidget(pLabelRaw, 5);
        pLayoutImage->addWidget(m_pThresholdLabelAfter, 5);

        LabelDisplayMat(pLabelRaw, m_matRaw);
        LabelDisplayMat(m_pThresholdLabelAfter, m_matRaw);

        // 主界面
        pLayoutMain->addLayout(pGridLayoutRadioButton, 2);
        pLayoutMain->addLayout(pLayoutSlider, 1);
        pLayoutMain->addWidget(m_pThresholdLabelDesc);
        pLayoutMain->addLayout(pLayoutImage, 6);
        m_pThresholdWidget->setLayout(pLayoutMain);

        emit pButton_THRESH_BINARY->clicked(true);
        emit pSlider->valueChanged(125);
    }
    pStackedWidget->addWidget(m_pThresholdWidget);
}

void ImageIdentifyWindow::AddCannyStackWidget(QStackedWidget * pStackedWidget)
{
    if(m_pCannyWidget != nullptr)
        return;

    m_pCannyWidget = new QDialog(this);
    m_pCannyWidget->setWindowTitle(QString::fromLocal8Bit("Canny边缘检测"));
    {

        QVBoxLayout * pLayoutMain = new QVBoxLayout();

        QVBoxLayout * pLayoutSlider = new  QVBoxLayout();

        // low slider
        QHBoxLayout * pLowLayoutSlider = new QHBoxLayout();
        QSlider * pLowSlider = new QSlider(Qt::Horizontal , m_pCannyWidget);
        pLowSlider->setRange(1, 255);
        m_pLowSliderLabel = new QLabel( "0", m_pCannyWidget);
        QLabel * pDesc = new QLabel("Low        ", m_pCannyWidget);

        pLowLayoutSlider->addWidget(pDesc);
        pLowLayoutSlider->addWidget(pLowSlider);
        pLowLayoutSlider->addWidget(m_pLowSliderLabel);

        // high slider
        QHBoxLayout * pHighLayoutSlider = new QHBoxLayout();
        QSlider * pHighSlider = new QSlider(Qt::Horizontal , m_pCannyWidget);
        pHighSlider->setRange(20, 30);
        pHighSlider->setValue(20);
        m_pHighSliderLabel = new QLabel( "20", m_pCannyWidget);
        pDesc = new QLabel(QString::fromLocal8Bit("最小的倍数 "), m_pCannyWidget);

        pHighLayoutSlider->addWidget(pDesc);
        pHighLayoutSlider->addWidget(pHighSlider);
        pHighLayoutSlider->addWidget(m_pHighSliderLabel);

        // kenal slider
        QHBoxLayout * pKenalLayoutSlider = new QHBoxLayout();
        QSlider * pKenalSlider = new QSlider(Qt::Horizontal , m_pCannyWidget);
        pKenalSlider->setRange(3, 5);
        pKenalSlider->setValue(3);
        m_pKenalSliderLabel = new QLabel( "3", m_pCannyWidget);
        pDesc = new QLabel("Kenal      ", m_pCannyWidget);

        pKenalLayoutSlider->addWidget(pDesc);
        pKenalLayoutSlider->addWidget(pKenalSlider);
        pKenalLayoutSlider->addWidget(m_pKenalSliderLabel);

        pLayoutSlider->addLayout(pLowLayoutSlider);
        pLayoutSlider->addLayout(pHighLayoutSlider);
        pLayoutSlider->addLayout(pKenalLayoutSlider);

        connect(pLowSlider, &QSlider::valueChanged, this, &ImageIdentifyWindow::OnCannyLowSliderChanged);
        connect(pHighSlider, &QSlider::valueChanged, this, &ImageIdentifyWindow::OnCannyHighSliderChanged);
        connect(pKenalSlider, &QSlider::valueChanged, this, &ImageIdentifyWindow::OnCannyKenalSliderChanged);


        // 图片，左右
        QHBoxLayout * pLayoutImage = new QHBoxLayout();
        QLabel * pLabelRaw = new QLabel(m_pCannyWidget);
        m_pCannyLabelAfter = new QLabel(m_pCannyWidget);
        pLayoutImage->addWidget(pLabelRaw, 5);
        pLayoutImage->addWidget(m_pCannyLabelAfter, 5);

        LabelDisplayMat(pLabelRaw, m_matRaw);
        LabelDisplayMat(m_pCannyLabelAfter, m_matRaw);

        // 主界面
        pLayoutMain->addLayout(pLayoutSlider, 1);
        pLayoutMain->addLayout(pLayoutImage, 9);

        m_pCannyWidget->setLayout(pLayoutMain);

    }
    pStackedWidget->addWidget(m_pCannyWidget);
}

void ImageIdentifyWindow::AddSobelStackWidget(QStackedWidget * pStackedWidget)
{
    if(m_pSobelWidget != nullptr)
        return;

    m_pSobelWidget = new QDialog(this);
    m_pSobelWidget->setWindowTitle("Sobel");
    {

        QHBoxLayout * pRadioButtonLayout = new QHBoxLayout();
        QRadioButton * pRadioButton_1 = new QRadioButton("1", m_pSobelWidget);
        QRadioButton * pRadioButton_3 = new QRadioButton("3", m_pSobelWidget);
        QRadioButton * pRadioButton_5 = new QRadioButton("5", m_pSobelWidget);
        QRadioButton * pRadioButton_7 = new QRadioButton("7", m_pSobelWidget);
        pRadioButtonLayout->addWidget(pRadioButton_1);
        pRadioButtonLayout->addWidget(pRadioButton_3);
        pRadioButtonLayout->addWidget(pRadioButton_5);
        pRadioButtonLayout->addWidget(pRadioButton_7);

        connect(pRadioButton_1, &QRadioButton::clicked, this, &ImageIdentifyWindow::OnSobelKenalSizeChanged);
        connect(pRadioButton_3, &QRadioButton::clicked, this, &ImageIdentifyWindow::OnSobelKenalSizeChanged);
        connect(pRadioButton_5, &QRadioButton::clicked, this, &ImageIdentifyWindow::OnSobelKenalSizeChanged);
        connect(pRadioButton_7, &QRadioButton::clicked, this, &ImageIdentifyWindow::OnSobelKenalSizeChanged);

        QGridLayout * pLayoutImage = new QGridLayout();

        QLabel * pLabelRawDesc = new QLabel(QString::fromLocal8Bit("原图"), m_pSobelWidget);
        QLabel * pLabelAllDesc = new QLabel(QString::fromLocal8Bit("整体"), m_pSobelWidget);
        QLabel * pLabelXDesc = new QLabel(QString::fromLocal8Bit("X方向"), m_pSobelWidget);
        QLabel * pLabelYDesc = new QLabel(QString::fromLocal8Bit("Y方向"), m_pSobelWidget);

        QLabel * pSobelLabelRaw = new QLabel(m_pSobelWidget);
        m_pSobelLabelAll = new QLabel(m_pSobelWidget);
        m_pSobelLabelX = new QLabel(m_pSobelWidget);
        m_pSobelLabelY = new QLabel(m_pSobelWidget);


        pLayoutImage->addWidget(pLabelRawDesc, 0, 0);           pLayoutImage->addWidget(pLabelAllDesc, 0, 1);
        pLayoutImage->addWidget(pSobelLabelRaw, 1, 0);             pLayoutImage->addWidget(m_pSobelLabelAll, 1, 1);
        pLayoutImage->addWidget(pLabelXDesc, 2, 0);             pLayoutImage->addWidget(pLabelYDesc, 2, 1);
        pLayoutImage->addWidget(m_pSobelLabelX, 3, 0);               pLayoutImage->addWidget(m_pSobelLabelY, 3, 1);


        QVBoxLayout * pMainLayout = new QVBoxLayout(m_pSobelWidget);

        pMainLayout->addLayout(pRadioButtonLayout, 1);
        pMainLayout->addLayout(pLayoutImage, 9);

        m_pSobelWidget->setLayout(pMainLayout);

        LabelDisplayMat(pSobelLabelRaw, m_matRaw);
        LabelDisplayMat(m_pSobelLabelAll, m_matRaw);
        LabelDisplayMat(m_pSobelLabelX, m_matRaw);
        LabelDisplayMat(m_pSobelLabelY, m_matRaw);

    }
    pStackedWidget->addWidget(m_pSobelWidget);
}

void ImageIdentifyWindow::AddResizeStackWidget(QStackedWidget * pStackedWidget)
{
    if(m_pResizeWidget != nullptr)
        return;

    m_pResizeWidget = new QDialog(this);
    m_pResizeWidget->setWindowTitle("Resize");
    {

        QHBoxLayout * pResizeTypeLayout = new QHBoxLayout();
        QRadioButton * pResizeRadioButton = new QRadioButton("Resize", m_pResizeWidget);
        QRadioButton * pPyrUpRadioButton = new QRadioButton("PyrUp", m_pResizeWidget);
        QRadioButton * pPyrDownRadioButton = new QRadioButton("PyrDown", m_pResizeWidget);

        pResizeTypeLayout->addWidget(pResizeRadioButton);
        pResizeTypeLayout->addWidget(pPyrUpRadioButton);
        pResizeTypeLayout->addWidget(pPyrDownRadioButton);

        connect(pResizeRadioButton, &QRadioButton::clicked, this, &ImageIdentifyWindow::OnResizeTypeChanged);
        connect(pPyrUpRadioButton, &QRadioButton::clicked, this, &ImageIdentifyWindow::OnResizeTypeChanged);
        connect(pPyrDownRadioButton, &QRadioButton::clicked, this, &ImageIdentifyWindow::OnResizeTypeChanged);

        QVBoxLayout * pSliderLayout = new QVBoxLayout();

        QHBoxLayout * pSliderXLayout = new QHBoxLayout();
        QSlider * pXSlider = new QSlider(Qt::Horizontal , m_pResizeWidget);
        pXSlider->setRange(5, 20);
        pXSlider->setValue(10);
        m_pSliderXLabel = new QLabel("1.0");
        pSliderXLayout->addWidget(pXSlider, 9);
        pSliderXLayout->addWidget(m_pSliderXLabel, 1);

        QHBoxLayout * pSliderYLayout = new QHBoxLayout();
        QSlider * pYSlider = new QSlider(Qt::Horizontal , m_pResizeWidget);
        pYSlider->setRange(5, 20);
        pYSlider->setValue(10);
        m_pSliderYLabel = new QLabel("1.0");
        pSliderYLayout->addWidget(pYSlider, 9);
        pSliderYLayout->addWidget(m_pSliderYLabel, 1);

        pSliderLayout->addLayout(pSliderXLayout);
        pSliderLayout->addLayout(pSliderYLayout);

        connect(pXSlider, &QSlider::valueChanged, this, &ImageIdentifyWindow::OnResizeXSliderChanged);
        connect(pYSlider, &QSlider::valueChanged, this, &ImageIdentifyWindow::OnResizeYSliderChanged);


        QHBoxLayout * pLayoutImage = new QHBoxLayout();

        QLabel* pResizeLabelRaw = new QLabel(m_pResizeWidget);
        m_pResizeLabelAfter = new QLabel(m_pResizeWidget);

        pLayoutImage->addWidget(pResizeLabelRaw);
        pLayoutImage->addWidget(m_pResizeLabelAfter);

        QVBoxLayout * pMainLayout = new QVBoxLayout(m_pResizeWidget);

        pMainLayout->addLayout(pResizeTypeLayout, 1);
        pMainLayout->addLayout(pSliderLayout, 1);
        pMainLayout->addLayout(pLayoutImage, 8);

        m_pResizeWidget->setLayout(pMainLayout);

        LabelDisplayMat(pResizeLabelRaw, m_matRaw);
        LabelDisplayMat(m_pResizeLabelAfter, m_matRaw);

    }
    pStackedWidget->addWidget(m_pResizeWidget);
}

void ImageIdentifyWindow::AddHoughStackWidget(QStackedWidget * pStackedWidget)
{
    if(m_pHoughWidget != nullptr)
        return;

    m_pHoughWidget = new QDialog(this);
    m_pHoughWidget->setWindowTitle("Hough");
    {

        QHBoxLayout * pHoughTypeLayout = new QHBoxLayout();
        QRadioButton * pHoughSHTRadioButton = new QRadioButton("SHT", m_pHoughWidget);
        QRadioButton * pHoughMSHTRadioButton = new QRadioButton("MSHT", m_pHoughWidget);
        QRadioButton * pHoughPPHTRadioButton = new QRadioButton("PPHT", m_pHoughWidget);

        pHoughSHTRadioButton->setChecked(true);

        pHoughTypeLayout->addWidget(pHoughSHTRadioButton);
        pHoughTypeLayout->addWidget(pHoughMSHTRadioButton);
        pHoughTypeLayout->addWidget(pHoughPPHTRadioButton);

        connect(pHoughSHTRadioButton, &QRadioButton::clicked, this, &ImageIdentifyWindow::OnHoughTypeChanged);
        connect(pHoughMSHTRadioButton, &QRadioButton::clicked, this, &ImageIdentifyWindow::OnHoughTypeChanged);
        connect(pHoughPPHTRadioButton, &QRadioButton::clicked, this, &ImageIdentifyWindow::OnHoughTypeChanged);

        QVBoxLayout * pSliderLayout = new QVBoxLayout();

        QHBoxLayout * pSliderRhoLayout = new QHBoxLayout();
        QSlider * pRhoSlider = new QSlider(Qt::Horizontal , m_pHoughWidget);
        pRhoSlider->setRange(1, 10);
        pRhoSlider->setValue(1);
        m_pHoughRhoLabel = new QLabel("1.0");
        pSliderRhoLayout->addWidget(pRhoSlider, 9);
        pSliderRhoLayout->addWidget(m_pHoughRhoLabel, 1);

        QHBoxLayout * pHoughThetaLayout = new QHBoxLayout();
        QSlider * pThetaSlider = new QSlider(Qt::Horizontal , m_pHoughWidget);
        pThetaSlider->setRange(1, 180);
        pThetaSlider->setValue(1);
        m_pHoughThetaLabel = new QLabel("1");
        pHoughThetaLayout->addWidget(pThetaSlider, 9);
        pHoughThetaLayout->addWidget(m_pHoughThetaLabel, 1);

        QHBoxLayout * pThresholdLayout = new QHBoxLayout();
        QSlider * pThresholdSlider = new QSlider(Qt::Horizontal , m_pHoughWidget);
        pThresholdSlider->setRange(1, 255);
        pThresholdSlider->setValue(125);
        m_pHoughThresholdLabel = new QLabel("125");
        pThresholdLayout->addWidget(pThresholdSlider, 9);
        pThresholdLayout->addWidget(m_pHoughThresholdLabel, 1);


        pSliderLayout->addLayout(pSliderRhoLayout);
        pSliderLayout->addLayout(pHoughThetaLayout);
        pSliderLayout->addLayout(pThresholdLayout);

        connect(pRhoSlider, &QSlider::valueChanged, this, &ImageIdentifyWindow::OnHoughRhoChanged);
        connect(pThetaSlider, &QSlider::valueChanged, this, &ImageIdentifyWindow::OnHoughThetaChanged);
        connect(pThresholdSlider, &QSlider::valueChanged, this, &ImageIdentifyWindow::OnHoughThresholdChanged);

        QHBoxLayout * pLayoutImage = new QHBoxLayout();

        QLabel* pHoughLabelRaw = new QLabel(m_pHoughWidget);
        m_pHoughLabelAfter = new QLabel(m_pHoughWidget);

        pLayoutImage->addWidget(pHoughLabelRaw);
        pLayoutImage->addWidget(m_pHoughLabelAfter);

        QVBoxLayout * pMainLayout = new QVBoxLayout(m_pHoughWidget);

        pMainLayout->addLayout(pHoughTypeLayout, 1);
        pMainLayout->addLayout(pSliderLayout, 1);
        pMainLayout->addLayout(pLayoutImage, 8);

        m_pHoughWidget->setLayout(pMainLayout);

        LabelDisplayMat(pHoughLabelRaw, m_matRaw);
        LabelDisplayMat(m_pResizeLabelAfter, m_matRaw);

        emit pThresholdSlider->valueChanged(125);

    }
    pStackedWidget->addWidget(m_pHoughWidget);
}


void ImageIdentifyWindow::OnThresholdTypesChanged()
{
    QRadioButton * pRadioButton = (QRadioButton * )sender();
    if(pRadioButton->text() == "THRESH_BINARY")
    {
        m_pThresholdLabelDesc->setText(QString::fromLocal8Bit("THRESH_BINARY 大于阈值的部分为255 小于的为0"));
        m_ThresholdTypes = cv::THRESH_BINARY;
    }
    if(pRadioButton->text() == "THRESH_BINARY_INV")
    {
        m_pThresholdLabelDesc->setText(QString::fromLocal8Bit("THRESH_BINARY_INV 大于阈值的部分为0 小于的为255"));
        m_ThresholdTypes = cv::THRESH_BINARY_INV;
    }
    if(pRadioButton->text() == "THRESH_TRUNC")
    {
        m_pThresholdLabelDesc->setText(QString::fromLocal8Bit("THRESH_TRUNC 大于阈值的部分为阈值  小于的保持原样"));
        m_ThresholdTypes = cv::THRESH_TRUNC;
    }
    if(pRadioButton->text() == "THRESH_TOZERO")
    {
        m_pThresholdLabelDesc->setText(QString::fromLocal8Bit("THRESH_TOZERO 大于阈值的部分不变  小于的为0"));
        m_ThresholdTypes = cv::THRESH_TOZERO;
    }
    if(pRadioButton->text() == "THRESH_TOZERO_INV")
    {
        m_pThresholdLabelDesc->setText(QString::fromLocal8Bit("THRESH_TOZERO_INV 大于阈值的部分为0  小于的为保存不变"));
        m_ThresholdTypes = cv::THRESH_TOZERO_INV;
    }
    if(pRadioButton->text() == "THRESH_MASK")
    {
        m_pThresholdLabelDesc->setText(QString::fromLocal8Bit("THRESH_MASK 大于255的保持原样，小于的为0"));
        m_ThresholdTypes = cv::THRESH_MASK;
    }
    if(pRadioButton->text() == "THRESH_OTSU")
    {
        m_pThresholdLabelDesc->setText(QString::fromLocal8Bit("THRESH_OTSU 使用OTSU选取最佳阈值"));
        m_ThresholdTypes = cv::THRESH_OTSU;
    }
    if(pRadioButton->text() == "THRESH_TRIANGLE")
    {
        m_pThresholdLabelDesc->setText(QString::fromLocal8Bit("THRESH_TRIANGLE 使用三角算法选取最佳阈值"));
        m_ThresholdTypes = cv::THRESH_TRIANGLE;
    }

    OnThresholdParamChanged();
}
void ImageIdentifyWindow::OnThresholdSliderChanged(int value)
{
    m_nThresholdSliderValue = value;
    m_pThresholdLabelPersent->setText(QString("%1").arg(value));

    OnThresholdParamChanged();
}
void ImageIdentifyWindow::OnThresholdParamChanged()
{
    if( m_matRaw.data == NULL)
        return;

    cv::Mat dst;
    cv::cvtColor(m_matRaw, dst, cv::ColorConversionCodes::COLOR_RGB2GRAY);
    cv::threshold(dst, dst, m_nThresholdSliderValue, 255, m_ThresholdTypes);

    LabelDisplayMat(m_pThresholdLabelAfter, dst);
}

void ImageIdentifyWindow::OnCannyLowSliderChanged(int value)
{
    m_nCannyLowSliderValue = value;
    m_pLowSliderLabel->setText(QString("%1").arg(value));

    OnCannyParamChanged();
}
void ImageIdentifyWindow::OnCannyHighSliderChanged(int value)
{
    m_fCannyHighScale = float(value)/ 10.0;
    m_pHighSliderLabel->setText(QString("%1").arg(m_fCannyHighScale));

    OnCannyParamChanged();
}
void ImageIdentifyWindow::OnCannyKenalSliderChanged(int value)
{

    m_nCannyKenalSliderValue = value;
    m_nCannyKenalSliderValue = m_nCannyKenalSliderValue % 2 == 1 ? m_nCannyKenalSliderValue : m_nCannyKenalSliderValue + 1;

    m_pKenalSliderLabel->setText(QString("%1").arg(m_nCannyKenalSliderValue));

    OnCannyParamChanged();
}
void ImageIdentifyWindow::OnCannyParamChanged()
{
    if( m_matRaw.data == NULL)
        return;

    cv::Mat dst;

    cv::Canny(m_matRaw, dst, m_nCannyLowSliderValue, m_nCannyLowSliderValue * m_fCannyHighScale, m_nCannyKenalSliderValue);

    cv::Mat dst1 = m_matRaw.clone();
    dst1 = cv::Scalar::all(0);
    m_matRaw.copyTo(dst1, dst);

    LabelDisplayMat(m_pCannyLabelAfter, dst1);
}

void ImageIdentifyWindow::OnSobelKenalSizeChanged()
{
    m_nSobelKSize = ((QRadioButton*) sender())->text().toInt();

    OnSobelParamChanged();

}

void ImageIdentifyWindow::OnSobelParamChanged()
{
    if( m_matRaw.data == NULL)
        return;

    cv::Mat dsta,dstx,dsty;

    cv::Sobel(m_matRaw, dsta, -1, 1, 1, m_nSobelKSize);
    cv::Sobel(m_matRaw, dstx, -1, 1, 0, m_nSobelKSize);
    cv::Sobel(m_matRaw, dsty, -1, 0, 1, m_nSobelKSize);

//    cv::Mat dst1 = m_matRaw.clone();
//    dst1 = cv::Scalar::all(0);
//    m_matRaw.copyTo(dst1, dst);

    //LabelDisplayMat(m_pLabelRaw, dsta);
    LabelDisplayMat(m_pSobelLabelAll, dsta);
    LabelDisplayMat(m_pSobelLabelX, dstx);
    LabelDisplayMat(m_pSobelLabelY, dsty);

}

void ImageIdentifyWindow::OnResizeTypeChanged()
{
    if(((QRadioButton*)sender())->text() == "Resize")
        m_nResizeType = 0;
    else if(((QRadioButton*)sender())->text() == "PyrUp")
        m_nResizeType = 1;
    else if(((QRadioButton*)sender())->text() == "PyrDown")
        m_nResizeType = 2;

    OnResizeParamChanged();
}

void ImageIdentifyWindow::OnResizeXSliderChanged(int value)
{
    m_nResizeXScale = value;
    m_pSliderXLabel->setText(QString("%1").arg(float(m_nResizeXScale) / 10.0));

    OnResizeParamChanged();
}

void ImageIdentifyWindow::OnResizeYSliderChanged(int value)
{
    m_nResizeYScale = value;
    m_pSliderYLabel->setText(QString("%1").arg(float(m_nResizeYScale) / 10.0));

    OnResizeParamChanged();
}

void ImageIdentifyWindow::OnResizeParamChanged()
{
    if( m_matRaw.data == NULL)
        return;

    cv::Mat dst;
    float fXScale = float(m_nResizeXScale) / 10.0;
    float fYScale = float(m_nResizeYScale) / 10.0;

    if(m_nResizeType == 0)
        cv::resize(m_matRaw, dst, cv::Size(), fXScale, fYScale);
    else if(m_nResizeType == 1)
        cv::pyrUp(m_matRaw, dst, cv::Size());
    else if(m_nResizeType == 2)
        cv::pyrDown(m_matRaw, dst, cv::Size());

    LabelDisplayMat(m_pResizeLabelAfter, dst);
}

void ImageIdentifyWindow::OnHoughTypeChanged()
{
    if(((QRadioButton*)sender())->text() == "SHT")
        m_nHoughType = 0;
    else if(((QRadioButton*)sender())->text() == "MSHT")
        m_nHoughType = 1;
    else if(((QRadioButton*)sender())->text() == "PPHT")
        m_nHoughType = 2;

    OnHoughParamChanged();
}

void ImageIdentifyWindow::OnHoughRhoChanged(int value)
{
    m_dHoughRho = value;
    m_pHoughRhoLabel->setText(QString::fromLocal8Bit("像素精度：%1").arg(value));

    OnHoughParamChanged();
}


void ImageIdentifyWindow::OnHoughThetaChanged(int value)
{
    #define CV_PI   3.1415926535897932384626433832795
    m_dHoughTheta = float(value) / 180.0 * CV_PI;
    m_pHoughThetaLabel->setText(QString::fromLocal8Bit("角度精度：%1").arg(m_dHoughTheta));

    OnHoughParamChanged();
}

void ImageIdentifyWindow::OnHoughThresholdChanged(int value)
{
    m_dHoughThreshold = value;
    m_pHoughThresholdLabel->setText(QString::fromLocal8Bit("阈值：%1").arg(m_dHoughThreshold));

    OnHoughParamChanged();
}

void ImageIdentifyWindow::OnHoughParamChanged()
{
    if( m_matRaw.data == NULL)
        return;

    cv::Mat mid = m_matRaw.clone(), dst;

    cv::Canny(m_matRaw, dst, 50, 200, 3);//进行一此canny边缘检测
    //cv::cvtColor(mid, dst, cv::COLOR_RGB2GRAY);

    std::vector<cv::Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合

    if(m_nHoughType == 0)
        cv::HoughLines(dst, lines, m_dHoughRho, m_dHoughTheta / 180, m_dHoughThreshold, 0, 0 );
        //HoughLines(dst, lines, 1, CV_PI/180, 150, 0, 0 );
    else if(m_nHoughType == 1)
        cv::HoughLines(dst, lines, m_dHoughRho, m_dHoughTheta / 180, m_dHoughThreshold, 0, 0 );
    else if(m_nHoughType == 2)
    {
        std::vector<cv::Vec4i> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
        cv::HoughLinesP(dst, lines, m_dHoughRho, m_dHoughTheta / 180, m_dHoughThreshold, 50);

        for( size_t i = 0; i < lines.size(); i++ )
        {
            cv::Vec4i l = lines[i];
            line( mid, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(186,88,255), 1);
        }

        LabelDisplayMat(m_pHoughLabelAfter, mid);

        return;
    }


    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        cv::line( mid, pt1, pt2, cv::Scalar(0, 0, 255), 1);
    }

    LabelDisplayMat(m_pHoughLabelAfter, mid);

}
