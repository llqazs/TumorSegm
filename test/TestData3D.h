#pragma once

#include "../Data3D.h"
#include "../MyBasic.h"

using namespace MyBasic;

namespace TestData3D {
void TestConstructor()
{
	Data3D<int> d1(2,2,3);
	d1.print();
	if(d1.getNumData()!=12) 
	{
		cout<<"Invalid result in TestConstructor() !!!!!!!!!!!!"<<endl;
		return ;
	}

	Data3D<int> d2;
	d2.resize(1,2,3);
	d2.print();
	if(d2.getNumData()!=6) 
	{
		cout<<"Invalid result in TestConstructor() !!!!!!!!!!!!"<<endl;
		return ;
	}

	cout<<"Valid result in TestConstructor() "<<endl;

}


void TestAccess()
{
	Data3D<int> d1(2,2,3);
	d1.at(0,0,0)=1; 
	d1.at(Index3D(0,0,1))=2;
	d1.at(2)=3;
	d1.set(Range3D(0,0,1,1,0,2),4);
	d1.set(Range(1,1),Range(0,1),Range(0,2),5);
	d1.print();
	if(d1.at(0)!=1 || d1.at(0,0,1)!=2 || d1.at(Index3D(0,0,2))!=3 || d1.at(3)!=4 || d1.at(6)!=5)
	{
		cout<<"Invalid result in TestAccess() !!!!!!!!!!!!"<<endl;
		return ;
	}

	cout<<"Valid result in TestAccess() "<<endl;
}

void TestInRange()
{
	Data3D<int> d(2,2,3);
	cout<<d.inRange(2,2,0)<<" "<<d.inRange(1,1,1)<<" "<<d.inRange(1)<<" "<<d.inRange(Index3D(0,1,1))<<endl;
}

void TestInfo()
{
	Data3D<int> d(2,2,3);
	for(int i=0;i<12;i++) d.at(i) = i;
	cout<<"size: "<<d.getNumData()<<" capacity: "<<d.getCapacity()<<" min: "<<d.getMin()<<" max: "<<d.getMax()<<endl;
	cout<<"num slice: "<<d.getNumSli()<<" num row: "<<d.getNumRow()<<" num col: "<<d.getNumCol()<<endl;

	Data3D<int> d2;
	cout<<"isEmpty: "<<d2.isEmpty()<<endl;
	cout<<"size: "<<d2.getNumData()<<" capacity: "<<d2.getCapacity()<<endl;
	cout<<"num slice: "<<d2.getNumSli()<<" num row: "<<d2.getNumRow()<<" num col: "<<d2.getNumCol()<<endl;
	d2.copyFrom(d);
	cout<<"isEmpty: "<<d2.isEmpty()<<endl;
	cout<<"size: "<<d2.getNumData()<<" capacity: "<<d2.getCapacity()<<endl;
	cout<<"num slice: "<<d2.getNumSli()<<" num row: "<<d2.getNumRow()<<" num col: "<<d2.getNumCol()<<endl;
	cout<<"isEqual: "<<d2.isEqualTo(d)<<endl;


}

void TestBlockValueAssignment()
{
	Data3D<int> d1(2,2,3);
	d1.set(Range3D(0,0,1,1,0,2),4);

	Data3D<bool> f(2,2,3);
	f.setAll(false);
	f.set(Range(1,1),Range(0,1),Range(0,2),true);
	d1.set(f,3);

	d1.print();
}

void TestCopyFrom()
{
	Data3D<int> d1(2,4,3);
	d1.setAll(-1);
	d1.set(Range(0,0),Range(0,1),Range(1,2),1);
	d1.set(Range3D(Range(0,0),Range(2,3),Range(1,2)),2);
	d1.print();

	//copy whole matrix
	Data3D<int> d2;
	d2.copyFrom(d1);
	d2.print();

	//copy with mask
	Data3D<bool> f(2,4,3);
	f.setAll(false);
	f.set(Range(0,0),Range(0,3),Range(0,0),true);
	f.print();

	d2.setAll(0);
	d2.copyFrom(d1,f);
	d2.print();

	//copy to different type with Range
	d1.set(Range(0,0),Range(0,3),Range(0,0),0);
	f.copyFrom(d1,Range3D(Range(0,0),Range(0,3),Range(0,1)),Range3D(Range(0,0),Range(0,3),Range(1,2)));
	f.print();
}


void TestData3D()
{
	//constructors
//	TestConstructor();

	//get and set method
//	TestAccess();

	//test inRange
//	TestInRange();

	//test info
//	TestInfo();

	//block value assignment
//	TestBlockValueAssignment();

	//copy to
	TestCopyFrom();


	//resize


}

}