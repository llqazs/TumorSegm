#pragma once

/* This file is for 3D matrix, and it supports random access,
   copy data patches/blocks with bounding box or mask, min and max
   functions
*/


#include "MyBasic.h"
//#include "Data.h"
#include "smart_assert.h"

using namespace MyBasic;

template<typename T>
class Data3D
{


public:
	//////////////////////////////////////////////////////////////////////////////////
	//constructor and destructor
	//////////////////////////////////////////////////////////////////////////////////

	//rewrite default constructor
	Data3D()
	{
	  m_nSli = 0;
	  m_nRow = 0;
	  m_nCol = 0;
	}

	Data3D(int slice, int row, int col, T val = T()){
	    m_nSli = 0;
	    m_nRow = 0;
	    m_nCol = 0;
		resize(slice,row,col);
		setAll(val);
	}

	//constructor with single input parameter, use explicit
	explicit Data3D(MyBasic::Size3D size, T val = T()){
	    m_nSli = 0;
	    m_nRow = 0;
	    m_nCol = 0;
	    resize(size.nSli,size.nRow,size.nCol);
	    setAll(val);
	}

     //added on May 31
    Data3D(const Data3D<T>& src) : m_mat(src.m_mat)
    {
        m_nSli = src.getNumSli();
        m_nRow = src.getNumRow();
        m_nCol = src.getNumCol();
    }

    template<typename T2>
    Data3D(const Data3D<T2>& src) : m_mat(src.m_mat)
    {
        m_nSli = src.getNumSli();
        m_nRow = src.getNumRow();
        m_nCol = src.getNumCol();
    }

	virtual ~Data3D(void) {

	}

	//////////////////////////////////////////////////////////////////////////////////
	//print the matrix
	//////////////////////////////////////////////////////////////////////////////////
    void print(void) const
	{
		cout<<"The matrix is----------------->"<<endl;
		for(int z=0;z<m_nSli;z++)
		{
		 cout<<"The "<<z<<"th slice---->"<<endl;
		for(int y=0;y<m_nRow;y++)
		{
			for(int x=0;x<m_nCol;x++)
			   cout<<at(z,y,x)<<' ';
			cout<<endl;
		}
		}
	}


    //load and save
	void save(const std::string filename)
	{
		FILE* pFile=fopen(filename.c_str(),"wb");
		smart_assert(pFile,filename.c_str());

        unsigned int dim[3];
        dim[0] = m_nSli; dim[1] = m_nRow; dim[2] = m_nCol;
        //write dimension size
        fwrite(dim, sizeof(unsigned int),sizeof(dim),pFile);
        //write data
		fwrite(m_mat.m_pData,sizeof(T),dim[0]*dim[1]*dim[2],pFile);
		fclose(pFile);
	}

	void load(const std::string filename)
	{
		FILE* pFile=fopen(filename.c_str(),"rb");
		smart_assert(pFile,filename.c_str());

        //read dimension size
        unsigned int dim[3];
        fread(dim,sizeof(unsigned int),sizeof(dim),pFile);
        resize(dim[0],dim[1],dim[2]);
        //read data
		fread(m_mat.m_pData,sizeof(T),dim[0]*dim[1]*dim[2],pFile);
		fclose(pFile);
	}

	//////////////////////////////////////////////////////////////////////////////////
	//get operations
	//////////////////////////////////////////////////////////////////////////////////
	inline T& at(const int slice, const int row,const int col){
		smart_assert(inRange(slice,row,col), "Input index is out of range");
		return m_mat.at(slice,row*m_nCol+col);
	}

	inline T& at(const MyBasic::Index3D& p) {
		return at(p.sli,p.row,p.col);
	}

	inline T& at(const int id) {
		smart_assert(inRange(id),"Input index is out of range");
		return m_mat.at(id);
	}

	inline const T& at(const int slice, const int row,const int col)const {
		smart_assert(inRange(slice,row,col), "Input index is out of range");
		return m_mat.at(slice,row*m_nCol+col);
	}

	inline const T& at(const MyBasic::Index3D& p) const {
		return at(p.sli,p.row,p.col);
	}

