#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent *event);

private slots:
    void on_pushButtonExport_clicked();
    void on_pushButtonOpenFile_clicked();
    void on_checkBoxReverseBytes_stateChanged();
    void on_checkBoxThreshold_stateChanged();
    void on_horizontalSliderThreshold_valueChanged();

private:
    Ui::MainWindow *ui;

    Mat bw_image;
    Mat bin_image;
    Mat image;
    Mat dummy_bin_image;

    QImage imageToDisplay;

    bool reverse_bytes;
    bool enable_threshold;
};
#endif // MAINWINDOW_H
