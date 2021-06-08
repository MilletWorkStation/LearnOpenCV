
#include "imageidentifywindow.h"
#include "ui_imageidentifywindow.h"


#include "common.h"

#include <QStringLiteral>


ImageIdentifyWindow::ImageIdentifyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageIdentifyWindow)
{

    ui->setupUi(this);
    this->resize(1200,800);

    m_matRaw = LoadImage();

    // �ָ��
    QSplitter * pSplitterMain = new QSplitter(this);
    pSplitterMain->resize(this->width(), this->height());
    pSplitterMain->setOrientation(Qt::Horizontal);

    // ����б�
    QListWidget * pListWidget = new QListWidget(pSplitterMain);
    pListWidget->resize(this->width(), this->height());
    pListWidget->addItem("threshold");
    pListWidget->addItem(QString::fromLocal8Bit("˫��ֵ"));


    // �Ҳ�
    QSplitter * pSplitterRight = new QSplitter(pSplitterMain);
    pSplitterRight->setOrientation(Qt::Vertical);
    pSplitterRight->resize(this->width(), this->height());

    {

        QSplitter * pSplitterRightTop = new QSplitter(pSplitterRight);
        // ��ջ����
        {
            QStackedWidget * pStackedWidget = new QStackedWidget(pSplitterRightTop);
            pStackedWidget->resize(this->width(), this->height());
            connect(pListWidget, SIGNAL(currentRowChanged(int)), pStackedWidget, SLOT(setCurrentIndex(int)));

            // threshold
            AddThresholdStackWidget(pStackedWidget);



        }

//        // ��ť
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

//    pSplitterMain->setStretchFactor(0, 3 );
//    pSplitterMain->setStretchFactor(1, 7 );

}

ImageIdentifyWindow::~ImageIdentifyWindow()
{
    delete ui;
}

void ImageIdentifyWindow::OnThresholdTypesChanged()
{
    QRadioButton * pRadioButton = (QRadioButton * )sender();
    if(pRadioButton->text() == "THRESH_BINARY")
    {
        m_pLabelDesc->setText(QString::fromLocal8Bit("THRESH_BINARY ������ֵ�Ĳ���Ϊ255 С�ڵ�Ϊ0"));
        m_ThresholdTypes = cv::THRESH_BINARY;
    }
    if(pRadioButton->text() == "THRESH_BINARY_INV")
    {
        m_pLabelDesc->setText(QString::fromLocal8Bit("THRESH_BINARY_INV ������ֵ�Ĳ���Ϊ0 С�ڵ�Ϊ255"));
        m_ThresholdTypes = cv::THRESH_BINARY_INV;
    }
    if(pRadioButton->text() == "THRESH_TRUNC")
    {
        m_pLabelDesc->setText(QString::fromLocal8Bit("THRESH_TRUNC ������ֵ�Ĳ���Ϊ��ֵ  С�ڵı���ԭ��"));
        m_ThresholdTypes = cv::THRESH_TRUNC;
    }
    if(pRadioButton->text() == "THRESH_TOZERO")
    {
        m_pLabelDesc->setText(QString::fromLocal8Bit("THRESH_TOZERO ������ֵ�Ĳ��ֲ���  С�ڵ�Ϊ0"));
        m_ThresholdTypes = cv::THRESH_TOZERO;
    }
    if(pRadioButton->text() == "THRESH_TOZERO_INV")
    {
        m_pLabelDesc->setText(QString::fromLocal8Bit("THRESH_TOZERO_INV ������ֵ�Ĳ���Ϊ0  С�ڵ�Ϊ���治��"));
        m_ThresholdTypes = cv::THRESH_TOZERO_INV;
    }
    if(pRadioButton->text() == "THRESH_MASK")
    {
        m_pLabelDesc->setText(QString::fromLocal8Bit("THRESH_MASK ����255�ı���ԭ����С�ڵ�Ϊ0"));
        m_ThresholdTypes = cv::THRESH_MASK;
    }
    if(pRadioButton->text() == "THRESH_OTSU")
    {
        m_pLabelDesc->setText(QString::fromLocal8Bit("THRESH_OTSU ʹ��OTSUѡȡ�����ֵ"));
        m_ThresholdTypes = cv::THRESH_OTSU;
    }
    if(pRadioButton->text() == "THRESH_TRIANGLE")
    {
        m_pLabelDesc->setText(QString::fromLocal8Bit("THRESH_TRIANGLE ʹ�������㷨ѡȡ�����ֵ"));
        m_ThresholdTypes = cv::THRESH_TRIANGLE;
    }

    OnThresholdParamChanged();
}

void ImageIdentifyWindow::OnThresholdSliderChanged(int value)
{
    m_nSliderValue = value;
    m_pLabelPersent->setText(QString("%1").arg(value));

    OnThresholdParamChanged();
}

void ImageIdentifyWindow::OnThresholdParamChanged()
{
    if( m_matRaw.data == NULL)
        return;

    cv::Mat dst;
    cv::cvtColor(m_matRaw, dst, cv::ColorConversionCodes::COLOR_RGB2GRAY);
    cv::threshold(dst, dst, m_nSliderValue, 255, m_ThresholdTypes);

    LabelDisplayMat(m_pLabelAfter, dst);
}

void ImageIdentifyWindow::AddThresholdStackWidget(QStackedWidget * pStackedWidget)
{
    QDialog * pThresholdWidget = new QDialog(this);
    pThresholdWidget->setWindowTitle("threshold");
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

        QVBoxLayout * pLayoutMain = new QVBoxLayout(pThresholdWidget);

        QGridLayout * pGridLayoutRadioButton = new QGridLayout(pThresholdWidget);

        QRadioButton* pButton_THRESH_BINARY        = new QRadioButton("THRESH_BINARY", pThresholdWidget);
        QRadioButton* pButton_THRESH_BINARY_INV    = new QRadioButton("THRESH_BINARY_INV", pThresholdWidget);
        QRadioButton* pButton_THRESH_TRUNC         = new QRadioButton("THRESH_TRUNC", pThresholdWidget);
        QRadioButton* pButton_THRESH_TOZERO        = new QRadioButton("THRESH_TOZERO", pThresholdWidget);
        QRadioButton* pButton_THRESH_TOZERO_INV    = new QRadioButton("THRESH_TOZERO_INV", pThresholdWidget);
        QRadioButton* pButton_THRESH_MASK          = new QRadioButton("THRESH_MASK", pThresholdWidget);
        QRadioButton* pButton_THRESH_OTSU          = new QRadioButton("THRESH_OTSU", pThresholdWidget);
        QRadioButton* pButton_THRESH_TRIANGLE      = new QRadioButton("THRESH_TRIANGLE", pThresholdWidget);

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

        QHBoxLayout * pLayoutSlider = new QHBoxLayout(pThresholdWidget);
        QSlider * pSlider = new QSlider(Qt::Horizontal , pThresholdWidget);

        m_pLabelPersent = new QLabel("125",pThresholdWidget);
        pSlider->setRange(0, 255);
        pSlider->setValue(125);
        pLayoutSlider->addWidget(pSlider, 9);
        pLayoutSlider->addWidget(m_pLabelPersent, 1);
        connect(pSlider, &QSlider::valueChanged, this, &ImageIdentifyWindow::OnThresholdSliderChanged);

        m_pLabelDesc = new QLabel(pThresholdWidget);

        // ͼƬ������
        QHBoxLayout * pLayoutImage = new QHBoxLayout(pThresholdWidget);
        QLabel * pLabelRaw = new QLabel(pThresholdWidget);
        m_pLabelAfter = new QLabel(pThresholdWidget);
        pLayoutImage->addWidget(pLabelRaw, 5);
        pLayoutImage->addWidget(m_pLabelAfter, 5);

        LabelDisplayMat(pLabelRaw, m_matRaw);
        LabelDisplayMat(m_pLabelAfter, m_matRaw);

        // ������
        pLayoutMain->addLayout(pGridLayoutRadioButton, 2);
        pLayoutMain->addLayout(pLayoutSlider, 1);
        pLayoutMain->addWidget(m_pLabelDesc);
        pLayoutMain->addLayout(pLayoutImage, 6);
        pThresholdWidget->setLayout(pLayoutMain);

        emit pButton_THRESH_BINARY->clicked(true);
        emit pSlider->valueChanged(125);
    }
    pStackedWidget->addWidget(pThresholdWidget);
}