	inline const T& at(const int id) const {
		smart_assert(inRange(id),"Input index is out of range");
		return m_mat.at(id);
	}

	//////////////////////////////////////////////////////////////////////////////////
	//set operation for a patch of data
	//////////////////////////////////////////////////////////////////////////////////

	void set(const Range& range_sli,const Range& range_row, const Range& range_col, T val)
	{
		set(Range3D(range_sli,range_row,range_col),val);
	}

	void set(const Range3D& range, T val) {
		smart_assert(inRange(range),"Bounding box is out of range");

		for(int sli=range.sli.start; sli<=range.sli.end;sli++)
		  for(int row=range.row.start; row<=range.row.end; row++)
			 for(int col=range.col.start; col<=range.col.end; col++)
			    at(sli,row,col) = val;
	}

	//set data using a mask
	void set(const Data3D<bool> &mask, T val)
	 {
		 if(m_nSli!=mask.getNumSli() || m_nRow!=mask.getNumRow() || m_nCol != mask.getNumCol()) resize(mask.getNumSli(),mask.getNumRow(),mask.getNumRow());
		 m_mat.set(mask.getMat(),val);
	 }

	//set all data
	void setAll(T val) { set(Range3D(Range(0,m_nSli-1),Range(0,m_nRow-1),Range(0,m_nCol-1)),val); }

	//////////////////////////////////////////////////////////////////////////////////
	//get mat
	//////////////////////////////////////////////////////////////////////////////////
	const Data2D<T>& getMat() const
	{
		return m_mat;
	}

    //edit on May 29, 2014
    Data2D<T> getSlice(int slice) const
    {
        return Data2D<T>(m_nRow, m_nCol,m_mat.getData()+ slice*m_nRow*m_nCol);

    }

	//////////////////////////////////////////////////////////////////////////////////
	//comparison
	//////////////////////////////////////////////////////////////////////////////////
	bool isEqualTo(const Data3D<T>& src) const {
		if(m_nSli!= src.getNumSli() || m_nCol!= src.getNumCol() || m_nRow != src.getNumRow()) return false;
		return m_mat.isEqualTo(src.getMat());
	}


	//////////////////////////////////////////////////////////////////////////////////
	//copy data
	//////////////////////////////////////////////////////////////////////////////////

	////copy whole data to object
	template<typename T2>
	void copyFrom(const Data3D<T2> &src) {
		if(m_nSli!=src.getNumSli() || m_nCol!=src.getNumCol() || m_nRow != src.getNumRow()) resize(src.getNumSli(),src.getNumRow(),src.getNumCol());
		m_mat.copyFrom(src.getMat());
	}


	//copy data using rects
	template<typename T2>
	void copyFrom(const Data3D<T2> &src,const Range3D &rsrc,const Range3D &rdst) {
		smart_assert(inRange(rdst), "The box is outof range");
		smart_assert(src.inRange(rsrc), "The destination box is outof range");
		smart_assert(Size3D::isEqual(rsrc.getSize(),rdst.getSize()),"The copy ranges should be equal");

		Size3D size(rsrc.getSize());
		for(int sli=0;sli<size.nSli;sli++)
		  for(int row=0; row<size.nRow; row++)
			for(int col=0; col<size.nCol; col++)
			{
				 at(rdst.sli.start+sli, rdst.row.start+row, rdst.col.start+col) = (T)src.at(rsrc.sli.start+sli, rsrc.row.start+row, rsrc.col.start+col);
			}

	}

	//copy data using a mask
	template<typename T2>
	void copyFrom(const Data3D<T2> &src,const Data3D<bool> &mask) //does not check memory overlap
	 {
		smart_assert(Size3D::isEqual(src.getSize(),mask.getSize()),"Sizes of mask and src don't match");
		if(m_nCol!=src.getNumCol() || m_nRow != src.getNumRow()) resize(src.getNumSli(), src.getNumRow(),src.getNumCol());

		m_mat.copyFrom(src.getMat(),mask.getMat());
	}

	//////////////////////////////////////////////////////////////////////////////////
	//min and max
	//////////////////////////////////////////////////////////////////////////////////
	T getMin() const{
		return m_mat.getMin();
	}

	T getMax() const{
		return m_mat.getMax();
	}

