#pragma once

#include "../Data2D.h"
#include "../MyBasic.h"

using namespace MyBasic;

namespace TestData2D {

void TestConstructor()
{
	Data2D<int> d1(2,2);
	d1.print();
	if(d1.getNumData()!=4)
	{
		cout<<"Invalid result in TestConstructor() !!!!!!!!!!!!"<<endl;
		return ;
	}

	Data2D<int> d2;
	d2.resize(1,2);
	d2.print();
	if(d2.getNumData()!=2)
	{
		cout<<"Invalid result in TestConstructor() !!!!!!!!!!!!"<<endl;
		return ;
	}

	cout<<"Valid result in TestConstructor() "<<endl;

}

void TestAccess()
{
	Data2D<int> d1(2,2);
	d1.at(0,0)=1; d1.at(Index2D(0,1))=2;
	d1.at(2)=3; d1.set(Range2D(Range(1,1),Range(1,1)),4);
	d1.print();
	if(d1.at(0)!=1 || d1.at(0,1)!=2 || d1.at(Index2D(1,0))!=3 || d1.at(3)!=4)
	{
		cout<<"Invalid result in TestAccess() !!!!!!!!!!!!"<<endl;
		return ;
	}

	cout<<"Valid result in TestAccess() "<<endl;
}

void TestInRange()
{
	Data2D<int> d(2,2);
	cout<<d.inRange(2,2)<<" "<<d.inRange(1,1)<<" "<<d.inRange(1)<<" "<<d.inRange(Index2D(0,1))<<endl;
}

void TestInfo()
{
	Data2D<int> d(3,2);
	for(int i=0;i<6;i++) d.at(i) = i;
	cout<<"size: "<<d.getNumData()<<" capacity: "<<d.getCapacity()<<" min: "<<d.getMin()<<" max: "<<d.getMax()<<endl;
	cout<<"num row: "<<d.getNumRow()<<" num col: "<<d.getNumCol()<<endl;

	Data2D<int> d2;
	cout<<"isEmpty: "<<d2.isEmpty()<<endl;
	cout<<"size: "<<d2.getNumData()<<" capacity: "<<d2.getCapacity()<<endl;
	cout<<"num row: "<<d2.getNumRow()<<" num col: "<<d2.getNumCol()<<endl;
	d2.copyFrom(d);
	cout<<"isEmpty: "<<d2.isEmpty()<<endl;
	cout<<"size: "<<d2.getNumData()<<" capacity: "<<d2.getCapacity()<<endl;
	cout<<"num row: "<<d2.getNumRow()<<" num col: "<<d2.getNumCol()<<endl;
	cout<<"isEqual: "<<d2.isEqualTo(d)<<endl;


}

void TestBlockValueAssignment()
{
	Data2D<int> d(4,3);
	d.set(Range(0,1),Range(1,2),1);
	d.set(Range2D(Range(2,3),Range(1,2)),2);

	Data2D<bool> f(4,3);
	f.setAll(false);
	f.set(Range(0,3),Range(0,0),true);
	d.set(f,3);

	d.print();
}

void TestCopyFrom()
{
	Data2D<int> d1(4,3);
	d1.setAll(-1);
	d1.set(Range(0,1),Range(1,2),1);
	d1.set(Range2D(Range(2,3),Range(1,2)),2);
	d1.print();

	//copy whole matrix
	Data2D<int> d2;
	d2.copyFrom(d1);
	d2.print();

	//copy with mask
	Data2D<bool> f(4,3);
	f.setAll(false);
	f.set(Range(0,3),Range(0,0),true);
	f.print();

	d2.setAll(0);
	d2.copyFrom(d1,f);
	d2.print();

	//copy to different type with Range
	d1.set(Range(0,3),Range(0,0),0);
	f.copyFrom(d1,Range2D(Range(0,3),Range(0,1)),Range2D(Range(0,3),Range(1,2)));
	f.print();
}


void TestData2D()
{
	//constructors
//	TestConstructor();

	//get and set method
//	TestAccess();

	//test inRange
	TestInRange();

	//test info
	TestInfo();

	//block value assignment
	TestBlockValueAssignment();

	//copy to
	TestCopyFrom();


	//resize


}

}
