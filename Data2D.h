#pragma once

/* This file is for 2D matrix, and it supports random access,
   copy data patches/blocks with bounding box or mask, min and max
   functions
*/


#include "MyBasic.h"
#include "smart_assert.h"
#include <string.h>
#include <stdio.h>

using MyBasic::Range2D;
using MyBasic::Index2D;
using MyBasic::Size2D;
using MyBasic::Range;

/*
template<typename srcType, typename distType>
distType (*default_cast)(srcType a)
{
    return distType(a);
}
*/

template<typename T>
class Data2D
{

public:
	//////////////////////////////////////////////////////////////////////////////////
	//constructor and destructor
	//////////////////////////////////////////////////////////////////////////////////

	//rewrite default constructor
	Data2D()
    {
        m_nRow=0;
        m_nCol=0;
        m_numData =0;
        m_capacity =0;
        m_pData = NULL;
    }

	Data2D(int row, int col, T val = T()){
        m_nRow=0;
        m_nCol=0;
        m_numData =0;
        m_capacity =0;
        m_pData = NULL;
		resize(row,col);
		setAll(val);
	}

	//edit on May 29, 2014
	Data2D(int row, int col, const T* arr)
	{
        m_nRow=0;
        m_nCol=0;
        m_numData =0;
        m_capacity =0;
        m_pData = NULL;
		resize(row,col);
		memcpy(m_pData,arr, row*col*sizeof(T));
	}

	//constructor with single input parameter, use explicit
	explicit Data2D(MyBasic::Size2D size, T val = T()){
        m_nRow=0;
        m_nCol=0;
        m_numData =0;
        m_capacity =0;
        m_pData = NULL;
	    resize(size.nRow,size.nCol);
	    setAll(val);
	}

	//copy constrcutor
	Data2D(const Data2D<T>& src)
	{
        m_nRow=0;
        m_nCol=0;
        m_numData =0;
        m_capacity =0;
        m_pData = NULL;
	    copyFrom(src);

	}

	virtual ~Data2D(void) {
		delete[] m_pData;
		m_pData = NULL;
	}

	Data2D<T>& operator = (Data2D<T>& src)
	{
		copyFrom(src);
		return *this;
	}

	Data2D<T>& operator = (const Data2D<T>& src)
	{
		copyFrom(src);
		return *this;
	}

