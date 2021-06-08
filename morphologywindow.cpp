#include "morphologywindow.h"
#include "ui_morphologywindow.h"
#include "common.h"

#include <QSlider>

/*
 * //! type of morphological operation
enum MorphTypes{
    MORPH_ERODE    = 0, //!< see #erode
    MORPH_DILATE   = 1, //!< see #dilate
    MORPH_OPEN     = 2, //!< an opening operation
                        //!< \f[\texttt{dst} = \mathrm{open} ( \texttt{src} , \texttt{element} )= \mathrm{dilate} ( \mathrm{erode} ( \texttt{src} , \texttt{element} ))\f]
    MORPH_CLOSE    = 3, //!< a closing operation
                        //!< \f[\texttt{dst} = \mathrm{close} ( \texttt{src} , \texttt{element} )= \mathrm{erode} ( \mathrm{dilate} ( \texttt{src} , \texttt{element} ))\f]
    MORPH_GRADIENT = 4, //!< a morphological gradient
                        //!< \f[\texttt{dst} = \mathrm{morph\_grad} ( \texttt{src} , \texttt{element} )= \mathrm{dilate} ( \texttt{src} , \texttt{element} )- \mathrm{erode} ( \texttt{src} , \texttt{element} )\f]
    MORPH_TOPHAT   = 5, //!< "top hat"
                        //!< \f[\texttt{dst} = \mathrm{tophat} ( \texttt{src} , \texttt{element} )= \texttt{src} - \mathrm{open} ( \texttt{src} , \texttt{element} )\f]
    MORPH_BLACKHAT = 6, //!< "black hat"
                        //!< \f[\texttt{dst} = \mathrm{blackhat} ( \texttt{src} , \texttt{element} )= \mathrm{close} ( \texttt{src} , \texttt{element} )- \texttt{src}\f]
    MORPH_HITMISS  = 7  //!< "hit or miss"
                        //!<   .- Only supported for CV_8UC1 binary images. A tutorial can be found in the documentation
};
*/


MorphologyWindow::MorphologyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MorphologyWindow)
{
    ui->setupUi(this);

    m_FilterType = FT_MORPH_DILATE;
    m_KenalType = KT_MORPH_RECT;

    connect(ui->rbMORPH_RECT, &QRadioButton::clicked, this, &MorphologyWindow::OnKTypeChanged);
    connect(ui->rbMORPH_CROSS, &QRadioButton::clicked, this, &MorphologyWindow::OnKTypeChanged);
    connect(ui->rbMORPH_ELLIPSE, &QRadioButton::clicked, this, &MorphologyWindow::OnKTypeChanged);

    connect(ui->actionopen, &QAction::triggered, this, &MorphologyWindow::MorphOpen);
    connect(ui->actionclose, &QAction::triggered, this, &MorphologyWindow::MorphClose);
    connect(ui->actionerode, &QAction::triggered, this, &MorphologyWindow::MorphErode);
    connect(ui->actionTopHat, &QAction::triggered, this, &MorphologyWindow::MorphTopHat);
    connect(ui->actiondilate, &QAction::triggered, this, &MorphologyWindow::MorphDilate);
    connect(ui->actionBlackHat, &QAction::triggered, this, &MorphologyWindow::MorphBlackHat);
    connect(ui->actionGradient, &QAction::triggered, this, &MorphologyWindow::MorphGradient);

    connect(ui->kSlider, &QSlider::valueChanged, this, &MorphologyWindow::OnKSizeChanged);

    m_matRaw = LoadImage();

    LabelDisplayMat(ui->labelRaw, m_matRaw);
    this->setWindowTitle("Dilate");

    ui->kSlider->setRange(3, 19);

    ui->rbMORPH_RECT->setChecked(true);

}

MorphologyWindow::~MorphologyWindow()
{
    delete ui;
}



void MorphologyWindow::OnParamChanged()
{
    if(m_nKSize < 3)
        m_nKSize = 3;

    cv::Mat kenal = cv::getStructuringElement(m_KenalType, cv::Size(m_nKSize, m_nKSize));

    cv::Mat dst;

    // cv::erode(m_matRaw, dst, kenal);
    // cv::dilate(m_matRaw, dst, kenal);

    cv::morphologyEx(m_matRaw, dst, m_FilterType , kenal);

    LabelDisplayMat(ui->labelAfter, dst);
}

void MorphologyWindow::OnKSizeChanged(int ksize)
{
    if(m_matRaw.data == NULL)
        return;

    ksize = ksize % 2 == 1 ? ksize : ksize + 1;
    ui->labelPersent->setText(QString("%1").arg(ksize));

    m_nKSize = ksize;

    OnParamChanged();
}

