#include "filterwindow.h"
#include "ui_filterwindow.h"
#include "common.h"

#include <QMessageBox>



FilterWindow::FilterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FilterWindow)
{
    ui->setupUi(this);

    m_matRaw = LoadImage();

    connect(ui->actionBoxBlur, &QAction::triggered, this, &FilterWindow::BoxBlur);
    connect(ui->actionBlur, &QAction::triggered, this, &FilterWindow::Blur);
    connect(ui->actionGaussianBlur, &QAction::triggered, this, &FilterWindow::GaussianBlur);
    connect(ui->actionMedianBlur, &QAction::triggered, this, &FilterWindow::MedianBlur);
    connect(ui->actionBilateraFilter, &QAction::triggered, this, &FilterWindow::BilateralFilter);

    ui->wSlider->setRange(3, 10);
    ui->hSlider->setRange(3, 10);

    connect(ui->wSlider, &QSlider::valueChanged, this, &FilterWindow::FilterW);
    connect(ui->hSlider, &QSlider::valueChanged, this, &FilterWindow::FilterH);

    connect(ui->actionBoxBlur, &QAction::triggered, this, &FilterWindow::UpdateAction);
    connect(ui->actionBlur, &QAction::triggered, this, &FilterWindow::UpdateAction);
    connect(ui->actionGaussianBlur, &QAction::triggered, this, &FilterWindow::UpdateAction);
    connect(ui->actionMedianBlur, &QAction::triggered, this, &FilterWindow::UpdateAction);
    connect(ui->actionBilateraFilter, &QAction::triggered, this, &FilterWindow::UpdateAction);

    LabelDisplayMat(ui->labelRaw, m_matRaw);
    LabelDisplayMat(ui->labelAfter, m_matRaw);
}

FilterWindow::~FilterWindow()
{
    delete ui;
}

void FilterWindow::FilterW(int w)
{
    m_w = w;
    m_w = m_w % 2 == 1 ? m_w : m_w + 1;
    ui->labelWSlider->setText(QString("%1").arg(m_w));

    switch (m_FilterType)
    {
    case FT_BoxBlur:
        BoxBlur();             break;
    case FT_Blur:               Blur();                break;
    case FT_GaussianBlur:       GaussianBlur();        break;
    case FT_MedianBlur:         MedianBlur();          break;
    case FT_BilateralFilter:    BilateralFilter();     break;

    default:
        break;
    }
}

void FilterWindow::FilterH(int h)
{
    m_h = h;
    m_h = m_h % 2 == 1 ? m_h : m_h + 1;
    ui->labelHSlider->setText(QString("%1").arg(m_h));

    switch (m_FilterType)
    {
    case FT_BoxBlur:            BoxBlur();             break;
    case FT_Blur:               Blur();                break;
    case FT_GaussianBlur:       GaussianBlur();        break;
    case FT_MedianBlur:         MedianBlur();          break;
    case FT_BilateralFilter:    BilateralFilter();     break;

    default:
        break;
    }
}

void FilterWindow::UpdateAction()
{

    ui->actionBoxBlur->setChecked(false);
    ui->actionBlur->setChecked(false);
    ui->actionGaussianBlur->setChecked(false);
    ui->actionMedianBlur->setChecked(false);
    ui->actionBilateraFilter->setChecked(false);

    ((QAction*) sender())->setChecked(true);
}

/*
 * void boxFilter(InputArray src,
            OutputArray dst,
            int ddepth,
            Size ksize,
            Point anchor=Point(-1, -1),
            bool normalize = true,
            int borderType = BORDER_DEFALT)

* 参数一：InputArray类型，一般是cv::Mat，且可以处理任何通道数的图片。但需要注意，待处理的图片深度应该为CV_8U、CV_16U、CV_16S、CV_32F、CV_64F中的一个」
        参数二；OutputArray类型，输出的目标图像，需要和原图片有一样的尺寸和类型」
            参数三：int类型，表示输出图像的深度Ｑ代表使用原图深度」
                参数四：Size类型的ksize，内核的大小。一般用Size(w,h)来表示内核的大小，Size(3,3)就表礸3的核大小」
    参数五：Point类型，表示锚点（值被平滑的那个点）。注意：默认值Point(-1,-1)。如果点是负值，就表示取核的中心为锚点」
    参数六：bool类型normalize，默认为true，表示内核是否被其区域归一化」
    参数七：int类型的borderType，用于推断图像外部像素的某种边界模式，使用默认值BORDER_DEFULAT，一般无需使用」
*/

