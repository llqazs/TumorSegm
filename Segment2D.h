#ifndef SEGMENT2D_H_INCLUDED
#define SEGMENT2D_H_INCLUDED

class Segment2D
{
public:
    void segment();
    Data2D<LABEL> getLabel() const;
    void setSmoothCost(Data2D<BidirectEdge<double> >&)
private:
    Image2D<uint16>* pImg;


};

#endif // SEGMENT2D_H_INCLUDED


void Segment2D::segment()
{
    //build the graph
    g = new Graph2D<double>(max_node, max_edge);

    if(cfg.dataterm)
    {
        addDataCost();
    }

    if(cfg.smoothterm)
    {
        addSmoothCost();
    }

    if(cfg.hardconstraint)
    {
        addHardConstraint();
    }

    if(cfg.starshape)
    {
        addStarshape();
    }

    double flow = g.maxflow();
}

void Segment2D::addDataCost()
{
    if(fgcost.empty()||bkcost.empty()) computeDataCost();

    g->setDataCost(fgcost,FOREGROUND);
    g->setDataCost(bkcost,BACKGROUND);
}

void Segment2D::addSmoothCost()
{
    if(smoothcostR.empty() || smoothcostC.empty()) computeSmoothCost();

    g->setSmoothCost(smoothcostR,ROW);
    g->setSmoothCost(smoothcostC,COLUMN);
}

void Segment2D::addHardConstraint()
{
    if(hdvec.empty()) computeHardConstraint();

    g->setDataCost(hdvec);
}

void Segment2D::addStarshape()
{
    if(starLink.empty()) computeStarshape();

    g->setSmoothCost(starLink);
}

inline void Segment2D::setSmoothCost(const Data2D<double>* pSm, DIRECTION dir)
{
    switch(dir)
    {
    case ROW:
        pSmoothCostR = pSm;
        break;
    case COLUMN:
        pSmoothCostC = pSm;
        break;
    }
}

void Segment2D::computeDataCostMatrix()
{
    //compute data cost from mask
}

void Segment2D::computeSmoothCostMatrix()
{
    //compute smooth cost for seg_region
}

void Segment2D::computeHardConstraintArray()
{

}

void Segment2D::computeStarShapeArray()
{

}

void Segment2D::hardConstraintBorder()
{

}

void Segment2D::hardConstraintSeeds()
{

}
