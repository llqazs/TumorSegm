#ifndef SMOOTHTERM_H_INCLUDED
#define SMOOTHTERM_H_INCLUDED

#include "common.h"
#include <math.h>
#include <vector>
using namespace std;

namespace lliu
{
    double computeNLinkCost(double diff,double variance)
    {
        return exp(-(diff*diff)/(variance*variance));
    }

    template<typename T>
    vector<Data2D<double> > ComputeSmoothCostWhiteToDark(Data2D<T>& image,double dist,double variance, DIRECTION dir)
    {
        vector<Data2D<double> > sm(2,Data2D<double>(image.getNumRow(),image.getNumCol()));
        MyBasic::Range2D roi;
        MyBasic::Index2D shift;
        switch(dir)
        {
            case ROW:
                      roi = Range2D(0,image.getNumRow()-1,0,image.getNumCol()-2);
                      shift = MyBasic::Index2D(0,1);
                      break;
            case COLUMN:
                      roi = Range2D(0,image.getNumRow()-2,0,image.getNumCol()-1);
                      shift =  MyBasic::Index2D(1,0);
                      break;
        }

        double default_weight = 1/dist;

        for(int r=roi.row.start; r<=roi.row.end;r++)
        {
            for(int c=roi.col.start; c<=roi.col.end;c++)
            {
                MyBasic::Index2D point1(r,c);
                MyBasic::Index2D point2 = point1+shift;

                double diff = image.at(point1) - image.at(point2);
                double w = computeNLinkCost(diff,variance);

                if(w>0)
                {
                    sm[0].at(point1) = w/dist;
                    sm[1].at(point2) = default_weight;
                }
                else
                {
                    sm[0].at(point1) = default_weight;
                    sm[1].at(point2) = w/dist;
                }
            }
        }

        return sm;
    }
};

#endif // SMOOTHTERM_H_INCLUDED
