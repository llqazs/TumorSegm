#ifndef IMAGEDATA2D_H_INCLUDED
#define IMAGEDATA2D_H_INCLUDED

#include "common.h"
#include "Data2D.h"
#include "Image2D.h"

class ImageData2D
{
public:
    //this constructor is only for experiment
    ImageData2D(const string& filename);
    //To do: constructor with cropped image, bounding box and seeds
    //ImageData2D( some opencv data here);
    virtual ~ImageData2D();

    void load(const string& filename);
    void destroy();
public:
    Image2D<uint16> img;
    //ground truth is not provided in practise
//    Data2D<uchar> gt;
    Range2D seg_region;
//    Range2D box_mid;
    vector<Index2D> seeds;

};


#endif // IMAGEDATA2D_H_INCLUDED
