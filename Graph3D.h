#pragma once

#include "BK/graph.h"
#include "common.h"
#include "MyBasic.h"
#include "Data3D.h"
//#include "GraphLinkCost.h"
#include "Neighbor4.h"
#include <vector>
using std::vector;

template<typename T>
class Graph3D : public Graph<T,T,T>
{
	using Graph<T,T,T>::add_edge;
	using Graph<T,T,T>::add_tweights;
    using Graph<T,T,T>::what_segment;
public:
	Graph3D(int _sli, int _row, int _col);
	//virtual ~Graph3D();

	inline void add_edge(const Index3D& i, const Index3D& j, const T& cap, const T& rev_cap);
	inline void add_tweights(const Index3D& i, const T& fgcost, const T& bkcost);

 //   void setSmoothCost(const Data3D<Edge4<T> >& smoothcost);
    void setSmoothCost(const Data3D<BidirectEdge<T> >& smoothcost, const Index3D& shift);
    void setSmoothCost(const vector<SparseEdgeCost<T> >& smoothcost);

    void setDataCost(const Data3D<DataCost<T> >& datacost);
    void setDataCost(const vector<SparseDataCost<T> >& datacost);

	Data3D<uint8> get_labeling();

    int getNumSli() const;
    int getNumRow() const;
    int getNumCol() const;
    Size3D getSize() const;

private:
    int Index2Id(const Index3D& i) const;

private:
    int nSli;
	int nRow;
	int nCol;
};

template<typename T>
Graph3D<T>::Graph3D(int _sli, int _row, int _col):Graph<T,T,T>(_sli*_row*_col,8*_row*_col),nSli(_sli),nRow(_row),nCol(_col)
{
    this->add_node(_sli*_row*_col);
}

template<typename T>
inline void Graph3D<T>::add_edge(const Index3D& i, const Index3D& j, const T& cap, const T& rev_cap)
{
	add_edge(Index2Id(i),Index2Id(j),cap,rev_cap);
}

template<typename T>
inline void Graph3D<T>::add_tweights(const Index3D& i, const T& fgcost, const T& bkcost)
{
	this->add_tweights(Index2Id(i),fgcost, bkcost);
}

template<typename T>
void Graph3D<T>::setSmoothCost(const Data3D<EdgeCost<T> >& smoothcost, const Index3D& shift)
{
    Range3D roi(0,nSli-1-shift.sli, 0,nRow-1-shift.row, 0, nCol-1-shift.col);

    for(int s=roi.sli.start; s<=roi.sli.end; s++)
    for(int r=roi.row.start; r<=roi.row.end; r++)
    {
        for(int c=roi.col.start; c<=roi.col.end; c++)
        {
            add_edge(Index2Id(Index3D(s,r,c)),Index2Id(Index3D(s,r,c)+shift),smoothcost.at(s,r,c).w12,smoothcost.at(s,r,c).w21);
        }
    }
}

template<typename T>
void Graph3D<T>::setSmoothCost(const vector<SparseEdgeCost<T> >& smoothcost)
{
    int num = smoothcost.size();
    for(int i=0;i<num;i++)
    {
        add_edge(smoothcost[i].id1, smoothcost[i].id2, smoothcost[i].w12, smoothcost[i].w21);
    }
}

template<typename T>
void Graph3D<T>::setDataCost(const Data3D<DataCost<T> >& datacost)
{
    int node_num = nSli*nRow*nCol;
    for(int i=0;i<node_num;i++)
    {
        add_tweights(i,datacost.at(i).fgcost, datacost.at(i).bkcost);
    }

}

template<typename T>
void Graph3D<T>::setDataCost(const vector<SparseDataCost<T> >& datacost)
{
    int num = datacost.size();
    for(int i=0;i<num;i++)
    {
        add_edge(datacost[i].id, datacost[i].fgcost,datacost[i].bkcost);
    }
}



template<typename T>
Data3D<uchar> Graph3D<T>::get_labeling()// const
{
	Data3D<uchar> label(nRow,nCol);

	for(int s=0;s<nSli;s++)
    {
	for(int r=0;r<nRow;r++)
	{
		for(int c=0;c<nCol;c++)
		{
		    label.at(s,r,c) = what_segment(Index3D(s,r,c));
		}
	}
    }
	return label;
}

template<typename T>
int Graph3D<T>::Index2Id(const Index3D& i) const
{
	return i.sli*nRow*nCol+i.row*nCol + i.col;
}

template<typename T>
uchar Graph3D<T>::what_segment(const Index3D& i)// const
{
	return what_segment(Index2Id(i));
}



