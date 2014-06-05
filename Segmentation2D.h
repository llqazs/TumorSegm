#ifndef SEGMENTATION2D_H_INCLUDED
#define SEGMENTATION2D_H_INCLUDED

//input-----------------------
//2D image: image to be segmented
//mask:     indicator for background and foreground
//fgseeds:  seeds for foreground, hard constraint
//bkseeds:  seeds for background, hard constraint
//roi:      region to perform segmentation
//starshape center

class Segmentation2D
{
public:
    template<typename T>
    Segmentation2D(Image2D<T>& _img, Data2D<T>& _mask,vector<Index2D>& _fgseeds, vector<Index2D>& _bkseeds, vector<Index2D>& _istar, Range2D& _roi);
    void setImage(Image2D<T>& _img);
    void setMask(Data2D<T>& _mask);
    void setFgseeds(vector<Index2D>& _fgseeds);
    void setBkseeds(vector<Index2D>& _bkseeds);
    void setStarShapeCenter(vector<Index2D>& _istar);
    void setSegmentRegion(Range2D& _roi);

private:

    computeAppearanceModel();
    computeDataCost();
    computeSmoothCost();

private:
    Data2D<EdgeCost<double> > smoothtermR;
    Data2D<EdgeCost<double> > smoothtermC;
    Data2D<DataCost<double> > dataterm;

private:
    Image2D<T>& image;
    Data2D<T>& mask;
    vector<Index2D>& fgseeds;
    vector<Index2D>& bkseeds;
    vector<Index2D>& istar;
    Range2D& roi;

};
//intermediate result-----------
//smoothterm
//dataterm

//output-----------------------
//



//2D segmentation can have different methods, common things of these methoods
//smoothterm, dataterm

//adaptive 2D segmentation
//2D image, mask, hard constraint seeds for foreground and background
//appearance model, datacost, smoothcost, try different lambdas, get result

//3D segmentation with different methods
//adaptive 3D use adaptive 2D first
//pass a slice of volume to 2D segmentation, pass mask, fgseeds, bkseeds, roi, starshape center to 2D segmetation process
//adaptive segment, get the result
//get the intermediate result, smoothterm, dataterm

//get lambdas, build a 3D graph, put 3D star shape constraint, and segment
//save the result

#endif // SEGMENTATION2D_H_INCLUDED
