#ifndef TESTGRAPH2D_H_INCLUDED
#define TESTGRAPH2D_H_INCLUDED

#include "../Graph2D.h"
#include "../BK/graph.h"
#include "common.h"
#include <iostream>
using namespace std;

bool TestGraph()
{

    Graph2D<double> g(2,1);

    Index2D i(0,0), j(0,1);
    g.add_edge(i,j,3,4);
    g.add_tweights(i,1,5);
    g.add_tweights(j,2,6);

    double flow = g.maxflow();
  //  cout<<flow<<endl;

    Data2D<LABEL> label = g.get_labeling();
    for(int i=0;i<2;i++) printf("%d ",label.at(i));

    return true;

}

#endif // TESTGRAPH2D_H_INCLUDED
