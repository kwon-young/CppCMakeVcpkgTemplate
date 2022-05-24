#include "mainview.h"
#include "ui_mainview.h"

#include <iostream>
#include <vector>

#include <QAction>
#include <QColorSpace>
#include <QFileDialog>
#include <QImageReader>
#include <QImageWriter>
#include <QMessageBox>
#include <QStandardPaths>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/stitching.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>

MainView::MainView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainView)
{
    ui->setupUi(this);

    QObject::connect(ui->actionOpen, &QAction::triggered, this, &MainView::open);
    QObject::connect(ui->actionSave, &QAction::triggered, this, &MainView::saveAs);
}

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    for (const QByteArray &mimeTypeName : supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    dialog.setAcceptMode(acceptMode);
    if (acceptMode == QFileDialog::AcceptSave)
    {
        dialog.setDefaultSuffix("jpg");
    } else {
        dialog.setFileMode(QFileDialog::FileMode::ExistingFiles);
    }
}

bool MainView::loadFile(const QStringList &fileNames)
{
    /* QImageReader reader(fileName); */
    /* reader.setAutoTransform(true); */
    /* const QImage newImage = reader.read(); */
    /* if (newImage.isNull()) { */
    /*     QMessageBox::information(this, QGuiApplication::applicationDisplayName(), */
    /*                              tr("Cannot load %1: %2") */
    /*                              .arg(QDir::toNativeSeparators(fileName), reader.errorString())); */
    /*     return false; */
    /* } */
    std::vector<cv::Mat> cvimages;
    cv::Mat stitched;
    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(cv::Stitcher::SCANS);
    for (const auto & fileName : fileNames)
    {
        // read an image
        cvimages.push_back(cv::imread(fileName.toStdString()));
    }
    cv::Stitcher::Status status = stitcher->stitch(cvimages, stitched);
    if (status != cv::Stitcher::OK)
    {
        std::cout << "Can't stitch images, error code = " << int(status) << std::endl;
        /* return EXIT_FAILURE; */
    }
    // create an image window named "My Image" and show it
    //cv::namedWindow("My Image");
    //cv::imshow("My Image", image);
    cv::cvtColor(stitched, stitched, CV_BGR2RGB);
    QImage qOriginalImage((uchar*)stitched.data, stitched.cols,
      stitched.rows, stitched.step, QImage::Format_RGB888);

    setImage(qOriginalImage);

    /* setWindowFilePath(fileName); */

    /* const QString message = tr("Opened \"%1\", %2x%3, Depth: %4") */
    /*     .arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height()).arg(image.depth()); */
    /* statusBar()->showMessage(message); */
    return true;
}

void MainView::setImage(const QImage &newImage)
{
    image = newImage;
    if (image.colorSpace().isValid())
        image.convertToColorSpace(QColorSpace::SRgb);
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
//! [4]
    scaleFactor = 1.0;

    ui->scrollArea->setVisible(true);
    /* fitToWindowAct->setEnabled(true); */
    /* updateActions(); */

    /* if (!fitToWindowAct->isChecked()) */
    /*     imageLabel->adjustSize(); */
}

void MainView::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles())) {}
}

bool MainView::saveFile(const QString &fileName)
{
    QImageWriter writer(fileName);

    if (!writer.write(image)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot write %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName)), writer.errorString());
        return false;
    }
    /* const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName)); */
    /* statusBar()->showMessage(message); */
    return true;
}

void MainView::saveAs()
{
    QFileDialog dialog(this, tr("Save File As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().constFirst())) {}
}

MainView::~MainView()
{
    delete ui;
}
