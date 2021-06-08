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
膨胀(dilate)是求局部最大值的操作，从数据角度来说，膨胀是将图像与核进行卷积，实现了对目标像素点进行扩展的目的。
核算子可以是任何形状和大小，它拥有一个单独定义出来的参考点，即使之前提到的锚点(anchorpoint)。
多数情况下，核一般很小，之前常用到的就是3x3核算子，然后带有锚点Point(-1,-1)实际就是中心点。
    当有图像A，核算子B，A与B卷积计算B覆盖的区域的像素点的最大值，
并把这个最大值赋值给指定点（核区域内的锚点），这样会使图像中的高亮区域逐渐增大。

void dilate( InputArray src,
            OutputArray dst,
            InputArray kernel,
            Point anchor = Point(-1,-1),
            int iterations = 1,
            int borderType = BORDER_CONSTANT,
            const Scalar& borderValue = morphologyDefaultBorderValue() );
参数一：InputArray类型，一般是cv::Mat，通道数是任意的，但是深度必须为CV_8U、CV_16U、CV_16S、CV_32F或CV_64F；
    参数二；OutputArray类型，输出的目标图像，和原图像有一样的尺寸和类型；
        参数三：用于膨胀的内核结构元素；如果elemenat=Mat()，则为3 x 3矩形使用结构化元素。内核可以使用getStructuringElement创建；
          参数四：元素中锚点的锚点位置；默认值（-1，-1）表示锚定在构件中心
      参数五：迭代次数表示执行膨胀操作的次数；
          参数六：int类型的borderType，用于推断图像外部像素的某种边界模式，使用默认值BORDER_DEFULAT，一般无需设置；
              参数七：如果边界为常量，则边界值为borderValue。
      在上面的参数3中，需要内核结构元素，由下面的函数创建：

          Mat getStructuringElement(int shape,
                                Size ksize,
                                Point anchor = Point(-1,-1));
参数一：元素形状，可以是MORPH_RECT、MORPH_CROSS和MORPH_ELLIPSE；
        参数二：ksize结构元素的大小，必须大于等于1；
            参数三：元件内的锚定位置。默认值Point(-1,-1)表示锚在中间
    （注意，只有十字形构件的形状取决于锚位置。在其他情况下，锚只是调节形态的结果操作转移）；
*/
void MorphologyWindow::MorphDilate()
{
    this->setWindowTitle("Dilate 膨胀");

    m_FilterType = FT_MORPH_DILATE;
    OnParamChanged();

}

/*
 * 腐蚀(dilate)与膨胀(erode)是一对相反的操作，膨胀是求局部最大值的操作，那么膨胀就是求局部最小值的操作。
核算子可以是任何形状和大小，它拥有一个单独定义出来的参考点，即使之前提到的锚点(anchorpoint)。
多数情况下，核一般很小，之前常用到的就是3x3核算子，然后带有锚点Point(-1,-1)实际就是中心点。
    当有图像A，核算子B，A与B卷积计算B覆盖的区域的像素点的最小值，并把这个最大值赋值给指定点（核区域内的锚点），
这样会使图像中的高亮区域逐渐增大。

void erode( InputArray src,
           OutputArray dst,
           InputArray kernel,
           Point anchor = Point(-1,-1),
           int iterations = 1,
           int borderType = BORDER_CONSTANT,
           const Scalar& borderValue = morphologyDefaultBorderValue() );

参数一：InputArray类型，一般是cv::Mat，通道数是任意的，但是深度必须为CV_8U、CV_16U、CV_16S、CV_32F或CV_64F；
    参数二；OutputArray类型，输出的目标图像，和原图像有一样的尺寸和类型；
        参数三：用于膨胀的内核结构元素；如果elemenat=Mat()，则为3 x 3矩形使用结构化元素。内核可以使用getStructuringElement创建；
          参数四：元素中锚点的锚点位置；默认值（-1，-1）表示锚定在构件中心
      参数五：迭代次数表示执行腐蚀操作的次数；
          参数六：int类型的borderType，用于推断图像外部像素的某种边界模式，使用默认值BORDER_DEFULAT，一般无需设置；
              参数七：如果边界为常量，则边界值为borderValue；
                  在上面的参数3中，需要内核结构元素，由下面的函数创建：
                      Mat getStructuringElement(int shape,
                                            Size ksize,
                                            Point anchor = Point(-1,-1));
参数一：元素形状，可以是MORPH_RECT、MORPH_CROSS和MORPH_ELLIPSE；
        参数二：ksize结构元素的大小，必须大于等于1；
            参数三：元件内的锚定位置。默认值Point(-1,-1)表示锚在中间
    （注意，只有十字形构件的形状取决于锚位置。在其他情况下，锚只是调节形态的结果操作转移）；
*/
void MorphologyWindow::MorphErode()
{
    this->setWindowTitle("Erode 腐蚀");

    m_FilterType = FT_MORPH_ERODE;
    OnParamChanged();
}

