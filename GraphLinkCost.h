#ifndef GRAPHLINKCOST_H_INCLUDED
#define GRAPHLINKCOST_H_INCLUDED

#include "MyBasic.h"
#include "VectOperation.h"

//struct to store bidirected edge weights
template<typename T>
struct BidirectEdge
{
    vector<T> weight;
    BidirectEdge(void)
    {
        weight = vector<T>(2,T());
    }
    template<typename T2>
    BidirectEdge(const BidirectEdge<T2>& src)
    {
        weight = vector<T>(src.weight.begin(),src.weight.end());
    }
    BidirectEdge<T>& operator =(const BidirectEdge<T>& src) { weight = src.weight; return *this; }
    BidirectEdge<T>& operator *=(T scalar) { weight = vector<T>(weight)*scalar; return *this; }
    template<typename T2>
    operator T2() { return BidirectEdge<T2>(*this);}
};

//template<typename srcType, typename distType>
//BidirectEdge<distType> operator distType()(const BidirectEdge<srcType>& src)
//{
//    return BidirectEdge<distType>(src);
//}

template<typename T>
struct SparseBidirectEdge
{
    SparseBidirectEdge(MyBasic::Index2D p1, MyBasic::Index2D p2, T _w12, T _w21)
    {
        point2 = vector<MyBasic::Index2D>(2);
        weight = vector<T>(2);
        point2[0] = p1; point2[1] = p2;
        weight[0] = _w12; weight[1] = _w21;
    }
    SparseBidirectEdge(MyBasic::Index3D p1, MyBasic::Index3D p2, T _w12, T _w21)
    {
        point3 = vector<MyBasic::Index3D>(2);
        weight = vector<T>(2);
        point3[0] = p1; point3[1] = p2;
        weight[0] = _w12; weight[1] = _w21;
    }

    SparseBidirectEdge<T>& operator =(const SparseBidirectEdge<T>& src) { weight = src.weight; return *this; }
    SparseBidirectEdge<T>& operator *=(T scalar) { weight = vector<T>(weight)*scalar; return *this; }

    vector<MyBasic::Index2D> point2;
    vector<MyBasic::Index3D> point3;

    vector<T> weight;
};

template<typename T>
struct Edge4
{
    T weight[4];
    Edge4(void)
    {
        weight[0]=weight[1]=weight[2]=weight[3]=0;
    }
};

template<typename T>
struct DataCost
{
    DataCost(T _fgcost, T _bkcost):fgcost(_fgcost),bkcost(_bkcost) { }
    T fgcost, bkcost;
};

template<typename T>
struct SparseDataCost
{
    SparseDataCost(MyBasic::Index2D p, T _fgcost, T _bkcost):point2(p),fgcost(_fgcost),bkcost(_bkcost) {}
    SparseDataCost(MyBasic::Index3D p, T _fgcost, T _bkcost):point3(p),fgcost(_fgcost),bkcost(_bkcost) {}

      MyBasic::Index2D point2;
      MyBasic::Index3D point3;

    T fgcost, bkcost;
};

/*
enum NEIGHBOR
{
    uchar LEFT=0,FRONT,RIGHT,BACK,UP,DOWN
};

Index2D neighborSyms4(const Index2D& id, NEIGHBOR i)
{
    return id+neighborhood4[i];
}

Index3D neighborSyms6(const Index3D& id, NEIGHBOR i)
{
    return id+neighborhood6[i];
}

*/


#endif // EDGECOST_H_INCLUDED
