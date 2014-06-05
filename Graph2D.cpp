#include "Graph2D.h"

//implementation of non-inline functions of class Graph2D
Graph2D::Graph2D(int _row, int _col):Graph<double,double,double>(_row*_col,8*_row*_col),nRow(_row),nCol(_col)
{
    Graph<double,double,double>::add_node(_row*_col);
    done = false;
}

Graph2D::~Graph2D()
{

}

//add smooth cost with direction
void Graph2D::addSmoothCost(const Data2D<double>& cap, const Data2D<double>& rev_cap, DIRECTION d)
{
    switch(d)
    {
    case ROW:
        this->addSmoothCost(cap,rev_cap,MyBasic::Index2D(0,1));
        break;
    case COLUMN:
        this->addSmoothCost(cap,rev_cap,MyBasic::Index2D(1,0));
        break;
    case UNKNOWN:
        break;
    }
}


//add regular smooth cost
void Graph2D::addSmoothCost(const Data2D<double>& cap,const Data2D<double>& rev_cap, const Index2D& shift)
{
    Range2D roi(0,nRow-1-shift.row, 0, nCol-1-shift.col);

    for(int r=roi.row.start; r<=roi.row.end; r++)
    {
        for(int c=roi.col.start; c<=roi.col.end; c++)
        {
            MyBasic::Index2D point1(r,c);
            MyBasic::Index2D point2 = point1 + shift;

            add_edge(point1,point2,cap.at(point1),rev_cap.at(point2));
        }
    }
}

//add sparse smooth cost
void Graph2D::addSmoothCost(const vector<MyBasic::Index2D>& point1, const vector<MyBasic::Index2D>& point2,const vector<double>& cap,const vector<double>& rev_cap)
{
    int num = point1.size();
    for(int i=0;i<num;i++)
    {
        add_edge(point1[i], point2[i], cap[i], rev_cap[i]);
    }
}

//add sparse data cost
void Graph2D::addDataCost(const vector<MyBasic::Index2D>& point,const vector<double>& fgcost, const vector<double>& bkcost)
{
    int num = point.size();
    for(int i=0;i<num;i++)
    {
        add_tweights(point[i], fgcost[i],bkcost[i]);
    }
}

//add regular(grid) data cost
void Graph2D::addDataCost(const Data2D<double>& datacost, LABEL label)
{
    for(int r=0;r<nRow;r++)
    {
        for(int c=0;c<nCol;c++)
        {
            MyBasic::Index2D point(r,c);
            switch(label)
            {
            case FOREGROUND:
                add_tweights(point,datacost.at(point),0);
                break;
            case BACKGROUND:
                add_tweights(point,0,datacost.at(point));
                break;
            case UNKNOWN:
                break;
            }
        }
    }
}


Data2D<LABEL> Graph2D::get_labeling()// const
{
    if(!done) return Data2D<LABEL>();
	Data2D<LABEL> label(nRow,nCol);
	for(int r=0;r<nRow;r++)
	{
		for(int c=0;c<nCol;c++)
		{
		    label.at(r,c) = what_segment(Index2D(r,c));
		}
	}
	return label;
}

LABEL Graph2D::what_segment(const Index2D& i)// const
{
    if(Graph<double,double,double>::what_segment(Index2Id(i))==Graph<double,double,double>::SOURCE) return FOREGROUND;
    else return BACKGROUND;
}



double Graph2D::maxflow()
{
    flow = Graph<double,double,double>::maxflow();
    done = true;
    return flow;
}
