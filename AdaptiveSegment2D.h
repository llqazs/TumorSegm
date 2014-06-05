#ifndef ADAPTIVESEGMENT2D_H_INCLUDED
#define ADAPTIVESEGMENT2D_H_INCLUDED

#include "Graph2D.h"
#include "Image2D.h"
#include "Data2D.h"
#include <unordered_map>
//#include "GraphLinkCost.h"

class AdaptiveSegment2D
{
    //Configuration settings
    //To do : compactness measurement, can be passed as a function

public:
    AdaptiveSegment2D(Image2D<short> &_image, Data2D<LABEL> &_mask, bool _showFlag = false );
    virtual ~AdaptiveSegment2D();

    //the interface for the algorithm
    void execute();
    void segment(double lambda);

    //settings setters
    void configure(const Config& _cfg);
    void setShowFlag(bool _showFlag);
    void setSegRegion(const MyBasic::Range2D& _roi);

    //adding seeds
    void addFgSeeds(const vector<MyBasic::Index2D>& _seeds);
    void addBkSeeds(const vector<MyBasic::Index2D>& _seeds);
    void addStarCenter(const vector<MyBasic::Index2D>& _starcenter);

    //getters
    inline Data2D<LABEL> getLabeling() const { return labeling; }
    Data2D<double> getSmoothterm() const; //average of smoothcost at all direction
    Data2D<double> getDataterm() const; //diff between fgcost and bkcost
    inline double getLambda() const {return bestLambda; }

private:
    //core functions for adaptive segmentation

    //search for legal interval of lambdas
    void binarySearchForRange();
    void searchForBestLambda(double lambda_left, double lambda_right, int smp_num);
    void prepareData();

private:
    void computeSmoothterm();
    void computeDataterm();
    void computeSparseDataCost();
    void computeStarshape();
    void computeHardConstraint();
    void hardConstraintStarSeeds();
    void hardConstraintBorder();

private:
    //settings for the algorithm
    Config cfg;
    bool showFlag;
private:

    //flag for data
    bool readyToSegment;

    //inputs for the algorithm
    Image2D<short>& image;
    Data2D<LABEL>& mask;

    MyBasic::Range2D seg_region;
    vector<MyBasic::Index2D> fgseeds;
    vector<MyBasic::Index2D> bkseeds;
    vector<MyBasic::Index2D> starcenter;

    /////////////////////////////////////////////////////////////////////////////
    // below are intermediate result for building the appearance model
    // size is the same as the original image
    ////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////
    // below are intermediate result for constructing the graph
    // size could be different with the original image and mask
    ////////////////////////////////////////////////////////////////////////////

    //image roi for building the graph, could be the same as the original image
    //used for dataterm, smoothterm and starshape
    Image2D<short> img_roi;
    //segmentation result
    //segmentation as a lableing problem
    Data2D<LABEL> labeling;

    //smoothterm for regular edges, i.e. edges on a grid, 4,6, or 8-neighborhood
    //i.e. edges defined by a neighborhood system
    struct LinkCap
    {
        Data2D<double> cap;
        Data2D<double> rev_cap;
    };

    struct SparseLinkCap
    {
        vector<MyBasic::Index2D> point1;
        vector<MyBasic::Index2D> point2;
        vector<double> cap;
        vector<double> rev_cap;

    };

    std::unordered_map<char,LinkCap> smoothterm;

    //smoothterm for irregular edges generated from star shape constraint
    SparseLinkCap smoothtermExtra;

    //store the smooth cost with lambda = 1
    std::unordered_map<char,LinkCap> tmpSmoothterm;

    // data cost on a grid
    Data2D<double> fgcost;
    Data2D<double> bkcost;

    //dataterm for irregular t-links generated by hard constraint
    struct SparseUnaryLink
    {
        vector<double> fgcost;
        vector<double> bkcost;
        vector<MyBasic::Index2D> point;
    };
    SparseUnaryLink datatermExtra;
    //result for parameter searching while segmentation

    //the result for parameter searching
    double bestLambda;

};


#endif // ADAPTIVESEGMENT2D_H_INCLUDED