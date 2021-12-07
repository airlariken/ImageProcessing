#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    origin_path = ":/iconImg/Image/基准态曲面图.jpg";

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
