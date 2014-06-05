#ifndef IMAGEDATA_H_INCLUDED
#define IMAGEDATA_H_INCLUDED

class ImageData
{
public:

    Image3D<short>& image;
    Range3D box_fg;
    Range3D mid_box;
    Range3D seg_roi;
    vector<Index3D> fgseeds;
    vector<Index3D> bkseeds;
    vector<Index3D> istar2D;
    vector<Index3D> istar3D;
};


#endif // IMAGEDATA_H_INCLUDED
