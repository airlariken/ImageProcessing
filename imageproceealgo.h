#ifndef IMAGEPROCEEALGO_H
#define IMAGEPROCEEALGO_H

#include "Header.h"

class ImageProceeAlgo
{
public:
    ImageProceeAlgo();
    static QImage meanFilterAlgo(QImage image);
    static QImage gray(QImage image);//灰度化
    static QImage edgeDetection(QImage image);
    static QImage gammaTransferALgo(QImage image);
    static QImage adjustContrastAlgo(QImage Img, int iContrastValue);
    static QImage adjustSaturationAlgo(QImage Img, int iSaturateValue);
};

#endif // IMAGEPROCEEALGO_H
