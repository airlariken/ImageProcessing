#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    origin_path = "/Users/chenziwei/Downloads/第二问理想抛物面.jpg";

    ui->toolBar->setIconSize(QSize(20,20));//设置toolbar内icon大小，默认太大了太丑了
    //slider和spinbox联动，非常妙
    connect(ui->spinBox_exposure,SIGNAL(valueChanged(int)), ui->horizontalSlider_exposure, SLOT(setValue(int)));
    connect(ui->horizontalSlider_exposure,SIGNAL(valueChanged(int)), ui->spinBox_exposure,SLOT(setValue(int)));
    connect(ui->spinBox_contrast,SIGNAL(valueChanged(int)), ui->horizontalSlider_contrast, SLOT(setValue(int)));
    connect(ui->horizontalSlider_contrast,SIGNAL(valueChanged(int)), ui->spinBox_contrast,SLOT(setValue(int)));

    connect(ui->spinBox_saturation,SIGNAL(valueChanged(int)), ui->horizontalSlider_saturation, SLOT(setValue(int)));
    connect(ui->horizontalSlider_saturation,SIGNAL(valueChanged(int)), ui->spinBox_saturation,SLOT(setValue(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::on_actionopenFile_triggered()
//{
//    QStringList srcDirPathListS = QFileDialog::getOpenFileNames(this, tr("选择图片"), "E:/Qt/qtworks/MainWindow/images", tr("图像文件(*.jpg *.png *.bmp)"));

////    srcDirPathList =srcDirPathListS;
////    srcDirPathListS.clear();
////    index =0;
//    QString srcDirPath = srcDirPathListS.at(0);
//    QImage image(srcDirPath);

//    QImage Image = ImageCenter(image, ui->label_imgshow);
//    ui->label_imgshow->setPixmap(QPixmap::fromImage(Image));
//    ui->label_imgshow->setAlignment(Qt::AlignCenter);

////    //状态栏显示图片路径
////    QLabel *label=ui->statusBar->findChild<QLabel *>("status");
////    label->setText(srcDirPath);
//    //有图片触发事件
//    //isImage=true;
//    //qDebug("%d",srcDirPathList.size());
////    ui->pushButton_3->setDisabled(true);
////    ui->pushButton_4->setDisabled(true);
////    ui->label_other_3->setVisible(false);
////    ui->label_other_1->setVisible(false);

//}

//图片中心展示，适配窗口label大小
QImage MainWindow::ImageCenter(QImage  qimage,QLabel *qwidget)
{
    QImage image;
    QSize imageSize = qimage.size();
    QSize labelSize = qwidget->size();

    double dWidthRatio = 1.0*imageSize.width() / labelSize.width();
    double dHeightRatio = 1.0*imageSize.height() / labelSize.height();
    if (dWidthRatio>dHeightRatio) {
        image = qimage.scaledToWidth(labelSize.width(),Qt::SmoothTransformation);
    }
    else {
        image = qimage.scaledToHeight(labelSize.height(), Qt::SmoothTransformation);
    }
    return image;

}

void MainWindow::on_action_openFile_triggered()
{
    QStringList srcDirPathListS = QFileDialog::getOpenFileNames(this, tr("选择图片"), "E:/Qt/qtworks/MainWindow/images", tr("图像文件(*.jpg *.png *.bmp)"));

//    srcDirPathList =srcDirPathListS;
//    srcDirPathListS.clear();
//    index =0;
    if(srcDirPathListS.size() == 0)     //如果没有选择图片，则正常退出函数
        return;
    QString srcDirPath = srcDirPathListS.at(0);
    origin_path = srcDirPath;
    QImage image(srcDirPath);
    QImage Image = ImageCenter(image, ui->label_imgshow);
    ui->label_imgshow->setPixmap(QPixmap::fromImage(Image));
    ui->label_imgshow->setAlignment(Qt::AlignCenter);

//    IMREAD_REDUCED_GRAYSCALE_2;

//    Mat src = imread(origin_path.toStdString(), IMREAD_COLOR); //从路径中读取图片
//    show_histogram(src);
    getRGBHistogram();




//    //状态栏显示图片路径
//    QLabel *label=ui->statusBar->findChild<QLabel *>("status");
//    label->setText(srcDirPath);
    //有图片触发事件
    //isImage=true;
    //qDebug("%d",srcDirPathList.size());
//    ui->pushButton_3->setDisabled(true);
//    ui->pushButton_4->setDisabled(true);
//    ui->label_other_3->setVisible(false);
//    ui->label_other_1->setVisible(false);
}

void MainWindow::on_pushButton_toGray_clicked()
{
    if(origin_path!=nullptr) {
    QImage image(origin_path);

    QImage images=gray(image);
    QImage Image=ImageCenter(images,ui->label_imgshow);
    ui->label_imgshow->setPixmap(QPixmap::fromImage(Image));
    ui->label_imgshow->setAlignment(Qt::AlignCenter);
    }
    else {
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//灰度化
QImage MainWindow::gray(QImage image){
    QImage newImage =image.convertToFormat(QImage::Format_ARGB32);
    QColor oldColor;

    // 算法核心：三通道均值，三色相同即为灰度图
        for(int y = 0; y < newImage.height(); y++)
        {
            for(int x = 0; x < newImage.width(); x++)
            {
                oldColor = QColor(image.pixel(x,y));
                int average = (oldColor.red() + oldColor.green() + oldColor.blue()) / 3;
                newImage.setPixel(x, y, qRgb(average, average, average));
            }
        }
        return newImage;
}

//均值滤波
QImage MainWindow::meanFilterAlgo(QImage image){
    int kernel [3][3] = {
        {1,1,1},
        {1,1,1},
        {1,1,1}};
    int sizeKernel = 3;
    int sumKernel = 9;
    QColor color;
     for(int x = sizeKernel/2;x<image.width() - sizeKernel/2;x++)
    {
       for(int y= sizeKernel/2;y<image.height() - sizeKernel/2;y++)
        {
            int r = 0;
            int g = 0;
            int b = 0;
            for(int i = -sizeKernel/2;i<=sizeKernel/2;i++)
            {
               for(int j = -sizeKernel/2;j<=sizeKernel/2;j++)
                {
                 color = QColor(image.pixel(x+i,y+j));
                 r+=color.red()*kernel[sizeKernel/2+i][sizeKernel/2+j];
                 g+=color.green()*kernel[sizeKernel/2+i][sizeKernel/2+j];
                 b+=color.blue()*kernel[sizeKernel/2+i][sizeKernel/2+j];
                }
            }
            r = qBound(0,r/sumKernel,255);
            g = qBound(0,g/sumKernel,255);
            b = qBound(0,b/sumKernel,255);
            image.setPixel(x,y,qRgb( r,g,b));
        }
    }
    return image;
}


void MainWindow::on_pushButton_meanFilter_clicked()
{
    if(origin_path!=nullptr){
    QImage image(origin_path);
        image=meanFilterAlgo(image);
             QImage Image=ImageCenter(image,ui->label_imgshow);
             ui->label_imgshow->setPixmap(QPixmap::fromImage(Image));
             ui->label_imgshow->setAlignment(Qt::AlignCenter);
}
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

void MainWindow::on_pushButton_originalImg_clicked()
{
    if(origin_path!=nullptr) {
        QImage image(origin_path);
        image = ImageCenter(image, ui->label_imgshow);
        ui->label_imgshow->setPixmap(QPixmap::fromImage(image));
        ui->label_imgshow->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}


//边缘检测
QImage MainWindow::edgeDetection(QImage image){
    QImage newImage =image.convertToFormat(QImage::Format_ARGB32);
    QColor color0;
    QColor color1;
    QColor color2;
    QColor color3;
    int  r = 0;
    int g = 0;
    int b = 0;
    int rgb = 0;
    int r1 = 0;
    int g1 = 0;
    int b1 = 0;
    int rgb1 = 0;
    int a = 0;
    for( int y = 0; y < image.height() - 1; y++)
    {
        for(int x = 0; x < image.width() - 1; x++)
        {
            color0 =   QColor (image.pixel(x,y));
            color1 =   QColor (image.pixel(x + 1,y));
            color2 =   QColor (image.pixel(x,y + 1));
            color3 =   QColor (image.pixel(x + 1,y + 1));
            r = abs(color0.red() - color3.red());
            g = abs(color0.green() - color3.green());
            b = abs(color0.blue() - color3.blue());
            rgb = r + g + b;

            r1 = abs(color1.red() - color2.red());
            g1= abs(color1.green() - color2.green());
            b1 = abs(color1.blue() - color2.blue());
            rgb1 = r1 + g1 + b1;

            a = rgb + rgb1;
            a = a>255?255:a;

            newImage.setPixel(x,y,qRgb(a,a,a));
        }
    }
    return newImage;
}


void MainWindow::on_pushButton_edgeDetection_clicked()
{
    if(origin_path!=nullptr){
    QImage image(origin_path);
    QImage newImage =edgeDetection(image);
    QImage Image=ImageCenter(newImage,ui->label_imgshow);
    ui->label_imgshow->setPixmap(QPixmap::fromImage(Image));
    ui->label_imgshow->setAlignment(Qt::AlignCenter);
}
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}


QImage MainWindow::gammaTransferALgo(QImage image){
    double d=1.2;
    QColor color;
    int height = image.height();
    int width = image.width();
    for (int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            color = QColor(image.pixel(i,j));
            double r = color.red();
            double g = color.green();
            double b = color.blue();
            int R = qBound(0,(int)qPow(r,d),255);
            int G = qBound(0,(int)qPow(g,d),255);
            int B = qBound(0,(int)qPow(b,d),255);
            image.setPixel(i,j,qRgb(R,G,B));
        }
    }
    return image;
}

void MainWindow::on_pushButton_gammaTransfer_clicked()
{
    if(origin_path!=nullptr){
        QImage image(origin_path);
        image=gammaTransferALgo(image);
        QImage Image=ImageCenter(image,ui->label_imgshow);
        ui->label_imgshow->setPixmap(QPixmap::fromImage(Image));
        ui->label_imgshow->setAlignment(Qt::AlignCenter);
    }else{
        QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

void MainWindow::on_pushButton_imgSave_clicked()
{

}

    //亮度调节
void MainWindow::on_horizontalSlider_exposure_valueChanged(int value)
{

    if(origin_path!=nullptr) {
        QImage image(origin_path);
        int red, green, blue;
        int pixels = image.width() * image.height();
        unsigned int *data = (unsigned int *)image.bits();
        // qRed()等函数返回该像素的特定颜色通道值的引用
        for (int i = 0; i < pixels; ++i) {
            red= qRed(data[i])+ value;
            overflowCheck(red);
//            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;      //处理上下溢出情况
            green= qGreen(data[i]) + value;
//            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            overflowCheck(green);
            blue= qBlue(data[i]) + value;
//            blue =  (blue  < 0x00) ? 0x00 : (blue  > 0xff) ? 0xff : blue ;
            overflowCheck(blue);
            data[i] = qRgba(red, green, blue, qAlpha(data[i]));
        }
        QImage Image=ImageCenter(image,ui->label_imgshow);
        ui->label_imgshow->setPixmap(QPixmap::fromImage(Image));
        ui->label_imgshow->setAlignment(Qt::AlignCenter);
    }

}

void MainWindow::on_action_DockShow_triggered()
{

    ui->dockWidget->show();
}

void MainWindow::on_spinBox_exposure_valueChanged(int arg1)
{
//    static bool flag = 0;
//    explosure_value = arg1;
    cout<<arg1;
//    ui->horizontalSlider_exposure->valueChanged(arg1);
//    if(flag == 1) {
//        ui->horizontalSlider_exposure->valueChanged(arg1);
//        ui->horizontalSlider_exposure->

//        flag = 0;
//    }
//    else
//        flag = 0;
}

QImage MainWindow::adjustContrastAlgo(QImage Img, int iContrastValue)
{
    int pixels = Img.width() * Img.height();
    unsigned int *data = (unsigned int *)Img.bits();

    int red, green, blue, nRed, nGreen, nBlue;

    if (iContrastValue > 0 && iContrastValue < 100)
    {
        float param = 1 / (1 - iContrastValue / 100.0) - 1;

        for (int i = 0; i < pixels; ++i)
        {
            nRed = qRed(data[i]);
            nGreen = qGreen(data[i]);
            nBlue = qBlue(data[i]);

            red = nRed + (nRed - 127) * param;
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = nGreen + (nGreen - 127) * param;
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = nBlue + (nBlue - 127) * param;
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

            data[i] = qRgba(red, green, blue, qAlpha(data[i]));
        }
    }
    else
    {
        for (int i = 0; i < pixels; ++i)
        {
            nRed = qRed(data[i]);
            nGreen = qGreen(data[i]);
            nBlue = qBlue(data[i]);

            red = nRed + (nRed - 127) * iContrastValue / 100.0;
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = nGreen + (nGreen - 127) * iContrastValue / 100.0;
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = nBlue + (nBlue - 127) * iContrastValue / 100.0;
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

            data[i] = qRgba(red, green, blue, qAlpha(data[i]));
        }
    }
    return Img;
}

void MainWindow::on_horizontalSlider_contrast_valueChanged(int value)
{
    if(origin_path!=nullptr) {
        QImage image(origin_path);
        QImage adj_img = adjustContrastAlgo(image, value);
        QImage Image=ImageCenter(adj_img,ui->label_imgshow);
        ui->label_imgshow->setPixmap(QPixmap::fromImage(Image));
        ui->label_imgshow->setAlignment(Qt::AlignCenter);
    }

}
QImage MainWindow::adjustSaturationAlgo(QImage Img, int iSaturateValue)
{
    int red, green, blue, nRed, nGreen, nBlue;
    int pixels = Img.width() * Img.height();
    unsigned int *data = (unsigned int *)Img.bits();

    float Increment = iSaturateValue/100.0;

    float delta = 0;
    float minVal, maxVal;
    float L, S;
    float alpha;

    for (int i = 0; i < pixels; ++i)
    {
        nRed = qRed(data[i]);
        nGreen = qGreen(data[i]);
        nBlue = qBlue(data[i]);

        minVal = std::min(std::min(nRed, nGreen), nBlue);
        maxVal = std::max(std::max(nRed, nGreen), nBlue);
        delta = (maxVal - minVal) / 255.0;
        L = 0.5*(maxVal + minVal) / 255.0;
        S = std::max(0.5*delta / L, 0.5*delta / (1 - L));

        if (Increment > 0)
        {
            alpha = std::max(S, 1 - Increment);
            alpha = 1.0 / alpha - 1;
            red = nRed + (nRed - L*255.0)*alpha;
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = nGreen + (nGreen - L*255.0)*alpha;
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = nBlue + (nBlue - L*255.0)*alpha;
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
        }
        else
        {
            alpha = Increment;
            red = L*255.0 + (nRed - L * 255.0)*(1+alpha);
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = L*255.0 + (nGreen - L * 255.0)*(1+alpha);
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = L*255.0 + (nBlue - L * 255.0)*(1+alpha);
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
        }

        data[i] = qRgba(red, green, blue, qAlpha(data[i]));
    }

    return Img;
}

void MainWindow::on_horizontalSlider_saturation_valueChanged(int value)
{
    if(origin_path!=nullptr) {
        QImage image(origin_path);
        QImage adj_img = adjustSaturationAlgo(image, value);
        QImage Image=ImageCenter(adj_img,ui->label_imgshow);
        ui->label_imgshow->setPixmap(QPixmap::fromImage(Image));
        ui->label_imgshow->setAlignment(Qt::AlignCenter);
    }
}

void MainWindow::on_horizontalSlider_rotate_valueChanged(int value)
{


    //防止旋转后出现大小爆炸问题

    ui->label_imgshow->setMaximumHeight(ui->label_imgshow->height());
    ui->label_imgshow->setMinimumWidth(ui->label_imgshow->width());
//    ui->label_imgshow->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    if(ui->label_imgshow->pixmap()!=nullptr){
//        QImage images(ui->label_imgshow->pixmap()->toImage());
        QImage images(origin_path);
        images = ImageCenter(images, ui->label_imgshow);
        QMatrix matrix;
        matrix.rotate(-value);//逆时针旋转90度
        images = images.transformed(matrix,Qt::SmoothTransformation);
//        QImage Image=ImageCenter(images,ui->label_imgshow);
        ui->label_imgshow->setPixmap(QPixmap::fromImage(images));
//        ui->label_imgshow->setAlignment(Qt::AlignCenter);




    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
//    ui->horizontalLayout_4->setStretch(1,5);
//    ui->label_imgshow->setMaximumHeight(16777215);
//    ui->label_imgshow->setMinimumWidth(16777215);
}

void MainWindow::show_histogram(Mat& img)
{
    //为计算直方图配置变量
    //首先是需要计算的图像的通道，就是需要计算图像的哪个通道（bgr空间需要确定计算 b或g货r空间）
    int channels = 0;
    //然后是配置输出的结果存储的 空间 ，用MatND类型来存储结果
    MatND dstHist;
    //接下来是直方图的每一个维度的 柱条的数目（就是将数值分组，共有多少组）
    int histSize[] = { 256 };       //如果这里写成int histSize = 256;   那么下面调用计算直方图的函数的时候，该变量需要写 &histSize
    //最后是确定每个维度的取值范围，就是横坐标的总数
    //首先得定义一个变量用来存储 单个维度的 数值的取值范围
    float midRanges[] = { 0, 256 };
    const float *ranges[] = { midRanges };

    calcHist(&img, 1, &channels, Mat(), dstHist, 1, histSize, ranges, true, false);

    //calcHist  函数调用结束后，dstHist变量中将储存了 直方图的信息  用dstHist的模版函数 at<Type>(i)得到第i个柱条的值
    //at<Type>(i, j)得到第i个并且第j个柱条的值

    //开始直观的显示直方图——绘制直方图
    //首先先创建一个黑底的图像，为了可以显示彩色，所以该绘制图像是一个8位的3通道图像
    Mat drawImage = Mat::zeros(Size(256, 256), CV_8UC3);
    //因为任何一个图像的某个像素的总个数，都有可能会有很多，会超出所定义的图像的尺寸，针对这种情况，先对个数进行范围的限制
    //先用 minMaxLoc函数来得到计算直方图后的像素的最大个数
    double g_dHistMaxValue;
    minMaxLoc(dstHist, 0, &g_dHistMaxValue, 0, 0);
    //将像素的个数整合到 图像的最大范围内
    //遍历直方图得到的数据
    for (int i = 0; i < 256; i++)
    {
        int value = cvRound(dstHist.at<float>(i) * 256 * 0.9 / g_dHistMaxValue);

        line(drawImage, Point(i, drawImage.rows - 1), Point(i, drawImage.rows - 1 - value), Scalar(255, 255, 255));
    }

//    imshow("【原图直方图】", drawImage);
    imwrite("/Users/chenziwei/Downloads/nice.png", drawImage); //保存处理后的图片
}

int MainWindow::getRGBHistogram()
{
    //读取本地的一张图片
        Mat srcimage = imread(origin_path.toStdString());
//        imshow("原图", srcimage);
        int channels = 0;
        int histsize[] = { 256 };
        float midranges[] = { 0,255 };
        const float *ranges[] = { midranges };
        MatND  dsthist;    //要输出的直方图
        //重点关注calcHist函数，即为计算直方图的函数
        calcHist(&srcimage, 1, &channels, Mat(), dsthist, 1, histsize, ranges, true, false);

        Mat b_drawImage = Mat::zeros(Size(256, 256), CV_8UC3);
        double g_dhistmaxvalue;
        minMaxLoc(dsthist, 0, &g_dhistmaxvalue, 0, 0);
        for (int i = 0;i < 256;i++) {
            //这里的dsthist.at<float>(i)就是每个bins对应的纵轴的高度
            int value = cvRound(256 * 0.9 *(dsthist.at<float>(i) / g_dhistmaxvalue));
            line(b_drawImage, Point(i, b_drawImage.rows - 1), Point(i, b_drawImage.rows - 1 - value), Scalar(255, 0, 0));
        }
//        imshow("B通道直方图", b_drawImage);

        channels = 1;
        calcHist(&srcimage, 1, &channels, Mat(), dsthist, 1, histsize, ranges, true, false);
        Mat g_drawImage = Mat::zeros(Size(256, 256), CV_8UC3);
        for (int i = 0;i < 256;i++) {
            int value = cvRound(256 * 0.9 *(dsthist.at<float>(i) / g_dhistmaxvalue));
            line(g_drawImage, Point(i, g_drawImage.rows - 1), Point(i, g_drawImage.rows - 1 - value), Scalar(0, 255, 0));
        }
//        imshow("G通道直方图", g_drawImage);

        channels = 2;
        calcHist(&srcimage, 1, &channels, Mat(), dsthist, 1, histsize, ranges, true, false);
        Mat r_drawImage = Mat::zeros(Size(256, 256), CV_8UC3);
        for (int i = 0;i < 256;i++) {
            int value = cvRound(256 * 0.9 *(dsthist.at<float>(i) / g_dhistmaxvalue));
            line(r_drawImage, Point(i, r_drawImage.rows - 1), Point(i, r_drawImage.rows - 1 - value), Scalar(0, 0, 255));
        }
//        imshow("R通道直方图", r_drawImage);

        add(b_drawImage, g_drawImage, r_drawImage);   //将三个直方图叠在一块
//        imshow("RGB直方图", r_drawImage);
        imwrite("/Users/chenziwei/Downloads/RGB直方图.png", r_drawImage);
//        waitKey(0);
        return 0;
}

void MainWindow::on_pushButton_clicked()
{
    string edgeBasePath("");
    string command_esedbexport = "/Users/chenziwei/1/pythonSript.sh";
    command_esedbexport += edgeBasePath;//这里的edgeBasePath就是外部的可变参数
    system(command_esedbexport.c_str());

}

void MainWindow::on_pushButton_sharpen_clicked()
{
    //读取本地的一张图片
    Mat srcimage = imread(origin_path.toStdString());
    Mat res;
    res = sharpen(srcimage, res);


    QImage image(res.data, res.cols, res.rows, res.step, QImage::Format_RGB888);
    image = ImageCenter(image, ui->label_imgshow);

    ui->label_imgshow->setPixmap(QPixmap::fromImage(image.rgbSwapped()));

//    imshow("锐化", res);
//    cv::waitKey(1000);
//    ui->label_imgshow->setPixmap(QPixmap::fromImage(image));
}

Mat MainWindow::sharpen(const Mat &img, Mat &result)//锐化算子
{
//    //Method 1  直接操作像素点
//    result.create(img.rows, img.cols, img.type());
//    int nl = img.rows;
//    int nc = img.cols * img.channels();
//    for (int j = 1; j < nl - 1; j++)
//    {
//        const uchar* previous = img.ptr<const uchar>(j - 1);//上一行
//        const uchar* current = img.ptr<const uchar>(j);//当前行
//        const uchar* next = img.ptr<const uchar>(j + 1);//下一行
//        uchar* output = result.ptr<uchar>(j);//输出行
//        for (int i = 1; i < nc - 1; i++)
//        {
//             output[i]= saturate_cast<uchar>(5 * current[i] - current[i-1]
//                - current[i- 1] - previous[i] - next[i]);
//        }
//    }
//    //将未处理的像素设置为0
//    result.row(0).setTo(Scalar(0));
//    result.row(result.rows - 1).setTo(Scalar(0));
//    result.col(0).setTo(Scalar(0));
//    result.col(result.cols - 1).setTo(Scalar(0));
//    return result;

//    Method 2  cv::filter2D
//    构造滤波核

//        0  -1  0
//        -1  5  -1
//         0  -1  0
    Mat kernel(3, 3, CV_32F, Scalar(0));
    kernel.at<float>(1, 1) = 5.0;
    kernel.at<float>(0, 1) = -1.0;
    kernel.at<float>(1, 0) = -1.0;
    kernel.at<float>(1, 2) = -1.0;
    kernel.at<float>(2, 1) = -1.0;

//    cout << kernel << endl;
    filter2D(img, result, -1, kernel);

    return result;
}



// 创建高斯核
// kSize:卷积核的大小3、5、7等（3×3、5×5、7×7）
// sigma:方差

void MainWindow::CreatGaussKernel(float **pdKernel, int kSize, float sigma) {
    int sum = 0;
    float dCenter = (kSize - 1) / 2;
    //生成高斯数据
    for (int i = 0; i < kSize; i++) {
        for (int j = 0; j < kSize; j++) {
            //用和来近似平方和的开方
            float dis = fabsf(i - dCenter) + fabsf(j - dCenter);
            float val = exp(-dis * dis / (2 * sigma * sigma + EPS));
            pdKernel[i][j] = val;
            sum += val;
        }
    }
    //归一化
    for (int i = 0; i < kSize; i++) {
        for (int j = 0; j < kSize; j++) {
            pdKernel[i][j] /= (sum + EPS);
        }
    }
}

// KSize 3 5 7
Mat MainWindow::GaussBlur(Mat src, int kSize) {
    float sigma = 0.3*((kSize-1)*0.5-1)+0.8;
    sigma = 0.2;
    int row = src.rows;
    int col = src.cols;
    //分配高斯核空间
    float **pKernel = new float*[kSize];
    for (int i = 0; i < kSize; i++) {
        pKernel[i] = new float[kSize];
    }
    Mat dst(row, col, CV_8UC3);
    CreatGaussKernel(pKernel, kSize, sigma);
    int border = (kSize - 1) / 2;
    float sum = 0;
    for (int i = border; i < row - border; i++) {
        for (int j = border; j < col - border; j++) {
            for (int k = 0; k < 3; k++) {
                sum = 0;
                for (int x = -border; x <= border; x++) {
                    for (int y = -border; y <= border; y++) {
                        sum += src.at<Vec3b>(i + x, j + y)[k] * pKernel[border + x][border + y];
                    }
                }
                if (sum > 255) sum = 255;
                else if (sum < 0) sum = 0;
                dst.at<Vec3b>(i, j)[k] = sum;
            }
        }
    }
    return dst;
}

void MainWindow::on_pushButton_gaussianBlur_clicked()
{
    //读取本地的一张图片
    Mat srcimage = imread(origin_path.toStdString());
    Mat res;
    res = GaussBlur(srcimage, 3);
    GaussianBlur(srcimage, res, cv::Size(5,5), 1);
//    imshow("高斯",res);
//    waitKey(1000);


    QImage image(res.data, res.cols, res.rows, res.step, QImage::Format_RGB888);
    image = ImageCenter(image, ui->label_imgshow);

//    ui->label_imgshow->setPixmap(QPixmap::fromImage(image));
    ui->label_imgshow->setPixmap(QPixmap::fromImage(image.rgbSwapped()));
}
