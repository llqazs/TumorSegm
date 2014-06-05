#ifndef MYMATH_H_INCLUDED
#define MYMATH_H_INCLUDED

#include <vector>
#include "Data2D.h"
#include "Data3D.h"
#include "MyBasic.h"

///////////////////////////////////////////////////////
// Global operators for Data2D and Kernel2D objects //
///////////////////////////////////////////////////////

using namespace std;

//sum functions
namespace lliu
{
    template<typename T>
    double sum(const Data2D<T>& a)
    {
        return a.getSum();
    }

    template<typename T>
    double sum(const Data3D<T>& a)
    {
        return a.getSum();
    }

}

////operations for basic data structures
//MyBasic::Index2D operator+(const Index2D& op1, const Index2D& op2)
//{
//    Index2D result(op1);
//    return result+=op2;
//
//}
//
//MyBasic::Index2D operator-(const Index2D& op1, const Index2D& op2)
//{
//    Index2D result(op1);
//    return result-=op2;
//}
//
//MyBasic::Index3D operator+(const Index3D& op1, const Index3D& op2)
//{
//    Index3D result(op1);
//    return result+=op2;
//}
//
//MyBasic::Index3D operator-(const Index3D& op1, const Index3D& op2)
//{
//    Index3D result(op1);
//    return result-=op2;
//}


//operations for Data2D
template <class T>
Data2D<T> operator*(const Data2D<T>& a, const double& scalar)
{
    Data2D<T> result( a );
    return  result*= scalar;
}

template <class T>
Data2D<T> operator*(const double& scalar, const Data2D<T>& a)
{
    Data2D<T> result( a );
    return  result*= scalar;
}

template <class T>
Data2D<T> operator+(const Data2D<T>& a, const double& val)
{
    Data2D<T> result( a );
    return  result+= (T) val;
}

template <class T>
Data2D<T> operator+(const double& val, const Data2D<T>& a)
{
    Data2D<T> result( a );
    return  result+= (T) val;
}

template <class T>
Data2D<T> operator-(const Data2D<T>& a, const double& val)
{
    Data2D<T> result( a );
    return  result-= (T) val;
}

template <class T>
Data2D<T> operator-(const double& val, const Data2D<T>& a)
{
    Data2D<T> result(a.getNumCol(), a.getNumRow(), (T) val);
    return  result-= a;
}

template <class T>
Data2D<T> operator+(const Data2D<T>& a, const Data2D<T>& b)
{
    smart_assert( a.getNumCol()==b.getNumCol() && a.getNumRow()==b.getNumRow(),"Adding 2 arrays of different size (in operator+)");
    Data2D<T> result( a );
    return  result+=b;
}

template <class T>
Data2D<T> operator-(const Data2D<T>& a, const Data2D<T>& b)
{
    smart_assert( a.getNumCol()==b.getNumCol() && a.getNumRow()==b.getNumRow(),"Adding 2 arrays of different size (in operator-)");
    Data2D<T> result( a );
    return  result-=b;
}


///////////////////////////////////////////////////////////////////////////////
// operators for Data3D
//////////////////////////////////////////////////////////////////////////////
template <class T>
Data3D<T> operator*(const Data3D<T>& a, const double& scalar)
{
    Data3D<T> result( a );
    return  result*= scalar;
}

template <class T>
Data3D<T> operator*(const double& scalar, const Data3D<T>& a)
{
    Data3D<T> result( a );
    return  result*= scalar;
}

template <class T>
Data3D<T> operator+(const Data3D<T>& a, const double& val)
{
    Data3D<T> result( a );
    return  result+= (T) val;
}

template <class T>
Data3D<T> operator+(const double& val, const Data3D<T>& a)
{
    Data3D<T> result( a );
    return  result+= (T) val;
}

template <class T>
Data3D<T> operator-(const Data3D<T>& a, const double& val)
{
    Data3D<T> result( a );
    return  result-= (T) val;
}

template <class T>
Data3D<T> operator-(const double& val, const Data3D<T>& a)
{
    Data3D<T> result( a.getSize(), (T) val);
    return  result-= a;
}

template <class T>
Data3D<T> operator+(const Data3D<T>& a, const Data3D<T>& b)
{
    smart_assert( a.getSize() == b.getSize(),"Adding 2 arrays of different size (in operator+)");
    Data3D<T> result( a );
    return  result+=b;
}

template <class T>
Data3D<T> operator-(const Data3D<T>& a, const Data3D<T>& b)
{
    smart_assert( a.getSize() == b.getSize(),"Adding 2 arrays of different size (in operator-)");
    Data3D<T> result( a );
    return  result-=b;
}



#endif // MYMATH_H_INCLUDED
