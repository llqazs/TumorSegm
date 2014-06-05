#ifndef ADAPTIVESEGMENTATION_H_INCLUDED
#define ADAPTIVESEGMENTATION_H_INCLUDED

//To do
//decide which information to be stored
//dataterm, smoothterm, 1st segmentation, 2nd segmentation

#include "Smoothterm.h"

class AdaptiveSegmentation
{
public:
    struct Config
    {
        Config& operator =(const Config& cfg) { memcpy(this,&cfg,sizeof(Config)); return *this; }
        bool starshape;
        bool dataterm;
        bool hardConstraint;
        bool smootherm;
        bool white_to_dark;
        int bin_num;
        int min_area;
        int max_area;
        int seed_radius;
    };

public:
    AdaptiveSegmentation(ImageData& _imgData);

    void segment();

private:
    //before searching, datacost (including hardconstraint) and smoothcost (including star shape constraint)
    // for the to be segmented slice should be prepared

    //input: image, mask, bin_num, roi; output: datacost for foreground and background
    //input: image, roi; output: smoothcost for 4 edges
    //input: star shape center, roi; output: sparse smoothcost
    //input: fgseeds, bkseeds; output: sparse datacost
    void prepareData(int slice);

    //input: prepared data (fgcost, bkcost, smoothcost, sparse data cost, sparse smoothcost
    //output: best lambdas
    //segmentation for different lambdas on a slice can be shown during segmentation
    //inside the function, binary search, compactness measure, summation of a matrix would be performed
    void ParameterSearch();

    void ParameterSearchOnSlice();

    void SegmentMiddle();
    void SegmentLeft();
    void SegmentRight();


private:
    Data3D<double> fgcost;
    Data3D<double> bkcost;

    Data3D<BidirectEdge<double> > smoothcostRow;
    Data3D<BidirectEdge<double> > smoothcostCol;

    Data3D<uint8> mask;
private:
    Data3D<uint8> label;
    vector<double> bestLambda;

private:
    ImageData& imgData;
    Image3D<short>& imageROI;
    Config cfg;


};

#endif // ADAPTIVESEGMENTATION_H_INCLUDED
