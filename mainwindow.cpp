#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "messagewindow.h"


#include "ImageConverting.h"

#include <QDebug>
#include <QPainter>
#include <QFileDialog>
#include <QRegularExpression>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Image to bitmap converter v1.0"));
    this->ui->horizontalSliderThreshold->setMinimum(0);
    this->ui->horizontalSliderThreshold->setMaximum(255);
    this->ui->horizontalSliderThreshold->setValue(127);
    imageToDisplay = QImage("SplashScreen.png");

    reverse_bytes = false;
    enable_threshold = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonOpenFile_clicked()
{
    char cwd[1024];

    getcwd(cwd, sizeof(cwd));
    QString absolutePath(cwd);

    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), absolutePath, tr("Image Files (*.png *.jpg *.bmp)"));

    if(filename.isEmpty())
        return;

    image = imread(filename.toUtf8().constData(), CV_LOAD_IMAGE_COLOR);
    imageToDisplay = QImage((const uint8_t*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888);
    this->repaint();
}

void MainWindow::on_pushButtonExport_clicked()
{
    if(image.empty())
    {
        MessageWindow message;
        message.setModal(true);
        message.setText(tr("Please open image file first"));
        message.exec();

        return;
    }
    // Read array name
    QString array_name = ui->plainTextEditArrayName->toPlainText();

    //check if array name contains only ASCII characters
    if(array_name.isEmpty())
    {
        MessageWindow message;
        message.setModal(true);
        message.setText(tr("Please specify array name in a textbox"));
        message.exec();

        return;
    }

    if(array_name.contains(QRegularExpression("^[a-zA-Z0-9_]{1,128}$")) == false)
    {
        MessageWindow message;
        message.setModal(true);
        message.setText(tr("Array name can contain only ASCII letters and can't contain whitespaces"));
        message.exec();

        return;
    }

    if(array_name.contains(QRegularExpression("^[^0-9]")) == false)
    {
        MessageWindow message;
        message.setModal(true);
        message.setText(tr("Array name must not start with a number"));
        message.exec();

        return;
    }

    // Select file from file explorer
    QString filename = QFileDialog::getSaveFileName(this, tr("Export as header file"), "", tr("C/C++ header file (*.h);;All Files (*)"));
    if(filename.isEmpty())
        return;

    // Open file and write data
    ofstream outputFile;
    outputFile.open(filename.toUtf8().constData());
    if(enable_threshold == false && reverse_bytes == true)
        write_header_file_reversed(outputFile, image, array_name.toUtf8().constData());
    else if(enable_threshold == false && reverse_bytes == false)
        write_header_file_normal(outputFile, image, array_name.toUtf8().constData());
    else if(enable_threshold == false && reverse_bytes == false)
        write_header_file_normal(outputFile, image, array_name.toUtf8().constData());
    else if(enable_threshold == true && reverse_bytes == true)
        write_header_file_normal(outputFile, bin_image, array_name.toUtf8().constData());
    else if(enable_threshold == true && reverse_bytes == false)
        write_header_file_normal(outputFile, bin_image, array_name.toUtf8().constData());


    outputFile.close();
}

void MainWindow::on_checkBoxReverseBytes_stateChanged()
{
    reverse_bytes = !reverse_bytes;
    //qDebug() << "reverse bytes = " << reverse_bytes << endl;
}

void MainWindow::on_checkBoxThreshold_stateChanged()
{
    enable_threshold = !enable_threshold;
    //qDebug() << "enable threshold = " << enable_threshold << endl;
    if(image.empty())
    {
        return;
    }
    if(enable_threshold)
    {
        cvtColor(image, bw_image, cv::COLOR_BGR2GRAY);
        cvtColor(bw_image, bw_image, cv::COLOR_GRAY2BGR);
        threshold(bw_image, bin_image, this->ui->horizontalSliderThreshold->value(), 255, THRESH_BINARY);
        bin_image.convertTo(dummy_bin_image, CV_8UC3);
        imageToDisplay = QImage((const uint8_t*)dummy_bin_image.data, dummy_bin_image.cols, dummy_bin_image.rows, dummy_bin_image.step, QImage::Format_BGR888);
        this->repaint();
    }
    else
    {
        imageToDisplay = QImage((const uint8_t*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888);
        this->repaint();
    }

}

void MainWindow::on_horizontalSliderThreshold_valueChanged()
{
    //qDebug() << this->ui->horizontalSliderThreshold->value() << endl;
    if(enable_threshold)
    {
        threshold(bw_image, bin_image, this->ui->horizontalSliderThreshold->value(), 255, THRESH_BINARY);
        bin_image.convertTo(dummy_bin_image, CV_8UC3);
        imageToDisplay = QImage((const uint8_t*)dummy_bin_image.data, dummy_bin_image.cols, dummy_bin_image.rows, dummy_bin_image.step, QImage::Format_BGR888);
        this->repaint();
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QImage imageToDisplayScaled = imageToDisplay.scaled(600, 400, Qt::KeepAspectRatio);

    QRectF target(0.0,0.0, imageToDisplayScaled.width(), imageToDisplayScaled.height());
    QRectF source(0.0,0.0, imageToDisplayScaled.width(), imageToDisplayScaled.height());
    QPainter painter(this);
    painter.drawImage(target, imageToDisplayScaled, source);
}
