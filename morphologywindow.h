#ifndef MORPHOLOGYWINDOW_H
#define MORPHOLOGYWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>


namespace Ui {
class MorphologyWindow;
}

class MorphologyWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum KenalType
    {
        KT_MORPH_RECT,
        KT_MORPH_CROSS,
        KT_MORPH_ELLIPSE,
    };

    enum FilterType{
        FT_MORPH_ERODE    = 0, //��ʴ
        FT_MORPH_DILATE   = 1, //����
        FT_MORPH_OPEN     = 2, //������
        FT_MORPH_CLOSE    = 3, //�ղ���
        FT_MORPH_GRADIENT = 4, //�ݶȲ���
        FT_MORPH_TOPHAT   = 5, //��ñ����
        FT_MORPH_BLACKHAT = 6, //��ñ����
        FT_MORPH_HITMISS  = 7
    };

    FilterType m_FilterType;
    KenalType m_KenalType;
    int m_nKSize;


public:
    explicit MorphologyWindow(QWidget *parent = nullptr);
    ~MorphologyWindow();

    void OnParamChanged();

private slots:
    void OnKSizeChanged(int ksize);

    void OnKTypeChanged();

    void MorphDilate();
    void MorphErode();
    void MorphOpen();
    void MorphClose();
    void MorphGradient();
    void MorphTopHat();
    void MorphBlackHat();


private:
    Ui::MorphologyWindow *ui;

    cv::Mat m_matRaw;
};

#endif // MORPHOLOGYWINDOW_H