    double getSum(const MyBasic::Range2D& ) const
    {
        return m_mat.getSum();
    }
	//////////////////////////////////////////////////////////////////////////////////
	//check the input index is legal or not
	//////////////////////////////////////////////////////////////////////////////////
	inline bool inRange(const Index3D& p) const { return inRange(p.sli,p.row, p.col); }
	inline bool inRange(const Range3D& r) const { return inRange(Index3D(r.sli.start,r.row.start,r.col.start)) && inRange(Index3D(r.sli.end,r.row.end,r.col.end)); }
	inline bool inRange(int sli, int row, int col) const { return (0<=sli && sli<m_nSli && 0<=col && col<m_nCol && 0<=row && row<m_nRow); }
	inline bool inRange(int id) const { return m_mat.inRange(id); }

	//////////////////////////////////////////////////////////////////////////////////
	//tranforms between id and point
	//////////////////////////////////////////////////////////////////////////////////
	inline int getID(int sli, int row,int col) const {
		smart_assert(inRange(sli,row,col), "Input index is out of range");
		return sli*m_nRow*m_nCol+row*m_nCol+col;
	}
	inline int getID(MyBasic::Index3D& p) const {
		return getID(p.sli,p.row,p.col);
	}

	//////////////////////////////////////////////////////////////////////////////////
	//get data information
	//////////////////////////////////////////////////////////////////////////////////
	inline bool isEmpty() const {
		return m_mat.isEmpty();
	}

	//the 3D size and number of data contained in the object
	inline MyBasic::Size3D getSize() const { return Size3D(m_nSli,m_nRow,m_nCol); }
	inline int getNumCol() const { return m_nCol; }
	inline int getNumRow() const { return m_nRow; }
	inline int getNumSli() const { return m_nSli; }
	inline int getNumData() const { return m_mat.getNumData(); }
	inline int getCapacity() const { return m_mat.getCapacity();}

	void resize(int slice, int row, int col)
	{
		smart_assert(slice>=0 && row>=0 && col>=0, "Input size should be positive");
		m_nSli = slice;
		m_nCol = col;
		m_nRow = row;

		m_mat.resize(m_nSli,m_nRow*m_nCol);
	}


	//////////////////////////////////////////////////////////////////////////////////
	//operator overloading
	//////////////////////////////////////////////////////////////////////////////////

        // basic arithmetic operators
    Data3D<T>& operator+=(const Data3D<T>& arg); // adding another Data3D
    Data3D<T>& operator-=(const Data3D<T>& arg); // subtracting another Data3D
    Data3D<T>& operator+=(const T& val); // adding a constant to each element of the table
    Data3D<T>& operator-=(const T& val); // subtracting a constant from each element of the table
    Data3D<T>& operator*=(const double& s); // multiplication by a constant scalar



protected:
	int m_nCol;
	int m_nRow;
	int m_nSli;
	Data2D<T> m_mat;
};


//added on May 31
template <class T>
Data3D<T>& Data3D<T> :: operator+=(const Data3D<T>& arg) // adding another Data3D
{
    smart_assert( m_nCol==arg.getNumCol() && m_nRow==arg.getNumRow() && m_nSli == arg.getNumSli(),"Data3D of different size (in operator+=)");
    m_mat += arg.m_mat;
    return (*this);
}

template <class T>
Data3D<T>& Data3D<T> :: operator-=(const Data3D<T>& arg) // subtracting another Data3D
{
    smart_assert( m_nCol==arg.getNumCol() && m_nRow==arg.getNumRow() && m_nSli == arg.getNumSli(),"Data3D of different size (in operator-=)");
    m_mat -= arg.m_mat;
    return (*this);
}

template <class T>
Data3D<T>& Data3D<T> :: operator+=(const T& val) // adding a constant to each element
{
    m_mat += val;
    return (*this);
}

template <class T>
Data3D<T>& Data3D<T> :: operator-=(const T& val) // subtracting a constant from each element
{
    m_mat -= val;
    return (*this);
}

template <class T>
Data3D<T>& Data3D<T> :: operator*=(const double& scalar) // multiplication by a scalar
{
    m_mat *= scalar;
    return (*this);
}