	template<typename T2>
	Data2D<T>& operator = (const Data2D<T2>& src)
	{
		copyFrom(src);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////////////
	//print the matrix
	//////////////////////////////////////////////////////////////////////////////////
    void print(void) const
   {
		cout<<"The matrix is---->"<<endl;
		for(int y=0;y<m_nRow;y++)
		{
			for(int x=0;x<m_nCol;x++)
			   cout<<m_pData[y*m_nCol+x]<<' ';
			cout<<endl;
		}
	}


    //load and save
	virtual void save(const std::string filename)
	{
		FILE* pFile=fopen(filename.c_str(),"wb");
		smart_assert(pFile,filename.c_str());

        unsigned int dim[2];
        dim[0] = m_nRow; dim[1] = m_nCol;
        //write dimension size
        fwrite(dim, sizeof(unsigned int),sizeof(dim),pFile);
        //write data
		fwrite(m_pData,sizeof(T),getNumData(),pFile);
		fclose(pFile);
	}

	virtual void load(const std::string filename)
	{
		FILE* pFile=fopen(filename.c_str(),"rb");
		smart_assert(pFile,filename.c_str());

        //read dimension size
        unsigned int dim[2];
        fread(dim,sizeof(unsigned int),sizeof(dim),pFile);
        resize(dim[0],dim[1]);
        //read data
		fread(m_pData,sizeof(T),dim[0]*dim[1],pFile);
		fclose(pFile);
	}

	//////////////////////////////////////////////////////////////////////////////////
	//get operations
	//////////////////////////////////////////////////////////////////////////////////
	inline T& at(const int row,const int col){
		smart_assert(inRange(row,col), "Input index is out of range");
		return m_pData[row*m_nCol+col];
	}

	inline T& at(const MyBasic::Index2D& p) {
		return at(p.row,p.col);
	}

	inline T& at(const int id) {
		smart_assert(inRange(id),"Input index is out of range");
		return m_pData[id];
	}

	inline const T& at(const int row,const int col) const {
		smart_assert(inRange(row,col), "Input index is out of range");
		return m_pData[row*m_nCol+col];
	}

	inline const T& at(const MyBasic::Index2D& p) const  {
		return at(p.row,p.col);
	}

	inline const T& at(const int id) const {
		smart_assert(inRange(id),"Input index is out of range");
		return m_pData[id];
	}

	//////////////////////////////////////////////////////////////////////////////////
	//set operation for a patch of data
	//////////////////////////////////////////////////////////////////////////////////

	void set(const Range& range_row, const Range& range_col, T val)
	{
		set(Range2D(range_row,range_col),val);
	}

	void set(const Range2D& range, T val) {
		smart_assert(inRange(range),"Bounding box is out of range");

		for(int row=range.row.start; row<=range.row.end; row++)
			for(int col=range.col.start; col<=range.col.end; col++)
			   m_pData[row*m_nCol+col] = val;
	}

	//set data using a mask
	void set(const Data2D<bool> &mask, T val)
	 {
	//	smart_assert(m_numData==mask.getNumData(),"The size of mask is illegal");
		 if(m_nRow!=mask.getNumRow() || m_nCol != mask.getNumCol()) resize(mask.getNumRow(),mask.getNumRow());
		for(int i=0;i<m_numData; i++) {
			if(mask.at(i)) m_pData[i] = val;
		}
	}
	//set all data
	void setAll(T val) {  if(m_pData) set(Range2D(Range(0,m_nRow-1),Range(0,m_nCol-1)),val); }

	//////////////////////////////////////////////////////////////////////////////////
	//comparison
	//////////////////////////////////////////////////////////////////////////////////
	bool isEqualTo(const Data2D<T>& src) const {
		if(m_nCol!= src.getNumCol() || m_nRow != src.getNumRow()) return false;
		for(int i=0;i<m_numData;i++)
			if(m_pData[i] != src.at(i)) return false;

		return true;
	}


    const T* getData() const { return m_pData; }
	//////////////////////////////////////////////////////////////////////////////////
	//copy data
	//////////////////////////////////////////////////////////////////////////////////

	////copy whole data to object
	void copyFrom(const Data2D<T> &src) {
		if(m_nCol!=src.getNumCol() || m_nRow != src.getNumRow()) resize(src.getNumRow(),src.getNumCol());
		for(int i=0;i<m_numData;i++)  m_pData[i] = src.at(i);
	}


	//copy data using rects
	void copyFrom(const Data2D<T> &src,const Range2D &rsrc, const Range2D &rdst) {
	    //if rdst == MyBasic::Range2D::all(), problems will happen
		smart_assert(inRange(rdst), "The box is outof range");
		smart_assert(src.inRange(rsrc), "The source box is outof range");
		smart_assert(Size2D::isEqual(rsrc.getSize(),rdst.getSize()),"The copy ranges should be equal");

		Size2D size(rsrc.getSize());
		for(int row=0; row<size.nRow; row++)
			for(int col=0; col<size.nCol; col++)
			{
				at(rdst.row.start+row,rdst.col.start+col) = src.at(rsrc.row.start+row,rsrc.col.start+col);
			}
	}

	//copy data using a mask
	void copyFrom(const Data2D<T> &src,const Data2D<bool> &mask)  //does not check memory overlap
	 {
		smart_assert(Size2D::isEqual(src.getSize(),mask.getSize()),"Sizes of mask and src don't match");
		if(m_nCol!=src.getNumCol() || m_nRow != src.getNumRow()) resize(src.getNumRow(),src.getNumCol());

		for(int i=0;i<m_numData; i++) {
			if(mask.at(i)) m_pData[i]  =  src.at(i);
		}
	}


	////copy whole data to object
	template<typename T2>
	void copyFrom(const Data2D<T2> &src) {
		if(m_nCol!=src.getNumCol() || m_nRow != src.getNumRow()) resize(src.getNumRow(),src.getNumCol());
		for(int i=0;i<m_numData;i++)  m_pData[i] = T2(src.at(i));
	}


	//copy data using rects
	template<typename T2>
	void copyFrom(const Data2D<T2> &src,const Range2D &rsrc, const Range2D &rdst) {
	    //if rdst == MyBasic::Range2D::all(), problems will happen
		smart_assert(inRange(rdst), "The box is outof range");
		smart_assert(src.inRange(rsrc), "The source box is outof range");
		smart_assert(Size2D::isEqual(rsrc.getSize(),rdst.getSize()),"The copy ranges should be equal");

		Size2D size(rsrc.getSize());
		for(int row=0; row<size.nRow; row++)
			for(int col=0; col<size.nCol; col++)
			{
				at(rdst.row.start+row,rdst.col.start+col) = T2(src.at(rsrc.row.start+row,rsrc.col.start+col));
			}
	}

	//copy data using a mask
	template<typename T2>
	void copyFrom(const Data2D<T2> &src,const Data2D<bool> &mask)  //does not check memory overlap
	 {
		smart_assert(Size2D::isEqual(src.getSize(),mask.getSize()),"Sizes of mask and src don't match");
		if(m_nCol!=src.getNumCol() || m_nRow != src.getNumRow()) resize(src.getNumRow(),src.getNumCol());

		for(int i=0;i<m_numData; i++) {
			if(mask.at(i)) m_pData[i]  = T2(src.at(i));
		}
	}

	//////////////////////////////////////////////////////////////////////////////////
	//min and max
	//////////////////////////////////////////////////////////////////////////////////
	T getMin() const{
		smart_assert(!isEmpty(),"The Data is empty");

		T min = m_pData[0];
		for(int i=1;i<m_numData;i++){
			if(m_pData[i]<min) min = m_pData[i];
		}
		return min;
	}

	T getMax() const{
		smart_assert(!isEmpty(),"The Data is empty");

		T max = m_pData[0];
		for(int i=1;i<m_numData;i++){
			if(m_pData[i]>max) max = m_pData[i];
		}
		return max;
	}

	double getSum() const
	{
	    double sumval = 0;
	    for(int i=0;i<m_numData;i++) sumval += m_pData[i];

	    return sumval;
	}

	//////////////////////////////////////////////////////////////////////////////////
	//check the input index is legal or not
	//////////////////////////////////////////////////////////////////////////////////
	inline bool inRange(const Index2D& p) const { return inRange(p.row, p.col); }
	inline bool inRange(const Range2D& r) const
    {
	  return inRange(Index2D(r.row.start,r.col.start)) && inRange(Index2D(r.row.end,r.col.end));
    }

	inline bool inRange(int row, int col) const { return (0<=col && col<m_nCol && 0<=row && row<m_nRow); }
	inline bool inRange(int id) const { return (0<=id && id<m_numData); }

	//////////////////////////////////////////////////////////////////////////////////
	//tranforms between id and point
	//////////////////////////////////////////////////////////////////////////////////
	inline int getID(int row,int col) const {
		smart_assert(inRange(row,col), "Input index is out of range");
		return row*m_nCol+col;
	}
	inline int getID(MyBasic::Index2D& p) const {
		return getID(p.row,p.col);
	}

	//////////////////////////////////////////////////////////////////////////////////
	//get data information
	//////////////////////////////////////////////////////////////////////////////////
	inline bool isEmpty() const {
		return m_numData==0;
	}

	//the 3D size and number of data contained in the object
	inline MyBasic::Size2D getSize() const { return Size2D(m_nRow,m_nCol); }
	inline int getNumCol() const { return m_nCol; }
	inline int getNumRow() const { return m_nRow; }
	inline int getNumData() const { return m_numData; }
	inline int getCapacity() const { return m_capacity; }

	void resize(int row, int col)
	{
		smart_assert(row>=0 && col>=0, "Input size should be positive");
		m_nCol = col;
		m_nRow = row;
		int new_m_numData = row*col;
		if(new_m_numData > m_capacity )
		{
			T* new_m_pData = new T[row*col];
			if(m_numData>0) memcpy(new_m_pData,m_pData,m_numData*sizeof(T));
			memset(new_m_pData+m_numData,0,(new_m_numData-m_numData)*sizeof(T));
			m_capacity  = new_m_numData;
			delete[] m_pData;
			m_pData = new_m_pData;
		}
		m_numData = new_m_numData;
	}

	//////////////////////////////////////////////////////////////////////////////////
	//operator overloading
	//////////////////////////////////////////////////////////////////////////////////
  //  Data2D<T> operator()(const MyBasic::Range2D& roi) const;
        // basic arithmetic operators
    Data2D<T>& operator+=(const Data2D<T>& arg); // adding another Data2D
    Data2D<T>& operator-=(const Data2D<T>& arg); // subtracting another Data2D
    Data2D<T>& operator+=(const T& val); // adding a constant to each element of the table
    Data2D<T>& operator-=(const T& val); // subtracting a constant from each element of the table
    Data2D<T>& operator*=(const double& s); // multiplication by a constant scalar
    Data2D<T>& operator/=(const double& s); //division by a constant scalar
    Data2D<T> operator()(const MyBasic::Range2D& ROI) const;

protected:
	int m_nCol;
	int m_nRow;
	int m_capacity;
	int m_numData;
	T* m_pData;
};

//added on May 31
template <class T>
Data2D<T>& Data2D<T> :: operator+=(const Data2D<T>& arg) // adding another Data2D
{
    smart_assert( m_nCol==arg.getNumCol() && m_nRow==arg.getNumRow(),"Data2D of different size (in operator+=)");
    for (unsigned i=0; i<(m_nCol*m_nRow); i++) m_pData[i] += arg.m_pData[i];
    return (*this);
}

template <class T>
Data2D<T>& Data2D<T> :: operator-=(const Data2D<T>& arg) // subtracting another Data2D
{
    smart_assert( m_nCol==arg.getNumCol() && m_nRow==arg.getNumRow(),"Data2D of different size (in operator-=)");
    for (unsigned i=0; i<(m_nCol*m_nRow); i++) m_pData[i] -= arg.m_pData[i];
    return (*this);
}

template <class T>
Data2D<T>& Data2D<T> :: operator+=(const T& val) // adding a constant to each element
{
    for (unsigned i=0; i<(m_nCol*m_nRow); i++) m_pData[i] += val;
    return (*this);
}

template <class T>
Data2D<T>& Data2D<T> :: operator-=(const T& val) // subtracting a constant from each element
{
    for (unsigned i=0; i<(m_nCol*m_nRow); i++) m_pData[i] -= val;
    return (*this);
}

template <class T>
Data2D<T>& Data2D<T> :: operator*=(const double& scalar) // multiplication by a scalar
{
    for (unsigned i=0; i<(m_nCol*m_nRow); i++) m_pData[i] *= scalar;
    return (*this);
}

template <class T>
Data2D<T>& Data2D<T> :: operator/=(const double& scalar) // multiplication by a scalar
{
    smart_assert(scalar!=0, "Divided by 0");
    for (unsigned i=0; i<(m_nCol*m_nRow); i++) m_pData[i] /= scalar;
    return (*this);
}

template <class T>
Data2D<T> Data2D<T> :: operator()(const MyBasic::Range2D& ROI) const
{
    Data2D<T> result(ROI.getSize());
    result.copyFrom(*this,ROI,MyBasic::Range2D(0,result.getNumRow()-1,0,result.getNumCol()-1));
    return result;
}
