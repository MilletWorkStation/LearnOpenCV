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

* ����һ��InputArray���ͣ�һ����cv::Mat���ҿ��Դ����κ�ͨ������ͼƬ������Ҫע�⣬�������ͼƬ���Ӧ��ΪCV_8U��CV_16U��CV_16S��CV_32F��CV_64F�е�һ����
        ��������OutputArray���ͣ������Ŀ��ͼ����Ҫ��ԭͼƬ��һ���ĳߴ�����͡�
            ��������int���ͣ���ʾ���ͼ�����ȣѴ���ʹ��ԭͼ��ȡ�
                �����ģ�Size���͵�ksize���ں˵Ĵ�С��һ����Size(w,h)����ʾ�ں˵Ĵ�С��Size(3,3)�ͱ�g3�ĺ˴�С��
    �����壺Point���ͣ���ʾê�㣨ֵ��ƽ�����Ǹ��㣩��ע�⣺Ĭ��ֵPoint(-1,-1)��������Ǹ�ֵ���ͱ�ʾȡ�˵�����Ϊê�㡹
    ��������bool����normalize��Ĭ��Ϊtrue����ʾ�ں��Ƿ��������һ����
    �����ߣ�int���͵�borderType�������ƶ�ͼ���ⲿ���ص�ĳ�ֱ߽�ģʽ��ʹ��Ĭ��ֵBORDER_DEFULAT��һ������ʹ�á�
*/

void FilterWindow::BoxBlur()
{
    m_FilterType = FT_BoxBlur;
    this->setWindowTitle("BoxBlur");

    ui->textBrowser->setText(QString::fromLocal8Bit("���ǽ���ѡ��ľ��������м�Ȩƽ��������ָ�������ص㣬"
                                                  "����Ĭ�ϵ������ĵ㣬3X3�ľ���˽����ܱ�8�����ؼ�Ȩ�������档"));

    ui->wSlider->setEnabled(true);
    ui->hSlider->setEnabled(true);

    cv::Mat dst;
    cv::boxFilter(m_matRaw, dst, -1, cv::Size(m_w, m_h));

    LabelDisplayMat(ui->labelAfter, dst);
}

/*
 * ��ֵ�˲��ĺ���ԭ�����£�

void blur( InputArray src,
          OutputArray dst,
          Size ksize,
          Point anchor = Point(-1,-1),
          int borderType = BORDER_DEFAULT );
����һ��InputArray���ͣ�һ����cv::Mat���ҿ��Դ����κ�ͨ������ͼƬ������Ҫע�⣬�������ͼƬ���Ӧ��ΪCV_8U��CV_16U��CV_16S��CV_32F��CV_64F�е�һ����
        ��������OutputArray���ͣ������Ŀ��ͼ����Ҫ��ԭͼƬ��һ���ĳߴ�����͡�
            ��������Size���͵�ksize�������ӵĴ�С��һ����Size(w,h)����ʾ�����ӵĴ�С��Size(3,3)�ͱ�g3�ĺ����Ӵ�С��
    �����ģ�Point���ͣ���ʾê�㣨ֵ��ƽ�����Ǹ��㣩��ע�⣺Ĭ��ֵPoint(-1,-1)��������Ǹ�ֵ���ͱ�ʾȡ�˵�����Ϊê�㡹
    �����壺int���͵�borderType�������ƶ�ͼ���ⲿ���ص�ĳ�ֱ߽�ģʽ��ʹ��Ĭ��ֵBORDER_DEFULAT��һ������ʹ�á�
*/

void FilterWindow::Blur()
{
    this->setWindowTitle("Blur");
    m_FilterType = FT_Blur;
    ui->textBrowser->setText(QString::fromLocal8Bit("blurʹ�ù�һ���������ģ��ͼ��"
                                                  "��һ�ּ򵥵�ģ���������Ǽ���ÿ�������ж�Ӧ�˵�ƽ��ֵ��"));

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
    ui->textBrowser->setText(QString::fromLocal8Bit("�����ڷ����˲��ļ�Ȩƽ����ʹ��һ��ģ�������ͼ���в�����"
                                                  "�Ӿ�Ч��������͸����͸Ĥ������������ƽ����˵�ĸ�˹ģ�����Ǹ�˹��ͨ�Ĳ�����"));
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
    ui->textBrowser->setText(QString::fromLocal8Bit("����ԭ���ǰ�����ͼ�������������һ���ֵ�øõ��һ�������и���ֵ����ֵ����"
                                                  "��Ϊ������������������Щ�����ֵ���ܴ��ֵ����ȥ����������������������ͬʱ���ܱ���ͼ���Եϸ�ڡ�"
                                                  "�˷���ͼ��ϸ��ģ�������Ҷ��˳�������ż�ͼ��ɨ�������ǳ���Ч��Ҳ�����ڱ�����Ե��Ϣ, ����ߡ�"
                                                  "������ʱ���Ч�ʸ����ϸߡ����Ҳ��ʺ�ϸ�ڽ϶��ͼ��"));

    ui->wSlider->setEnabled(true);
    ui->hSlider->setEnabled(false);

    cv::Mat dst;
    cv::medianBlur(m_matRaw, dst, m_w);

    LabelDisplayMat(ui->labelAfter, dst);
}
void FilterWindow::BilateralFilter()
{
    this->setWindowTitle("BilateralFilter");

    ui->textBrowser->setText(QString::fromLocal8Bit("���ͼ��Ŀռ��ڽ��Ⱥ�����ֵ���ƶȵ�һ�����Դ���ͬʱ���ǿ�����Ϣ�ͻҶ������ԣ��ﵽ����ȥ���Ŀ�ġ�"
                                                  "˫���˲����ĺô��ǿ�������Ե����˫���˲�������˼��ȸ�˹�˲�����һ����˹����sigma��d��"
                                                  "���ǻ��ڿռ�ֲ��ĸ�˹�˲������������ڱ�Ե��������Ľ�Զ�����ز���̫��Ӱ�쵽��Ե�ϵ�����ֵ��"
                                                  "�����ͱ�֤�˱�Ե��������ֵ�ı��档"
                                                  "�������ڱ����˹���ĸ�Ƶ��Ϣ�����ڲ�ɫͼ����ĸ�Ƶ������"
                                                  "˫���˲������ܹ��ɾ����˵���ֻ�ܹ����ڵ�Ƶ��Ϣ���нϺõ��˲���"));

    ui->wSlider->setEnabled(true);
    ui->hSlider->setEnabled(false);

    m_FilterType = FT_BilateralFilter;

    cv::Mat dst;
    cv::medianBlur(m_matRaw, dst, m_w);

    LabelDisplayMat(ui->labelAfter, dst);
}

