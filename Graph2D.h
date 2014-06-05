#pragma once

#include "BK/graph.h"
#include "common.h"
#include "MyBasic.h"
#include "Data2D.h"
#include "MyMath.h"
#include <vector>
using std::vector;

class Graph2D : public Graph<double,double,double>
{
/*
	using Graph<double,double,double>::add_edge;
	using Graph<double,double,double>::add_tweights;
    using Graph<double,double,double>::what_segment;
*/
public:
	Graph2D(int _row, int _col);
	virtual ~Graph2D();

	inline void add_edge(const MyBasic::Index2D& i, const MyBasic::Index2D& j, const double& cap, const double& rev_cap);
	inline void add_tweights(const MyBasic::Index2D& i, const double& fgcost, const double& bkcost);

    void addSmoothCost(const Data2D<double>& cap,const Data2D<double>& rev_cap, DIRECTION d);
    void addSmoothCost(const Data2D<double>& cap,const Data2D<double>& rev_cap, const MyBasic::Index2D& shift);
    //sparse smooth cost
    void addSmoothCost(const vector<MyBasic::Index2D>& point1, const vector<MyBasic::Index2D>& point2,const vector<double>& cap,const vector<double>& rev_cap);

    void addDataCost(const vector<MyBasic::Index2D>& point,const vector<double>& cost0, const vector<double>& cost1);
    void addDataCost(const Data2D<double>& datacost, LABEL label);


	Data2D<LABEL> get_labeling();
	LABEL what_segment(const Index2D& i);
    double maxflow();

    inline int getNumRow() const ;
    inline int getNumCol() const ;
    inline MyBasic::Size2D getSize() const ;

private:
    inline int Index2Id(const MyBasic::Index2D& i) const;

private:
	int nRow;
	int nCol;
	double flow;
	bool done;
};

//implementation of inline functions
void Graph2D::add_edge(const MyBasic::Index2D& i, const MyBasic::Index2D& j, const double& cap, const double& rev_cap)
{
	Graph<double,double,double>::add_edge(Index2Id(i),Index2Id(j),cap,rev_cap);
}


void Graph2D::add_tweights(const MyBasic::Index2D& i, const double& fgcost, const double& bkcost)
{
	Graph<double,double,double>::add_tweights(Index2Id(i),fgcost, bkcost);
}


int Graph2D::Index2Id(const Index2D& i) const
{
	return i.row*nCol + i.col;
}


int Graph2D:: getNumRow() const
{
    return nRow;
}


int Graph2D:: getNumCol() const
{
    return nCol;
}

MyBasic::Size2D Graph2D::getSize() const
{
    return MyBasic::Size2D(nRow,nCol);
}





