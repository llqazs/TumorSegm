#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream> // For reading and saving files
#include "common.h"
#include "MyBasic.h"
#include "Data2D.h"
#include "smart_assert.h"

using std::string;
using MyBasic::Range2D;
using MyBasic::Range;
using MyBasic::Index2D;

//To do------------------------------------------------------------------
// 1) Think up some usage of ROI

template<typename T>
class Image2D : public Data2D<T>
{
public:
	Image2D(int r=0, int c=0);
	Image2D(const Data2D<T>& src);

	void load(const string& filename);
	void load_info(const string& filename);
	void save(const string& filename) const;
	void save_info(const string& filename) const;

    Data2D<T> dX() const;
    Data2D<T> dY() const;

	inline int getWidth() const;
	inline int getHeight() const;
    inline int getDistR() const;
    inline int getDistC() const;

    Image2D<T> getImageROI(const MyBasic::Range2D& r) const;

    inline double getVariance() const;
	inline Range2D getROI() const;
	inline void setROI(const Range2D& _roi);
	inline void resetROI();

private:
    void computeVariance() const;
    Data2D<T> dI(DIRECTION d) const;

private:
	double distR;
	double distC;
	mutable double variance;
	Range2D ROI;
};


template<typename T>
Image2D<T>::Image2D(int r, int c) : Data2D<T>(r,c),ROI(MyBasic::Range2D(0,r-1,0,c-1))
{
	distR = 1; distC = 1;
	variance = -1;
}

template<typename T>
Image2D<T>::Image2D(const Data2D<T>& src): Data2D<T>(src), ROI(MyBasic::Range2D(0,src.getNumRow()-1,0,src.getNumCol()-1))
{
    distR = 1; distC = 1;
}

template<typename T>
void Image2D<T>::load(const string& filename)
{
    //load dimension information and allocate the memory first
    load_info(filename);

	FILE* pFile=fopen(filename.c_str(),"rb");
	smart_assert(pFile,filename.c_str());

	int readsize=fread(this->m_pData,sizeof(T),this->m_numData,pFile);
	smart_assert(readsize==this->getNumData(),"File size dose not match with data size");
	//change the size of the image
}

template<typename T>
void Image2D<T>::load_info(const string& filename)
{
	string info_file = filename + "_info.txt";

	// open file
	ifstream fin( info_file.c_str() );
	smart_assert(fin.is_open(),"The info file is not found");

	// size of the data
	int row, col;
	fin >> row;
	fin >> col;
	fin.ignore(255, '\n');
	//spacing information
	fin >> distR;
	fin >> distC;

    fin.close();

    this->resize(row,col);
}

template<typename T>
void Image2D<T>::save(const string& filename) const
{
	smart_assert(!filename.empty(),"No file for written");
    smart_assert(!this->isEmpty(),"Image is empty, can't save");

    save_info(filename);
	unsigned int size=this->getNumData();

	FILE* pFile=fopen ( filename.c_str() , "wb" );
	smart_assert(pFile,"File error");

	fwrite(this->m_pData,sizeof(T),size,pFile);
	fclose(pFile);
}

template<typename T>
void Image2D<T>::save_info(const string& filename) const
{
	smart_assert(!filename.empty(),"No file for written");

    string info_file = filename + "_info.txt";
	FILE* pFile = fopen(info_file.c_str(),"wt");
	smart_assert(pFile,"File error");

	ofstream fout( info_file.c_str() );
	fout << this->getNumRow() << " ";
	fout << this->getNumCol() << "\n";

	//write spacing information
	fout << distR << " ";
	fout << distC << " ";

	fout.close();
}

template<typename T>
int Image2D<T>::getWidth() const
{
	return this->getNumCol();
}

template<typename T>
int Image2D<T>::getHeight() const
{
	return this->getNumRow();
}

template<typename T>
int Image2D<T>::getDistR() const
{
	return this->distR;
}

template<typename T>
int Image2D<T>::getDistC() const
{
	return this->distC;
}

template<typename T>
Image2D<T> Image2D<T>::getImageROI(const MyBasic::Range2D& r) const
{
    Image2D<T> img(r.getSize());
    img.copyFrom(*this, r, MyBasic::Range2D(0,img.getNumRow()-1,0,img.getNumCol()-1));

    return img;
}

template<typename T>
inline double Image2D<T>::getVariance() const
{
    if(variance<0) this->computeVariance();

    return this->variance;
}

template<typename T>
void Image2D<T>::computeVariance() const
{
	double DEFAULT_VARIANCE =  0.0001;
	double v = 0.0;
	int total = 0;
//	MyBasic::Range2D r(0,this->getNumRow()-2,0,this->getNumCol()-2);
    MyBasic::Range row(0,this->getNumRow()-2);
    MyBasic::Range col(0,this->getNumCol()-2);

    for(int r=row.start; r<=row.end; r++)
    {
        for(int c=col.start; c<=col.end; c++)
        {
            v=v+abs(this->at(r,c)-this->at(r+1,c))+abs(this->at(r,c)-this->at(r,c+1));
            total=total+2;
        }
    }

    if (v/total < DEFAULT_VARIANCE ) this->variance = DEFAULT_VARIANCE;
    else this->variance = v/total;
}

template<typename T>
Data2D<T> Image2D<T>::dI(DIRECTION d) const
{
	smart_assert(d<=1, "Input direction does not work for 2D image");

	Data2D<T> gd(this->getNumRow(),this->getNumCol());

	Range row, col;
	int shift[2]={0};
	if(d==ROW)
	{
		row=Range(0,this->getNumRow()-1);
		col=Range(0,this->getNumCol()-2);
		shift[1] = 1;
	}
	else
	{
		row = Range(0,this->getNumRow()-2);
		col = Range(0,this->getNumCol()-1);
		shift[0] = 1;
	}

	for(int r=row.start; r<= row.end; r++)
	{
		for(int c = col.start; c<=col.end; c++)
		{
			gd.at(r,c) = this->at(r,c)-this->at(r+shift[0],c+shift[1]);
		}
	}

	return gd;
}

template<typename T>
Data2D<T> Image2D<T>::dX() const
{
    return dI(COLUMN);
}

template<typename T>
Data2D<T> Image2D<T>::dY() const
{
    return dI(ROW);
}

template<typename T>
inline Range2D Image2D<T>::getROI() const
{
    return ROI;
}

template<typename T>
inline void Image2D<T>::setROI(const Range2D& _roi)
{
	ROI = _roi;
}

template<typename T>
inline void Image2D<T>::resetROI()
{
	ROI = Range2D::all();
}