void ImageIdentifyWindow::AddDoubleThresholdStackWidget(QStackedWidget * pStackedWidget)
{
    QDialog * pThresholdWidget = new QDialog(this);
    pThresholdWidget->setWindowTitle(QString::fromLocal8Bit("˫��ֵ"));
    {

        QVBoxLayout * pLayoutMain = new QVBoxLayout(pThresholdWidget);

        QGridLayout * pGridLayoutRadioButton = new QGridLayout(pThresholdWidget);

        QRadioButton* pButton_THRESH_BINARY        = new QRadioButton("THRESH_BINARY", pThresholdWidget);
        QRadioButton* pButton_THRESH_BINARY_INV    = new QRadioButton("THRESH_BINARY_INV", pThresholdWidget);
        QRadioButton* pButton_THRESH_TRUNC         = new QRadioButton("THRESH_TRUNC", pThresholdWidget);
        QRadioButton* pButton_THRESH_TOZERO        = new QRadioButton("THRESH_TOZERO", pThresholdWidget);
        QRadioButton* pButton_THRESH_TOZERO_INV    = new QRadioButton("THRESH_TOZERO_INV", pThresholdWidget);
        QRadioButton* pButton_THRESH_MASK          = new QRadioButton("THRESH_MASK", pThresholdWidget);
        QRadioButton* pButton_THRESH_OTSU          = new QRadioButton("THRESH_OTSU", pThresholdWidget);
        QRadioButton* pButton_THRESH_TRIANGLE      = new QRadioButton("THRESH_TRIANGLE", pThresholdWidget);

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

        QHBoxLayout * pLayoutSlider = new QHBoxLayout(pThresholdWidget);
        QSlider * pSlider = new QSlider(Qt::Horizontal , pThresholdWidget);

        m_pLabelPersent = new QLabel("125",pThresholdWidget);
        pSlider->setRange(0, 255);
        pSlider->setValue(125);
        pLayoutSlider->addWidget(pSlider, 9);
        pLayoutSlider->addWidget(m_pLabelPersent, 1);
        connect(pSlider, &QSlider::valueChanged, this, &ImageIdentifyWindow::OnThresholdSliderChanged);

        m_pLabelDesc = new QLabel(pThresholdWidget);

        // ͼƬ������
        QHBoxLayout * pLayoutImage = new QHBoxLayout(pThresholdWidget);
        QLabel * pLabelRaw = new QLabel(pThresholdWidget);
        m_pLabelAfter = new QLabel(pThresholdWidget);
        pLayoutImage->addWidget(pLabelRaw, 5);
        pLayoutImage->addWidget(m_pLabelAfter, 5);

        LabelDisplayMat(pLabelRaw, m_matRaw);
        LabelDisplayMat(m_pLabelAfter, m_matRaw);

        // ������
        pLayoutMain->addLayout(pGridLayoutRadioButton, 2);
        pLayoutMain->addLayout(pLayoutSlider, 1);
        pLayoutMain->addWidget(m_pLabelDesc);
        pLayoutMain->addLayout(pLayoutImage, 6);
        pThresholdWidget->setLayout(pLayoutMain);

        emit pButton_THRESH_BINARY->clicked(true);
        emit pSlider->valueChanged(125);
    }
    pStackedWidget->addWidget(pThresholdWidget);
}