void FilterWindow::BoxBlur()
{
    m_FilterType = FT_BoxBlur;
    this->setWindowTitle("BoxBlur");

    ui->textBrowser->setText(QString::fromLocal8Bit("它是将所选择的卷积区域进行加权平均来代替指定的像素点，"
                                                  "比如默认的是中心点，3X3的卷积核将对周边8个像素加权和来代替。"));

    ui->wSlider->setEnabled(true);
    ui->hSlider->setEnabled(true);

    cv::Mat dst;
    cv::boxFilter(m_matRaw, dst, -1, cv::Size(m_w, m_h));

    LabelDisplayMat(ui->labelAfter, dst);
}

/*
 * 均值滤波的函数原型如下－

void blur( InputArray src,
          OutputArray dst,
          Size ksize,
          Point anchor = Point(-1,-1),
          int borderType = BORDER_DEFAULT );
参数一：InputArray类型，一般是cv::Mat，且可以处理任何通道数的图片。但需要注意，待处理的图片深度应该为CV_8U、CV_16U、CV_16S、CV_32F、CV_64F中的一个」
        参数二；OutputArray类型，输出的目标图像，需要和原图片有一样的尺寸和类型」
            参数三：Size类型的ksize，核算子的大小。一般用Size(w,h)来表示核算子的大小，Size(3,3)就表礸3的核算子大小」
    参数四：Point类型，表示锚点（值被平滑的那个点）。注意：默认值Point(-1,-1)。如果点是负值，就表示取核的中心为锚点」
    参数五：int类型的borderType，用于推断图像外部像素的某种边界模式，使用默认值BORDER_DEFULAT，一般无需使用」
*/

void FilterWindow::Blur()
{
    this->setWindowTitle("Blur");
    m_FilterType = FT_Blur;
    ui->textBrowser->setText(QString::fromLocal8Bit("blur使用归一化框过滤器模糊图像，"
                                                  "是一种简单的模糊函数，是计算每个像素中对应核的平均值。"));

    ui->wSlider->setEnabled(true);
    ui->hSlider->setEnabled(true);

    cv::Mat dst;
    cv::blur(m_matRaw, dst, cv::Size(m_w, m_h));

    LabelDisplayMat(ui->labelAfter, dst);
}

/*
 *
*/
void FilterWindow::GaussianBlur()
{
    this->setWindowTitle("GaussianBlur");

    m_FilterType = FT_GaussianBlur;
    ui->textBrowser->setText(QString::fromLocal8Bit("类似于方框滤波的加权平均，使用一个模板对整张图进行操作，"
                                                  "视觉效果就像是透过半透膜看东西，我们平常所说的高斯模糊就是高斯低通的操作。"));
    ui->wSlider->setEnabled(true);
    ui->hSlider->setEnabled(true);

    cv::Mat dst;
    cv::GaussianBlur(m_matRaw, dst, cv::Size(m_w, m_h), 0);

    LabelDisplayMat(ui->labelAfter, dst);
}
void FilterWindow::MedianBlur()
{
    this->setWindowTitle("MedianBlur");

    m_FilterType = FT_MedianBlur;
    ui->textBrowser->setText(QString::fromLocal8Bit("基本原理是把数字图像或数字序列中一点的值用该点的一个邻域中各点值的中值代，"
                                                  "因为它不依赖于邻域内那些与典型值差别很大的值，在去除脉冲噪声、椒盐噪声的同时又能保留图像边缘细节。"
                                                  "克服了图像细节模糊，而且对滤除脉冲干扰及图像扫描噪声非常有效，也常用于保护边缘信息, 保存边。"
                                                  "但处理时间和效率付出较高。并且不适合细节较多的图像。"));

    ui->wSlider->setEnabled(true);
    ui->hSlider->setEnabled(false);

    cv::Mat dst;
    cv::medianBlur(m_matRaw, dst, m_w);

    LabelDisplayMat(ui->labelAfter, dst);
}
void FilterWindow::BilateralFilter()
{
    this->setWindowTitle("BilateralFilter");

    ui->textBrowser->setText(QString::fromLocal8Bit("结合图像的空间邻近度和像素值相似度的一种折衷处理，同时考虑空域信息和灰度相似性，达到保边去噪的目的。"
                                                  "双边滤波器的好处是可以做边缘保存双边滤波器顾名思义比高斯滤波多了一个高斯方差sigma－d，"
                                                  "它是基于空间分布的高斯滤波函数，所以在边缘附近，离的较远的像素不会太多影响到边缘上的像素值，"
                                                  "这样就保证了边缘附近像素值的保存。"
                                                  "但是由于保存了过多的高频信息，对于彩色图像里的高频噪声，"
                                                  "双边滤波器不能够干净的滤掉，只能够对于低频信息进行较好的滤波。"));

    ui->wSlider->setEnabled(true);
    ui->hSlider->setEnabled(false);

    m_FilterType = FT_BilateralFilter;

    cv::Mat dst;
    cv::medianBlur(m_matRaw, dst, m_w);

    LabelDisplayMat(ui->labelAfter, dst);
}

