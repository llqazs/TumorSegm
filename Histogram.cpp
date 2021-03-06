#include "Histogram.h"

    // data:        data to be binned
    // bin_id:      bin id for each element in data
    // bin_num:     number of bins used
    // mask:        indicator of which elements to hist on
//template Data2D<int> lliu::binning<short>(const Data2D<short>&,int);

//template<typename T>
//int getBinID(T x,T binmin,T binmax,int binnum)
//{
//	bin_id id=(int)floor((binnum-1)*(x-binmin)/(binmax-binmin) + 0.5);
//	//if(id==binnum) id=binnum-1;
//	return id;
//}
//
//    template<typename T>
//    Data2D<int> binning(const Data2D<T>& data, int bin_num)
//    {
//        Data2D<int> bin_id(data.getSize());
//        T dmin = data.getMin();
//        T dmax = data.getMax();
//
//        for(int i=0;i<data.getNumData();i++)
//        {
//            bin_id.at(i) = getBinID<T>(data.at(i),dmin,dmax,bin_num);
//        }
//
//        return bin_id;
//    }

    template<typename T>
    Data3D<int> lliu::binning(const Data3D<T>& data,  int bin_num)
    {
        Data3D<int> bin_id(data.getSize());
        T dmin = data.getMin();
        T dmax = data.getMax();

        for(int i=0;i<data.getNumData();i++)
        {
            bin_id.at(i) = getBinID<T>(data.at(i),dmin,dmax,bin_num);
        }

        return bin_id;
    }

    vector<double> lliu::hist(const Data2D<int>& bin_id, const Data2D<int>& mask, int bin_num, int indicator = 1)
    {
        vector<double> vhist(bin_num,0);
        smart_assert(bin_id.getSize()==mask.getSize(),"size not equal");

        for(int i=0;i<bin_id.getNumData();i++)
        {
            if(mask.at(i)==indicator) vhist[bin_id.at(i)] = vhist[bin_id.at(i)]+1;
        }

        return vhist;
    }

    vector<double> lliu::hist(const Data3D<int>& bin_id, const Data3D<int>& mask, int bin_num, int indicator = 1)
    {
        return hist(bin_id.getMat(),mask.getMat(),bin_num,indicator);
    }

