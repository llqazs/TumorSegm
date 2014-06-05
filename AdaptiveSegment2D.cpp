#include "AdaptiveSegment2D.h"
#include "Smoothterm.h"
#include "MyMath.h"
#include "Histogram.h"
#include "AppearanceModel.h"
#include "HardConstraint.h"
#include "RegionProps.h"
#include <iostream>
using namespace std;

#define HDHUGE 10000

AdaptiveSegment2D::AdaptiveSegment2D(Image2D<short>& _image, Data2D<LABEL>& _mask, bool _showFlag):image(_image),mask(_mask),showFlag(_showFlag)
{

    seg_region = MyBasic::Range2D(0,_image.getNumRow()-1,0,_image.getNumCol()-1);

    //set cfg to default values
    cfg.starshape = cfg.smoothterm = cfg.dataterm = cfg.hardConstraint = cfg.white_to_dark = true;
    cfg.bin_num = 2;
    cfg.max_area = lliu::area<LABEL>(mask);
    cfg.min_area = cfg.max_area/3;
    cfg.seed_radius = 2;

    readyToSegment = false;
}

AdaptiveSegment2D::~AdaptiveSegment2D()
{

}


void AdaptiveSegment2D::execute()
{
    if(!readyToSegment)
    {
        prepareData();
        readyToSegment = true;
    }
    binarySearchForRange();
    searchForBestLambda(0.1,20,4);
}

void AdaptiveSegment2D::configure(const Config& _cfg)
{
    memcpy(&cfg,&_cfg,sizeof(Config));
}

void AdaptiveSegment2D::setShowFlag(bool _showFlag)
{
    showFlag = _showFlag;
}

void AdaptiveSegment2D::setSegRegion(const MyBasic::Range2D& _roi)
{
    if(_roi==MyBasic::Range2D::all())
    {
        seg_region = MyBasic::Range2D(0,image.getNumRow()-1,0,image.getNumCol()-1);
    }
    else seg_region = _roi;
}

void AdaptiveSegment2D::addFgSeeds(const vector<MyBasic::Index2D>& _seeds)
{
    vector<MyBasic::Index2D> tmp = _seeds;
    MyBasic::Index2D shift(seg_region.row.start,seg_region.col.start);
    for(int i=0;i<tmp.size();i++)
    {
        tmp[i]-=shift;
    }
 //   vector<MyBasic::Index2D> tmp -= MyBasic::Index2D(seg_region.row.start,seg_region.col.start);

    fgseeds.insert(fgseeds.end(),tmp.begin(),tmp.end());
}

void AdaptiveSegment2D::addBkSeeds(const vector<MyBasic::Index2D>& _seeds)
{
    vector<MyBasic::Index2D> tmp = _seeds;
    MyBasic::Index2D shift(seg_region.row.start,seg_region.col.start);
    for(int i=0;i<tmp.size();i++)
    {
        tmp[i]-=shift;
    }

    bkseeds.insert(bkseeds.end(),tmp.begin(),tmp.end());
}

void AdaptiveSegment2D::addStarCenter(const vector<MyBasic::Index2D>& _starcenter)
{
    vector<MyBasic::Index2D> tmp = _starcenter;
    MyBasic::Index2D shift(seg_region.row.start,seg_region.col.start);
    for(int i=0;i<tmp.size();i++)
    {
        tmp[i]-=shift;
    }
    starcenter.insert(starcenter.end(),tmp.begin(),tmp.end());
}


Data2D<double> AdaptiveSegment2D::getSmoothterm() const
{
    //To do
    Data2D<double> sm(seg_region.getSize());
    sm = smoothterm.at(ROW).cap + smoothterm.at(ROW).rev_cap + smoothterm.at(COLUMN).cap + smoothterm.at(COLUMN).rev_cap;
    sm/=4;
    return sm;
}

Data2D<double> AdaptiveSegment2D::getDataterm() const
{
    return bkcost-fgcost;
}

void AdaptiveSegment2D::binarySearchForRange()
{
    double lambda_left=0.1, lambda_right=20;
    double lambda_middle;
    do
    {
        lambda_middle = (lambda_left+lambda_right)/2;
        segment(lambda_middle);
        int area = lliu::sum(labeling);
        if( area>=cfg.min_area && area<=cfg.max_area) break;
        if(area<cfg.min_area)
        {
            lambda_right = lambda_middle;
        }
        else
        {
            lambda_left = lambda_middle;
        }
    }while(lambda_left!=lambda_right);

    //lambda_middle is only a lambda gives the legal area
    //lambda_middle is not the best lambda

    //second step: get the legal interval
    //shrink lambda_left and lambda_right towards lambda_middle
    //until the results of lambda_left and lambda_right are within legal range
    do
    {
        segment(lambda_left);
        int area = lliu::sum(labeling);
        if(area>=cfg.min_area && area<=cfg.max_area) break;
        lambda_left = (lambda_left+lambda_middle)/2;
    }while(lambda_left!=lambda_middle);

    do
    {
        segment(lambda_right);
        int area = lliu::sum(labeling);
        if(area>=cfg.min_area && area<=cfg.max_area) break;
        lambda_right = (lambda_right+lambda_middle)/2;
    }while(lambda_right!=lambda_middle);
}

