#pragma once

#include<iostream>
#include "../MyBasic.h"

using namespace MyBasic;
using namespace std;

void TestSize2D()
{
	Size2D s1;
	cout<<s1.nRow<<" "<<s1.nCol<<endl;

	s1.set(1,1);
	cout<<s1.nRow<<" "<<s1.nCol<<endl;

	Size2D s2;
	s2.set(s1);
	cout<<s1.nRow<<" "<<s1.nCol<<endl;
	cout<<"isEqual "<<Size2D::isEqual(s1,s2)<<endl;


}

void TestSize3D()
{
	Size3D s1;
	cout<<s1.nSli<<" "<<s1.nRow<<" "<<s1.nCol<<endl;

	s1.set(1,1,1);
	cout<<s1.nSli<<" "<<s1.nRow<<" "<<s1.nCol<<endl;

	Size3D s2;
	s2.set(s1);
	cout<<s1.nSli<<" "<<s1.nRow<<" "<<s1.nCol<<endl;
	cout<<"isEqual "<<Size3D::isEqual(s1,s2)<<endl;


}

void TestIndex2D()
{
	Index2D s1;
	cout<<s1.row<<" "<<s1.col<<endl;

	s1.set(1,1);
	cout<<s1.row<<" "<<s1.col<<endl;

	Index2D s2;
	s2.set(s1);
	cout<<s1.row<<" "<<s1.col<<endl;
}

void TestIndex3D()
{
	Index3D s1;
	cout<<s1.sli<<" "<<s1.row<<" "<<s1.col<<endl;

	s1.set(1,1,1);
	cout<<s1.sli<<" "<<s1.row<<" "<<s1.col<<endl;

	Index3D s2;
	s2.set(s1);
	cout<<s1.sli<<" "<<s1.row<<" "<<s1.col<<endl;
}

void TestRange()
{
	Range r1(0,4);
	cout<<"size: "<<r1.getSize()<<" isEmpty: "<<r1.isEmpty()<<endl;

	Range r2;
	r2 = r1;
	cout<<r2.start<<" "<<r2.end<<endl;
	cout<<"isEqual: "<<Range::isEqual(r1,r2)<<endl;


}

void TestMyBasic()
{
	TestSize2D();

//	TestSize3D();

//	TestIndex2D();

//	TestIndex3D();

//	TestRange();
}