void MorphologyWindow::OnKTypeChanged()
{
    QRadioButton * pSender = (QRadioButton * )sender();

    if(pSender == ui->rbMORPH_RECT)
        m_KenalType = KT_MORPH_RECT;
    if(pSender == ui->rbMORPH_CROSS)
        m_KenalType = KT_MORPH_CROSS;
    if(pSender == ui->rbMORPH_ELLIPSE)
        m_KenalType = KT_MORPH_ELLIPSE;

    OnParamChanged();

}

/*
����(dilate)����ֲ����ֵ�Ĳ����������ݽǶ���˵�������ǽ�ͼ����˽��о����ʵ���˶�Ŀ�����ص������չ��Ŀ�ġ�
�����ӿ������κ���״�ʹ�С����ӵ��һ��������������Ĳο��㣬��ʹ֮ǰ�ᵽ��ê��(anchorpoint)��
��������£���һ���С��֮ǰ���õ��ľ���3x3�����ӣ�Ȼ�����ê��Point(-1,-1)ʵ�ʾ������ĵ㡣
    ����ͼ��A��������B��A��B�������B���ǵ���������ص�����ֵ��
����������ֵ��ֵ��ָ���㣨�������ڵ�ê�㣩��������ʹͼ���еĸ�������������

void dilate( InputArray src,
            OutputArray dst,
            InputArray kernel,
            Point anchor = Point(-1,-1),
            int iterations = 1,
            int borderType = BORDER_CONSTANT,
            const Scalar& borderValue = morphologyDefaultBorderValue() );
����һ��InputArray���ͣ�һ����cv::Mat��ͨ����������ģ�������ȱ���ΪCV_8U��CV_16U��CV_16S��CV_32F��CV_64F��
    ��������OutputArray���ͣ������Ŀ��ͼ�񣬺�ԭͼ����һ���ĳߴ�����ͣ�
        ���������������͵��ں˽ṹԪ�أ����elemenat=Mat()����Ϊ3 x 3����ʹ�ýṹ��Ԫ�ء��ں˿���ʹ��getStructuringElement������
          �����ģ�Ԫ����ê���ê��λ�ã�Ĭ��ֵ��-1��-1����ʾê���ڹ�������
      �����壺����������ʾִ�����Ͳ����Ĵ�����
          ��������int���͵�borderType�������ƶ�ͼ���ⲿ���ص�ĳ�ֱ߽�ģʽ��ʹ��Ĭ��ֵBORDER_DEFULAT��һ���������ã�
              �����ߣ�����߽�Ϊ��������߽�ֵΪborderValue��
      ������Ĳ���3�У���Ҫ�ں˽ṹԪ�أ�������ĺ���������

          Mat getStructuringElement(int shape,
                                Size ksize,
                                Point anchor = Point(-1,-1));
����һ��Ԫ����״��������MORPH_RECT��MORPH_CROSS��MORPH_ELLIPSE��
        ��������ksize�ṹԪ�صĴ�С��������ڵ���1��
            ��������Ԫ���ڵ�ê��λ�á�Ĭ��ֵPoint(-1,-1)��ʾê���м�
    ��ע�⣬ֻ��ʮ���ι�������״ȡ����êλ�á�����������£�êֻ�ǵ�����̬�Ľ������ת�ƣ���
*/
void MorphologyWindow::MorphDilate()
{
    this->setWindowTitle("Dilate ����");

    m_FilterType = FT_MORPH_DILATE;
    OnParamChanged();

}

