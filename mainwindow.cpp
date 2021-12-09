#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    origin_path = "/Users/chenziwei/Downloads/第二问理想抛物面.jpg";
    origin_path ="/Users/chenziwei/Downloads/中国地质大学.png";

//    QImage rota(":/iconImg/Image/旋转.png");
//    ImageCenter(rota, ui->label_rotation);
//    ui->label_rotation->setPixmap(QPixmap::fromImage(rota));
    cur_img = QImage(origin_path);
    ui->toolBar->setIconSize(QSize(25,25));//设置toolbar内icon大小，默认太大了太丑了
    //slider和spinbox联动，非常妙
    connect(ui->spinBox_exposure,SIGNAL(valueChanged(int)), ui->horizontalSlider_exposure, SLOT(setValue(int)));
    connect(ui->horizontalSlider_exposure,SIGNAL(valueChanged(int)), ui->spinBox_exposure,SLOT(setValue(int)));
    connect(ui->spinBox_contrast,SIGNAL(valueChanged(int)), ui->horizontalSlider_contrast, SLOT(setValue(int)));
    connect(ui->horizontalSlider_contrast,SIGNAL(valueChanged(int)), ui->spinBox_contrast,SLOT(setValue(int)));

    connect(ui->spinBox_saturation,SIGNAL(valueChanged(int)), ui->horizontalSlider_saturation, SLOT(setValue(int)));
    connect(ui->horizontalSlider_saturation,SIGNAL(valueChanged(int)), ui->spinBox_saturation,SLOT(setValue(int)));

    QImage t1(":/iconImg/Image/亮度.png");
    t1 = ImageCenter(t1, ui->label_exposure);
    ui->label_exposure->setPixmap(QPixmap::fromImage(t1));
    t1.load(":/iconImg/Image/对比度.png");
    t1 = ImageCenter(t1, ui->label_exposure);
    ui->label_contrast->setPixmap(QPixmap::fromImage(t1));

    t1.load(":/iconImg/Image/饱和度.png");
    t1 = ImageCenter(t1, ui->label_exposure);
    ui->lable_saturation->setPixmap(QPixmap::fromImage(t1));

//    :/iconImg/Image/旋转.png
    //画RGB直方图
//    getRGBHistogram();

    //yolo ini
    Net_config yolo_nets[4] = {
        {0.5, 0.5, 0.5, "yolov5s"},
        {0.5, 0.5, 0.5,  "yolov5m"},
        {0.5, 0.5, 0.5, "yolov5l"},
        {0.5, 0.5, 0.5, "yolov5x"}
    };
    yolo_model = new YOLO(yolo_nets[0]);

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
    QStringList srcDirPathListS = QFileDialog::getOpenFileNames(this, tr("选择图片"), "/Users/chenziwei/Downloads", tr("图像文件(*.jpg *.png *.bmp *.jpeg)"));

//    srcDirPathList =srcDirPathListS;
//    srcDirPathListS.clear();
//    index =0;
    if(srcDirPathListS.size() == 0)     //如果没有选择图片，则正常退出函数
        return;
    QString srcDirPath = srcDirPathListS.at(0);
    origin_path = srcDirPath;
    QImage image(srcDirPath);
    cur_img = image;
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
//    QImage image(origin_path);
    QImage image(cur_img);
    QImage images=ImageProceeAlgo::gray(image);
    QImage Image=ImageCenter(images,ui->label_imgshow);
    ui->label_imgshow->setPixmap(QPixmap::fromImage(Image));
    ui->label_imgshow->setAlignment(Qt::AlignCenter);
    Mat m = QImage2cvMat(Image);
    getGrayHistogram(m);
    }
    else {
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
        cur_img = image;
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
    getRGBHistogram();
}


