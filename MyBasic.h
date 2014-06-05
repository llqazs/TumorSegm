#pragma once

#include "common.h"
#include <climits>

namespace MyBasic {

class Size2D {
public:
	Size2D() {set(0,0); }
	Size2D(int r, int c) { set(r,c); }
	void set(int r, int c) { nRow = r; nCol = c; }
	void set(const Size2D& s) { set(s.nRow,s.nCol); }
	bool isZero(void) const { return nRow==0 || nCol==0; }
	bool operator==(const Size2D& src) {return Size2D::isEqual(*this,src);}
	static inline bool isEqual(const Size2D& s1, const Size2D& s2)
	{
		return s1.nRow==s2.nRow && s1.nCol==s2.nCol;
	}
	int nRow, nCol;
};

class Size3D{
public:
	Size3D():nRow(0),nCol(0),nSli(0) {}
	Size3D(int s,int r, int c) { set(s,r,c); }
	void set(int s,int r, int c) { nRow = r; nCol = c; nSli = s; }
	void set(const Size3D& s) { set(s.nSli, s.nRow, s.nCol); }
	bool isZero(void) const { return nRow==0 || nCol==0 || nSli==0; }
	bool operator==(const Size3D& src) {return Size3D::isEqual(*this,src);}
	static inline bool isEqual(const Size3D& s1, const Size3D& s2)
	{
		return s1.nRow==s2.nRow && s1.nCol==s2.nCol && s1.nSli==s2.nSli;
	}

	int nRow,nCol,nSli;
};

class Range {
public:
 		Range() { start=0; end = 0; }
		Range(const Range& src) { start = src.start; end = src.end; }
 		Range(int _start, int _end) {start=_start; end=_end;}

		Range& operator =(const Range& src) { start = src.start; end = src.end; return *this; }

 		int getSize() const {return end-start+1; }
		bool isEmpty() const { return (0==getSize()); }

		static inline bool isEqual(const Range& r1, const Range& r2)
		{
			return r1.start==r2.start && r1.end==r2.end;
		}

		void set(const Range& r) { start = r.start; end = r.end;}
		static inline Range all() { return Range(INT_MIN, INT_MAX); }

public:
 	    int start, end;
};

class Range2D{
public:
	Range2D() { }
	Range2D(const Range2D& src):row(src.row),col(src.col) {}

	Range2D(int rst,int rend,int cst, int cend) {
		row.start = rst;  row.end = rend;
		col.start = cst; col.end = cend;
	}
	Range2D(const Range& r, const Range& c){ row.set(r); col.set(c); }

	Range2D& operator =(const Range2D& src)
	{
		row = src.row;
		col = src.col;
		return *this;
	}
    bool operator ==(const Range2D& src) const { return isEqual(*this,src); }

	void set(const Range& r, const Range& c) { row.set(r); col.set(c); }
	void setRow(const Range& r) {row.set(r);}
	void setCol(const Range& c) {col.set(c);}

	int getRowStart() const { return row.start; }
	int getRowEnd() const { return row.end; }
	int getColStart() const { return col.start; }
	int getColEnd() const { return col.end; }
	Size2D getSize() const { return Size2D(row.getSize(),col.getSize()); }

	bool isEmpty() const { return row.isEmpty() || col.isEmpty(); }
	static inline bool isEqual(const Range2D& r1, const Range2D& r2)
	{
		return Range::isEqual(r1.row,r2.row) && Range::isEqual(r1.col,r2.col);
	}

	static inline Range2D all() { return Range2D(Range::all(),Range::all()); }

public:
	Range row;
	Range col;
};

class Range3D{
public:
	Range3D(){}
	Range3D(int sst, int send, int rst,int rend,int cst, int cend) {
		sli.start = sst; sli.end = send;
		row.start = rst;  row.end = rend;
		col.start = cst; col.end = cend;
	}
	Range3D(const Range& s, const Range& r, const Range& c){ row.set(r); col.set(c); sli.set(s); }

	void set(const Range& s, const Range& r, const Range& c) { row.set(r); col.set(c); sli.set(s);}
	void setRow(const Range& r) {row.set(r);}
	void setCol(const Range& c) {col.set(c);}
	void setSli(const Range& s) {sli.set(s);}

	Size3D getSize()const { return Size3D(sli.getSize(),row.getSize(),col.getSize());}

	bool isEmpty() const { return row.isEmpty() || col.isEmpty() || sli.isEmpty(); }
	static inline bool isEqual(const Range3D& r1, const Range3D& r2)
	{
		return Range::isEqual(r1.row,r2.row) && Range::isEqual(r1.col,r2.col) && Range::isEqual(r1.sli,r2.sli);
	}

	static inline Range3D all() { return Range3D(Range::all(),Range::all(), Range::all()); }

public:
	Range row;
	Range col;
	Range sli;
};

class Index2D {
public:
	Index2D(){set(0,0); }
	Index2D(int r, int c){set(r,c);}
	void set(int r, int c) { row = r; col = c;}
	void set(const Index2D& ind) { set(ind.row, ind.col); }
	Index2D& operator =(const Index2D& src) { set(src); return *this; }
	Index2D& operator +=(const Index2D& op) { row += op.row; col += op.col; return *this; }
	Index2D& operator -=(const Index2D& op) { row -=op.row; col-=op.col; return *this; }
	bool isValid() const { return row>=0 && col>=0; }
	int row, col;
};

class Index3D {
public:
	Index3D(){set(0,0,0); }
	Index3D(int s,int r, int c) {set(s,r,c);}
	void set(int s,int r, int c) { sli = s; row = r; col = c;}
	void set(const Index3D& ind) { set(ind.sli, ind.row, ind.col); }

	Index3D& operator =(const Index3D& src) { set(src); return *this; }
	Index3D& operator +=(const Index3D& op) { sli += op.sli; row += op.row; col += op.col; return *this; }
    Index3D& operator -=(const Index3D& op) { sli-=op.sli; row -=op.row; col-=op.col; return *this; }

	bool isValid() const { return row>=0 && col>=0 && sli>=0; }
	int sli,row, col;
};

}

//operations for basic data structures
inline MyBasic::Index2D operator +(const MyBasic::Index2D& op1, const MyBasic::Index2D& op2)
{
    MyBasic::Index2D result(op1);
    return result+=op2;

}

inline MyBasic::Index2D operator-(const MyBasic::Index2D& op1, const MyBasic::Index2D& op2)
{
    MyBasic::Index2D result(op1);
    return result-=op2;
}

inline MyBasic::Index3D operator+(const MyBasic::Index3D& op1, const MyBasic::Index3D& op2)
{
    MyBasic::Index3D result(op1);
    return result+=op2;
}

inline MyBasic::Index3D operator-(const MyBasic::Index3D& op1, const MyBasic::Index3D& op2)
{
    MyBasic::Index3D result(op1);
    return result-=op2;
}