/*
 * ��ʴ(dilate)������(erode)��һ���෴�Ĳ�������������ֲ����ֵ�Ĳ�������ô���;�����ֲ���Сֵ�Ĳ�����
�����ӿ������κ���״�ʹ�С����ӵ��һ��������������Ĳο��㣬��ʹ֮ǰ�ᵽ��ê��(anchorpoint)��
��������£���һ���С��֮ǰ���õ��ľ���3x3�����ӣ�Ȼ�����ê��Point(-1,-1)ʵ�ʾ������ĵ㡣
    ����ͼ��A��������B��A��B�������B���ǵ���������ص����Сֵ������������ֵ��ֵ��ָ���㣨�������ڵ�ê�㣩��
������ʹͼ���еĸ�������������

void erode( InputArray src,
           OutputArray dst,
           InputArray kernel,
           Point anchor = Point(-1,-1),
           int iterations = 1,
           int borderType = BORDER_CONSTANT,
           const Scalar& borderValue = morphologyDefaultBorderValue() );

����һ��InputArray���ͣ�һ����cv::Mat��ͨ����������ģ�������ȱ���ΪCV_8U��CV_16U��CV_16S��CV_32F��CV_64F��
    ��������OutputArray���ͣ������Ŀ��ͼ�񣬺�ԭͼ����һ���ĳߴ�����ͣ�
        ���������������͵��ں˽ṹԪ�أ����elemenat=Mat()����Ϊ3 x 3����ʹ�ýṹ��Ԫ�ء��ں˿���ʹ��getStructuringElement������
          �����ģ�Ԫ����ê���ê��λ�ã�Ĭ��ֵ��-1��-1����ʾê���ڹ�������
      �����壺����������ʾִ�и�ʴ�����Ĵ�����
          ��������int���͵�borderType�������ƶ�ͼ���ⲿ���ص�ĳ�ֱ߽�ģʽ��ʹ��Ĭ��ֵBORDER_DEFULAT��һ���������ã�
              �����ߣ�����߽�Ϊ��������߽�ֵΪborderValue��
                  ������Ĳ���3�У���Ҫ�ں˽ṹԪ�أ�������ĺ���������
                      Mat getStructuringElement(int shape,
                                            Size ksize,
                                            Point anchor = Point(-1,-1));
����һ��Ԫ����״��������MORPH_RECT��MORPH_CROSS��MORPH_ELLIPSE��
        ��������ksize�ṹԪ�صĴ�С��������ڵ���1��
            ��������Ԫ���ڵ�ê��λ�á�Ĭ��ֵPoint(-1,-1)��ʾê���м�
    ��ע�⣬ֻ��ʮ���ι�������״ȡ����êλ�á�����������£�êֻ�ǵ�����̬�Ľ������ת�ƣ���
*/
void MorphologyWindow::MorphErode()
{
    this->setWindowTitle("Erode ��ʴ");

    m_FilterType = FT_MORPH_ERODE;
    OnParamChanged();
}

/*
 * ������(Opening Operation)�����ȸ�ʴ�����͵Ĺ��̡�
?    **�����������������С���壬����ϸ�㴦�������壬
������ƽ���ϴ�����ı߽��ͬʱ�����Ըı��������ȥ��������ƽ��Ŀ��߽磩��**

�ȸ�ʴ�����ڽ�С�Ĳ��ָ�ʴ���û���ˣ������ͣ���������һ���ˣ����Ծʹﵽ��ȥ��С�����Ŀ�ġ�
���Գ�֮Ϊ������

*/
void MorphologyWindow::MorphOpen()
{
    this->setWindowTitle("Open ������");

    m_FilterType = FT_MORPH_OPEN;
    OnParamChanged();
}

/*�����ͺ�ʴ�Ĳ�����
 * ��������������С�ѷ죬����С�ͺڶ���������ƽ���ϴ�����ı߽��ͬʱ�����Ըı��������
 *
 * �����ͣ��Ͱ�С�ĵ�����Χ����������ˣ��ڸ�ʴ������С���ѷ�������
 * �����������Ŀ�������ڵ���ɢС�ն��ͷ�ɢ���֡�
 *
*/
void MorphologyWindow::MorphClose()
{
    this->setWindowTitle("Close ������");

    m_FilterType = FT_MORPH_CLOSE;
    OnParamChanged();
}

/*
 * ��̬ѧ�ݶ�
 * ��̬ѧ�ݶ�(Morphological Gradient)�����ͣ����������͸�ʴ���������Ĳ�𣬽������ȥ����ǰ�������������
*/
void MorphologyWindow::MorphGradient()
{
    this->setWindowTitle("Gradient �ݶȲ���");

    m_FilterType = FT_MORPH_GRADIENT;
    OnParamChanged();
}

/*
 * ��ñ����(Top Hat)�ֳ�������ñ�����㣬��ԭͼ�񣨼������롰�����㡱�����������Ľ��ͼ֮�
    ����������Ľ���ǷŴ����ѷ���߾ֲ��������ȵ�������ˣ���ԭͼ�м�ȥ��������ͼ��
�õ���Ч��ͻ���˱�ԭͼ������Χ���������������������һ������ѡ��ĺ˵Ĵ�С��ء�
    ��ñ������������������ڽ�����һЩ�İ߿顣
��һ��ͼ����д���ı�������΢С��Ʒ�Ƚ��й��ɵ�����£�����ʹ�ö�ñ������б�����ȡ��
*/
void MorphologyWindow::MorphTopHat()
{
    this->setWindowTitle("TopHat ��ñ����");

    m_FilterType = FT_MORPH_TOPHAT;
    OnParamChanged();
}

/*
 *   ��ñ(Black Hat)�����Ǳ����㣨���������Ľ��ͼ��ԭͼ�񣨼�����֮�
    ��ñ������Ч��ͻ���˱�ԭͼ������Χ�������������������һ������ѡ��ĺ˴�С��ء�
    �����붥ñ�����෴����ñ����������������ڽ��㰵һЩ�İ߿飬Ч��ͼ���ŷǳ�������������
*/
void MorphologyWindow::MorphBlackHat()
{
    this->setWindowTitle("BlackHat ��ñ����");

    m_FilterType = FT_MORPH_BLACKHAT;
    OnParamChanged();
}
