#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("open image"),".",tr("Image file(*.png *.jpg *.bmp)"));
    cv::Mat image = cv::imread(filename.toLocal8Bit().data());
    QImage Qtemp = QImage((const unsigned char*)(image.data), image.cols, image.rows, image.step, QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(Qtemp));
}