//To do: compactness
void AdaptiveSegment2D::searchForBestLambda(double lambda_left, double lambda_right, int smp_num)
{
     //third step: sample smp_num values from [lambda_left, lambda_right]
     //find the one with the best compactness
     using namespace std;
     double dl = (lambda_right-lambda_left)/smp_num;
     vector<pair<double,double> > cmpt_lambda;
     for(int i=0;i<smp_num;i++)
     {
         double lambda = lambda_left + i*dl;
         segment(lambda);
         double compactness =1; //= compactnessMeasure(labeling);
     //    cmpt_lambda.push_back(pair(compactness, lambda);
     }

}

void AdaptiveSegment2D::prepareData()
{
    MyBasic::Size2D s = seg_region.getSize();
    img_roi.resize(s.nRow,s.nCol);
    img_roi.copyFrom(image,seg_region,MyBasic::Range2D(0,seg_region.row.getSize()-1,0,seg_region.col.getSize()-1));

    this->computeSmoothterm();
    this->computeDataterm();
    this->computeStarshape();
    this->hardConstraintStarSeeds();
    this->hardConstraintBorder();

    this->computeSparseDataCost();
}

void AdaptiveSegment2D::segment(double lambda)
{
    if(!readyToSegment)
    {
        prepareData();
        readyToSegment = true;
    }

    //scale smooth term
    smoothterm.at(ROW).cap *= lambda;
    smoothterm.at(ROW).rev_cap *= lambda;
    smoothterm.at(COLUMN).cap *= lambda;
    smoothterm.at(COLUMN).rev_cap *= lambda;

    //build graph and cut
    Graph2D g(img_roi.getNumRow(),img_roi.getNumCol());
    g.addDataCost(fgcost,FOREGROUND);
    g.addDataCost(bkcost,BACKGROUND);
    g.addDataCost(datatermExtra.point,datatermExtra.fgcost,datatermExtra.bkcost);
    g.addSmoothCost(smoothterm[ROW].cap,smoothterm[ROW].rev_cap,ROW);
    g.addSmoothCost(smoothterm[COLUMN].cap,smoothterm[COLUMN].rev_cap,COLUMN);
    g.addSmoothCost(smoothtermExtra.point1,smoothtermExtra.point2,smoothtermExtra.cap,smoothtermExtra.rev_cap);

    g.maxflow();

    labeling = g.get_labeling();
}

void AdaptiveSegment2D::computeSmoothterm()
{
    //
    vector<Data2D<double> > sm= lliu::ComputeSmoothCostWhiteToDark(img_roi,image.getDistR(),image.getVariance(),ROW);
    LinkCap smr;
    smr.cap = sm[0]; smr.rev_cap = sm[1];
    smoothterm.insert({ROW,smr});

    sm = lliu::ComputeSmoothCostWhiteToDark(img_roi,image.getDistC(),image.getVariance(),COLUMN);
    LinkCap smc;
    smc.cap = sm[0]; smc.rev_cap = sm[1];
    smoothterm.insert({COLUMN,smc});

}

void AdaptiveSegment2D::computeDataterm()
{

    Data2D<int> bin_id = lliu::binning<short>(image,cfg.bin_num);
    vector<double> fghist = lliu::hist(bin_id, mask, cfg.bin_num, 1);
    vector<double> fgprob = fghist/sum(fghist);
    vector<double> bkhist = lliu::hist(bin_id, mask, cfg.bin_num, 0);
    vector<double> bkprob = bkhist/sum(bkhist);

    Data2D<int> bin_id_roi = bin_id(seg_region);
    fgcost = lliu::LogLikelihood(bin_id_roi,fgprob);
    bkcost = lliu::LogLikelihood(bin_id_roi,bkprob);
}

void AdaptiveSegment2D::computeStarshape()
{

}

void AdaptiveSegment2D::computeSparseDataCost()
{
    //hard constraint
    //hard constraint for foreground
    for(int i=0;i<fgseeds.size();i++)
    {
        datatermExtra.fgcost.push_back(0);
        datatermExtra.bkcost.push_back(HDHUGE);
        datatermExtra.point.push_back(fgseeds[i]);
    }

    for(int i=0;i<bkseeds.size();i++)
    {
        datatermExtra.fgcost.push_back(HDHUGE);
        datatermExtra.bkcost.push_back(0);
        datatermExtra.point.push_back(bkseeds[i]);
    }

}


void AdaptiveSegment2D::hardConstraintStarSeeds()
{
    //the index of star centers is the index relative to the origin of the img_roi
    vector<MyBasic::Index2D> points = lliu::getPointsWithinCircle(starcenter,cfg.seed_radius);
    fgseeds.insert(fgseeds.end(),points.begin(),points.end());
}

void AdaptiveSegment2D::hardConstraintBorder()
{
    vector<MyBasic::Index2D> points = lliu::getBorderPoints(img_roi.getSize(),1);
    bkseeds.insert(bkseeds.end(),points.begin(),points.end());
}