/*
 * 开运算(Opening Operation)就是先腐蚀后膨胀的过程。
?    **开运算可以用来消除小物体，在纤细点处分离物体，
并且在平滑较大物体的边界的同时不明显改变其面积（去除噪声及平滑目标边界）。**

先腐蚀，对于较小的部分腐蚀后就没有了，再膨胀，就连接在一起了，所以就达到了去除小物体的目的。
所以称之为开运算

*/
void MorphologyWindow::MorphOpen()
{
    this->setWindowTitle("Open 开运算");

    m_FilterType = FT_MORPH_OPEN;
    OnParamChanged();
}

/*先膨胀后腐蚀的操作。
 * 闭运算可以用拟合小裂缝，消除小型黑洞，并且在平滑较大物体的边界的同时不明显改变其面积。
 *
 * 先膨胀，就把小的点用周围的像素填充了，在腐蚀，则会把小型裂缝消除。
 * 闭运算能填充目标区域内的离散小空洞和分散部分。
 *
*/
void MorphologyWindow::MorphClose()
{
    this->setWindowTitle("Close 闭运算");

    m_FilterType = FT_MORPH_CLOSE;
    OnParamChanged();
}

/*
 * 形态学梯度
 * 形态学梯度(Morphological Gradient)是膨胀（被减数）和腐蚀（减数）的差别，结果看上去就像前景物体的轮廓。
*/
void MorphologyWindow::MorphGradient()
{
    this->setWindowTitle("Gradient 梯度操作");

    m_FilterType = FT_MORPH_GRADIENT;
    OnParamChanged();
}

/*
 * 顶帽运算(Top Hat)又称作“礼帽”运算，是原图像（减数）与“开运算”（被减数）的结果图之差。
    开运算带来的结果是放大了裂缝或者局部降低亮度的区域。因此，从原图中减去开运算后的图，
得到的效果突出了比原图轮廓周围的区域更明亮的区域，且这一操作与选择的核的大小相关。
    顶帽运算往往用来分离比邻近点亮一些的斑块。
在一幅图像具有大幅的背景，而微小物品比较有规律的情况下，可以使用顶帽运算进行背景提取。
*/
void MorphologyWindow::MorphTopHat()
{
    this->setWindowTitle("TopHat 顶帽操作");

    m_FilterType = FT_MORPH_TOPHAT;
    OnParamChanged();
}

/*
 *   黑帽(Black Hat)运算是闭运算（被减数）的结果图与原图像（减数）之差。
    黑帽运算后的效果突出了比原图经过周围的区域更暗的区域，且这一操作和选择的核大小相关。
    所以与顶帽操作相反，黑帽操作是用来分离比邻近点暗一些的斑块，效果图有着非常完美的轮廓。
*/
void MorphologyWindow::MorphBlackHat()
{
    this->setWindowTitle("BlackHat 黑帽操作");

    m_FilterType = FT_MORPH_BLACKHAT;
    OnParamChanged();
}
