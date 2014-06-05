#ifndef APPEARANCEMODEL_H_INCLUDED
#define APPEARANCEMODEL_H_INCLUDED

#include "Histogram.h"
#include "Data2D.h"
#include "Data3D.h"
#include <math.h>

double const SMALLCONST = 1.0e-3;

namespace lliu
{

Data2D<double> LogLikelihood(const Data2D<int>& bin_id, const vector<double>& prob)
{
    Data2D<double> cost(bin_id.getSize());
    for(int i=0;i<bin_id.getNumData();i++)
    {
        cost.at(i) = -log(prob[bin_id.at(i)]+SMALLCONST);
    }

    return cost;
}

Data3D<double> LogLikelihood(const Data3D<int>& bin_id, const vector<double>& prob)
{
    Data3D<double> cost(bin_id.getSize());
    for(int i=0;i<bin_id.getNumData();i++)
    {
        cost.at(i) = -log(prob[bin_id.at(i)]+SMALLCONST);
    }

    return cost;
}

};

#endif // APPEARANCEMODEL_H_INCLUDED
