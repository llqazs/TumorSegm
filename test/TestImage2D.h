#pragma once

#include "../Image2D.h"
#include "common.h"

bool TestLoad()
{
	Image2D<int> img(21,12);
	cout<<img.getHeight()<<" "<<img.getWidth()<<endl;

	string filename = "test";
	img.save(filename);

	Image2D<int> img1;
	img1.load("test");

	return true;
}

bool TestDI()
{
   Image2D<int> img(2,2);
   for(int i=0;i<4;i++)
   {
	   img.at(i)  = i;
   }

   Data2D<int> dr = img.dI(ROW);
   dr.print();

   Data2D<int> dc = img.dI(COLUMN);
   dc.print();

   return true;
}

bool TestROI()
{
	Image2D<int> img(2,2);
	Range2D r = img.getROI();
	cout<<r.row.start<<" "<<r.row.end<<endl;
	cout<<r.col.start<<" "<<r.col.end<<endl;

	img.setROI(Range2D(0,1,0,1));
	r = img.getROI();
	cout<<r.row.start<<" "<<r.row.end<<endl;
	cout<<r.col.start<<" "<<r.col.end<<endl;

	img.resetROI();
	r = img.getROI();
	cout<<r.row.start<<" "<<r.row.end<<endl;
	cout<<r.col.start<<" "<<r.col.end<<endl;

	return true;
}

bool TestImage2D()
{
	TestLoad();
//	TestDI();
//	TestROI();
	return true;
}
