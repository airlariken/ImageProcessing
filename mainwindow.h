#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QMainWindow>
//#include <QFileDialog>
//#include <QImage>
//#include <QLabel>
//#include <string>
//#include <vector>
//#include <QMessageBox>
//#include<QtMath>
//#include<iostream>

//#include <opencv2/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include "opencv2/imgcodecs.hpp"
#include<opencv2/imgproc/types_c.h>     //CV_BGR2RGB

#include "Header.h"
#include "yolo.h"
#include <stdio.h>
#include <stdlib.h>
#include<QDebug>
#define EPS 1e-7

using namespace std;
using namespace cv;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    QImage ImageCenter(QImage  qimage,QLabel *qLabel);//调整图片比例
private slots:
//    void on_actionopenFile_triggered();

    void on_action_openFile_triggered();

    void on_pushButton_toGray_clicked();


    QImage meanFilterAlgo(QImage image);
    QImage gray(QImage image);//灰度化
    QImage edgeDetection(QImage image);
    QImage gammaTransferALgo(QImage image);
    QImage adjustContrastAlgo(QImage Img, int iContrastValue);
    QImage adjustSaturationAlgo(QImage Img, int iSaturateValue);

    //处理上下溢出情况
    inline void overflowCheck(int &cor_val) {
        cor_val = (cor_val < 0x00) ? 0x00 : (cor_val > 0xff) ? 0xff : cor_val;
    }
//    QImage gray2(QImage image);//灰度化2

    //opencv函数
//    void show_histogram(Mat& img);
    int getRGBHistogram();
    Mat sharpen(const Mat &img, Mat &result);    //锐化算子
    void CreatGaussKernel(float **pdKernel, int kSize, float sigma);
    Mat GaussBlur(Mat src, int kSize);
    cv::Mat QImage2cvMat(QImage image);

    void on_pushButton_meanFilter_clicked();

    void on_pushButton_originalImg_clicked();

    void on_pushButton_edgeDetection_clicked();

    void on_pushButton_gammaTransfer_clicked();

    void on_pushButton_imgSave_clicked();

    void on_horizontalSlider_exposure_valueChanged(int value);

    void on_action_DockShow_triggered();

    void on_spinBox_exposure_valueChanged(int arg1);

    void on_horizontalSlider_contrast_valueChanged(int value);

    void on_horizontalSlider_saturation_valueChanged(int value);

    void on_horizontalSlider_rotate_valueChanged(int value);

    void on_pushButton_clicked();

    void on_pushButton_sharpen_clicked();

    void on_pushButton_gaussianBlur_clicked();

    void on_pushButton_objectDetection_clicked();

    void on_pushButton_dilate_clicked();

    void on_pushButton_erode_clicked();

    void on_horizontalSlider_exposure_sliderReleased();

private:
    YOLO *yolo_model;
    Ui::MainWindow *ui;
    QString origin_path;//目前处理的图片的原图
    int explosure_value;
    QImage cur_img;
};
#endif // MAINWINDOW_H
