#include "mainview.h"
#include "ui_mainview.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

MainView::MainView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainView)
{
    ui->setupUi(this);
    // read an image
    cv::Mat image = cv::imread("../small.png");
    // create an image window named "My Image" and show it
    //cv::namedWindow("My Image");
    //cv::imshow("My Image", image);
    QImage qOriginalImage((uchar*)image.data, image.cols,
      image.rows, image.step, QImage::Format_RGB888);
    ui->label_2->setPixmap(QPixmap::fromImage(qOriginalImage));
}

MainView::~MainView()
{
    delete ui;
}