void MainWindow::on_pushButton_meanFilter_clicked()
{
    if(origin_path!=nullptr){
//    QImage image(origin_path);
        QImage image(cur_img);
        image=ImageProceeAlgo::meanFilterAlgo(image);
        QImage Image=ImageCenter(image,ui->label_imgshow);
        ui->label_imgshow->setPixmap(QPixmap::fromImage(Image));
        ui->label_imgshow->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
    getRGBHistogram();
}



void MainWindow::on_pushButton_edgeDetection_clicked()
{
    if(origin_path!=nullptr){
//    QImage image(origin_path);
    QImage image(cur_img);
    QImage newImage = ImageProceeAlgo::edgeDetection(image);
    QImage Image=ImageCenter(newImage,ui->label_imgshow);
    ui->label_imgshow->setPixmap(QPixmap::fromImage(Image));
    ui->label_imgshow->setAlignment(Qt::AlignCenter);
}
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}


void MainWindow::on_pushButton_gammaTransfer_clicked()
{
    if(origin_path!=nullptr){
        QImage image(cur_img);
        image=ImageProceeAlgo::gammaTransferALgo(image);
        QImage Image=ImageCenter(image,ui->label_imgshow);
        ui->label_imgshow->setPixmap(QPixmap::fromImage(Image));
        ui->label_imgshow->setAlignment(Qt::AlignCenter);
        getRGBHistogram();
    }else{
        QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }

}

void MainWindow::on_pushButton_imgSave_clicked()
{
    on_action_save_triggered();
}

    //亮度调节
void MainWindow::on_horizontalSlider_exposure_valueChanged(int value)
{

//        static int ori_val = 0;
    if(origin_path!=nullptr) {

//        QImage image(origin_path);
        QImage image(cur_img);
//        int off_set = (int)value - (int)ori_val;
//        ori_val = value;
//        cout<<(int)off_set<<endl;
//        qDebug()<<off_set;
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
//        cur_img = Image;
        ui->label_imgshow->setPixmap(QPixmap::fromImage(Image));
        ui->label_imgshow->setAlignment(Qt::AlignCenter);
        getRGBHistogram();
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



void MainWindow::on_horizontalSlider_contrast_valueChanged(int value)
{
    static int ori_val = 0;
    if(origin_path!=nullptr) {
//        QImage image(origin_path);
        QImage image(cur_img);
//        int off_set = value - ori_val;
//        ori_val = value;
        QImage adj_img = ImageProceeAlgo::adjustContrastAlgo(image, value);
        QImage Image=ImageCenter(adj_img,ui->label_imgshow);
        ui->label_imgshow->setPixmap(QPixmap::fromImage(Image));
        ui->label_imgshow->setAlignment(Qt::AlignCenter);
//        cur_img = Image;
        getRGBHistogram();
    }

}

void MainWindow::on_horizontalSlider_saturation_valueChanged(int value)
{
//    static int ori_val = 0;
    if(origin_path!=nullptr) {
//        QImage image(origin_path);
        QImage image(cur_img);
//        int off_set = value - ori_val;
//        ori_val = value;
        QImage adj_img = ImageProceeAlgo::adjustSaturationAlgo(image, value);
        QImage Image=ImageCenter(adj_img,ui->label_imgshow);
        ui->label_imgshow->setPixmap(QPixmap::fromImage(Image));
        ui->label_imgshow->setAlignment(Qt::AlignCenter);
//        cur_img = Image;
    }
    getRGBHistogram();
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
    getRGBHistogram();
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
//    ui->horizontalLayout_4->setStretch(1,5);
//    ui->label_imgshow->setMaximumHeight(16777215);
//    ui->label_imgshow->setMinimumWidth(16777215);
}

void MainWindow::getGrayHistogram(Mat& img)
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
//    imwrite("/Users/chenziwei/Downloads/nice.png", drawImage); //保存处理后的图片

    QImage i(cvMat2QImage(drawImage));
    i = ImageCenter(i, ui->label);

    ui->label->setPixmap(QPixmap::fromImage(i));
    return ;

}

int MainWindow::getRGBHistogram()
{
    //读取本地的一张图片
//    QPixmap t(*ui->label_imgshow->pixmap());
    QImage t = ui->label_imgshow->pixmap(Qt::ReturnByValue).toImage();
    if(t.isNull()){
        cerr<<"cant read pixmap!"<<endl;
        return -255;
    }

    Mat srcimage = QImage2cvMat(t);
    //        Mat srcimage = imread(origin_path.toStdString());
    int channels = 0;
    int histsize[] = { 256 };
    float midranges[] = { 0,255 };
    const float *ranges[] = { midranges };
    MatND  dsthist;    //要输出的直方图
    //重点calcHist函数，即为计算直方图的函数
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
    Mat res1, res2;
    //        imshow("R通道直方图", r_drawImage);
    add(b_drawImage, g_drawImage, res1);   //将三个直方图叠在一块
    add(res1, r_drawImage, res2);
    //        imshow("RGB直方图", b_drawImage);
    //        imshow("RGB直方图", g_drawImage);
    //        imshow("RGB直方图", r_drawImage);

    //        Mat m[3];
    //        m[0] = b_drawImage;
    //        m[1]=  g_drawImage;
    //        m[2]=  r_drawImage;

    //        cv::merge(m, 3, res);

    //        imshow("RGB直方图", res2);

    //        waitKey(100);
    //        imwrite("/Users/chenziwei/Downloads/RGB直方图.png", res);
    //读取本地的一张图片


    QImage i(res2.data, res2.cols, res2.rows, res2.step, QImage::Format_RGB888);
    i = ImageCenter(i, ui->label);

    ui->label->setPixmap(QPixmap::fromImage(i.rgbSwapped()));
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
//    Mat srcimage = imread(origin_path.toStdString());
    Mat srcimage = QImage2cvMat(cur_img);
    Mat res;
    res = sharpen(srcimage, res);




    QImage image(cvMat2QImage(res));
    image = ImageCenter(image, ui->label_imgshow);

    ui->label_imgshow->setPixmap(QPixmap::fromImage(image));
    getRGBHistogram();
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
    getRGBHistogram();
}

//目标识别 物体检测
void MainWindow::on_pushButton_objectDetection_clicked()
{

//    string imgpath = "/Users/chenziwei/摄影/2021.2.9微电影/IMG_6343.JPG";
//    Mat srcimg = imread(origin_path.toStdString());
//    Mat srcimg(QImage2cvMat(cur_img));
    cur_img.save("./tmp_pic.png");
    Mat srcimg = imread("./tmp_pic.png");

    Mat img = imread(origin_path.toStdString());
    if (srcimg.data == img.data) {
        qDebug()<<"same";
        return;
    }

    if(srcimg.cols > 1000 || srcimg.rows>1000)
        qDebug()<<"too big";


    yolo_model->detect(srcimg);
//    static const string kWinName = "Deep learning object detection in OpenCV";
//    imwrite("/Users/chenziwei/Downloads/srcimg.jpg", srcimg);
//    imshow("nice", srcimg);
//    waitKey(0);
//    destroyAllWindows();
    QImage t = cvMat2QImage(srcimg);
    t = ImageCenter(t, ui->label_imgshow);
    ui->label_imgshow->setPixmap(QPixmap::fromImage(t));

}

//膨胀算法
void MainWindow::on_pushButton_dilate_clicked()
{
    //从文件中读取成灰度图像
   Mat img = imread(origin_path.toStdString(), IMREAD_GRAYSCALE);
   if (img.empty()) {
       cerr<<"Can not load image %s\n"<<origin_path.toStdString();
       return;
   }
   //OpenCV方法
   Mat dilated_cv;
   dilate(img, dilated_cv, Mat());

   //自定义方法
   Mat dilated_my;
   dilated_my.create(img.rows, img.cols, CV_8UC1);
   for (int i = 0; i < img.rows; ++i)
   {
       for (int j = 0; j < img.cols; ++j)
       {
           //uchar minV = 255;
           uchar maxV = 0;

           //遍历周围最大像素值
           for (int yi = i-1; yi <= i+1; yi++)
           {
               for (int xi = j-1; xi <= j+1; xi++)
               {
                   if (xi<0||xi>= img.cols|| yi<0 || yi >= img.rows)
                   {
                       continue;
                   }
                   //minV = (std::min<uchar>)(minV, img.at<uchar>(yi, xi));
                   maxV = (std::max<uchar>)(maxV, img.at<uchar>(yi, xi));
               }
           }
           dilated_my.at<uchar>(i, j) = maxV;
       }
   }

   //比较两者的结果
   Mat c;
   compare(dilated_cv, dilated_my, c, CMP_EQ);

   //显示
   imshow("原始", img);
   waitKey(0);
   imshow("膨胀_cv", dilated_cv);
   waitKey(0);
   imshow("膨胀_my", dilated_my);
   waitKey(0);
   imshow("比较结果", c);
   waitKey(0);
   destroyAllWindows();
}

//腐蚀算法
void MainWindow::on_pushButton_erode_clicked()
{
    //从文件中读取成灰度图像

    Mat img = imread(origin_path.toStdString(), IMREAD_GRAYSCALE);

    if (img.empty())
    {
        cerr<<"Can not load image %s\n"<<origin_path.toStdString();
        return ;
    }

    //OpenCV方法
    Mat eroded_cv;
    erode(img, eroded_cv, Mat());

    //自定义方法
    Mat eroded_my;
    eroded_my.create(img.rows, img.cols, CV_8UC1);
    for (int i = 0; i < img.rows; ++i)
    {
        for (int j = 0; j < img.cols; ++j)
        {
            uchar minV = 255;
            //uchar maxV = 0;

            //遍历周围最大像素值
            for (int yi = i-1; yi <= i+1; yi++)
            {
                for (int xi = j-1; xi <= j+1; xi++)
                {
                    if (xi<0||xi>= img.cols|| yi<0 || yi >= img.rows)
                    {
                        continue;
                    }
                    minV = (std::min<uchar>)(minV, img.at<uchar>(yi, xi));
                    //maxV = (std::max<uchar>)(maxV, img.at<uchar>(yi, xi));
                }
            }
            eroded_my.at<uchar>(i, j) = minV;
        }
    }

    //比较两者的结果
    Mat c;
    compare(eroded_cv, eroded_my, c, CMP_EQ);


    //显示
    imshow("原始", img);
    waitKey(0);
    imshow("膨胀_cv", eroded_cv);
    waitKey(0);
    imshow("膨胀_my", eroded_my);
    waitKey(0);
    imshow("比较结果", c);
    waitKey(0);
    destroyAllWindows();
    return;
}


cv::Mat MainWindow::QImage2cvMat(QImage image)
{
    cv::Mat mat;
//    qDebug() << image.format();
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}
QImage MainWindow::cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}


void MainWindow::on_pushButton_renew_clicked()
{
    const QPixmap *current_img = ui->label_imgshow->pixmap();
    current_img->save("/tmp/a.png");
    cur_img = QImage("/tmp/a.png");
}

void MainWindow::on_action_save_triggered()
{
    if(ui->label_imgshow->pixmap()!=nullptr){
        QString filename = QFileDialog::getSaveFileName(this,
        tr("保存图片"),
        "/Users/chenziwei/Downloads/ImgPro_images.png",
        tr("*.png;; *.jpg;; *.bmp;; *.tif;; *.GIF")); //选择路径
        if (filename.isEmpty()) {
            return;
        }
        else {
            if (!(ui->label_imgshow->pixmap()->toImage().save(filename))) //保存图像
            {
                QMessageBox::information(this,
                    tr("图片保存成功！"),
                    tr("图片保存失败！"));
                return;
            }
//        ui->statusBar->showMessage("图片保存成功！");
        }

    }
    else {
    QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}



void MainWindow::on_pushButton_histEqualization_clicked()
{
    Mat gray_img, res;
    cvtColor(QImage2cvMat(cur_img), gray_img, CV_RGB2GRAY);
    equalizeHist(gray_img, res);
    QImage q(cvMat2QImage(res));
    q = ImageCenter(q, ui->label_imgshow);
    ui->label_imgshow->setPixmap(QPixmap::fromImage(q));
    getGrayHistogram(res);
}





void MainWindow::on_pushButton_downSampling_clicked()
{
    Mat src,res;
//    src = imread(origin_path.toStdString());
    src = QImage2cvMat(cur_img);
    if(!src.data){
        cout << "图像打开失败！" << endl;
        return ;
    }
    pyrDown(src,res,Size(src.cols/2,src.rows/2));
    QImage q(cvMat2QImage(res));
    q = ImageCenter(q, ui->label_imgshow);
    ui->label_imgshow->setPixmap(QPixmap::fromImage(q));
}

void MainWindow::on_action_renew_triggered()
{
    on_pushButton_renew_clicked();
}

void MainWindow::on_pushButton_blue_clicked()
{
    unsigned char *blueData;                                // 定义字符型指针数组blueData用于存储蓝色分量数据
    QImage img(cur_img);                                      // 加载当前图像
    img = img.convertToFormat(QImage::Format_RGB888);
    unsigned char *data = img.bits ();                      // 指向当前图像第一个像素
    int width = img.width();                               // 图像宽度
    int height = img.height();                             // 图像高度
    int bytePerLine = img.bytesPerLine();                   // 图像每行字节数
    blueData = new unsigned char [bytePerLine * height];    // 设置数组大小
    unsigned char blue = 0;                                 // 蓝色分量
    for (int i = 0; i < height; i++)                        // 遍历每一行
    {
        for ( int j = 0; j < width; j++ )                   // 遍历每一列
        {
            blue = *(data);                                 // 获取当前像素点蓝色分量
            blueData[i * bytePerLine + j * 3] = 0;
            blueData[i * bytePerLine + j * 3+1]=0;
            blueData[i * bytePerLine + j * 3+2]=blue;       // 蓝色通道赋原值
            data += 3;                                      // 更新至下一像素点
        }
    }

    QImage blueImage(blueData,width,height,bytePerLine,QImage::Format_RGB888);
    blueImage = ImageCenter(blueImage, ui->label_imgshow);
    ui->label_imgshow->setPixmap(QPixmap::fromImage(blueImage));
//    imgLabel->setPixmap (pixmap2);                          // 显示灰度图像
//    qDebug() << "提取蓝色分量";
    blueImage = ImageProceeAlgo::gray(blueImage);
    Mat m = QImage2cvMat(blueImage);
    getGrayHistogram(m);
}

void MainWindow::on_pushButton_red_clicked()
{
    unsigned char *redData;                                // 定义字符型指针数组blueData用于存储蓝色分量数据
    QImage img(cur_img);                                      // 加载当前图像
    img = img.convertToFormat(QImage::Format_RGB888);
    unsigned char *data = img.bits();                      // 指向当前图像第一个像素
    int width = img.width();                               // 图像宽度
    int height = img.height();                             // 图像高度
    int bytePerLine = img.bytesPerLine();                   // 图像每行字节数
    redData = new unsigned char [bytePerLine * height];    // 设置数组大小
    unsigned char red = 0;                                 // 蓝色分量
    for (int i = 0; i < height; i++)                        // 遍历每一行
    {
        for ( int j = 0; j < width; j++ )                   // 遍历每一列
        {
            red = *(data+2);                                 // 获取当前像素点蓝色分量
            redData[i * bytePerLine + j * 3] = red;
            redData[i * bytePerLine + j * 3+1]= 0;
            redData[i * bytePerLine + j * 3+2]= 0;
            data += 3;                                      // 更新至下一像素点，分别是RBGA
        }
    }
    QImage redImage(redData,width,height,bytePerLine,QImage::Format_RGB888);
    redImage = ImageCenter(redImage, ui->label_imgshow);
    ui->label_imgshow->setPixmap(QPixmap::fromImage(redImage));
//    imgLabel->setPixmap (pixmap2);                          // 显示灰度图像
//    qDebug() << "提取蓝色分量";
    redImage = ImageProceeAlgo::gray(redImage);
    Mat m = QImage2cvMat(redImage);
    getGrayHistogram(m);
}

void MainWindow::on_pushButton_4_clicked()
{
    unsigned char *greData;                                // 定义字符型指针数组blueData用于存储蓝色分量数据
    QImage img(cur_img);                                      // 加载当前图像
    img = img.convertToFormat(QImage::Format_RGB888);
    unsigned char *data = img.bits ();                      // 指向当前图像第一个像素
    int width = img.width();                               // 图像宽度
    int height = img.height();                             // 图像高度
    int bytePerLine = img.bytesPerLine();                   // 图像每行字节数
    greData = new unsigned char [bytePerLine * height];    // 设置数组大小
    unsigned char gre = 0;                                 // 蓝色分量
    for (int i = 0; i < height; i++)                        // 遍历每一行
    {
        for ( int j = 0; j < width; j++ )                   // 遍历每一列
        {
            gre = *(data+1);                                 // 获取当前像素点蓝色分量
            greData[i * bytePerLine + j * 3] = 0;
            greData[i * bytePerLine + j * 3+1]= gre;
            greData[i * bytePerLine + j * 3+2]= 0;
            data += 3;                                      // 更新至下一像素点，分别是RBGA
        }
    }
    QImage greImage(greData,width,height,bytePerLine,QImage::Format_RGB888);
    greImage = ImageCenter(greImage, ui->label_imgshow);
    ui->label_imgshow->setPixmap(QPixmap::fromImage(greImage));
//    imgLabel->setPixmap (pixmap2);                          // 显示灰度图像
    greImage = ImageProceeAlgo::gray(greImage);
    Mat m = QImage2cvMat(greImage);
    getGrayHistogram(m);
}
