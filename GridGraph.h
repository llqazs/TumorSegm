#ifndef GRIDGRAPH_H_INCLUDED
#define GRIDGRAPH_H_INCLUDED

//for smoothness term
//used to store pair-wise term
//the pair-wise term here is directed

struct Edge
{
    Edge(int _id1, int _id2, T _w12, T _w21) : id1(_id1),id2(_id2),w12(_w12),w21(_w21) { }
    node_id getFirstNode() const;
    node_id getSecondNode() const;
    T getFirstEdge() const;
    T getSecondEdge() const;
private:
    node_id id1, id2;
    T w12, w21;
};

//dimension size in terms of node
template<vector<size_t> DIMSIZE, vector<int> SHIFT>
class GridGraph
{
public:
    typedef Edge* iterator;
    typedef const Edge* const_iterator;

public:
    GridGraph(const BidirectEdge* weight, uint16 num_edge, vector<size_t> DIMSIZE, vector<int> SHIFT);
    virtual ~GridGraph();

    //public interface to be overridden by derived classes
    virtual iterator begin();
    virtual iterator end();
    virtual iterator next();

private:
    const BidirectEdge* p_weight;
    unsigned int n_edge;
    vector<size_t> s_dim;
    vector<int> d_node;
};

dimension, dim size, input is Edge* array,

template<typename DIMENSION_SIZE>
class GridGraph : public AbstractGridGraph
{
public:
      GridGraph();

      virtual iterator begin();
private:
    const BidirectEdge* p_cost;
    unsigned int n_edge;
    vector<size_t> s_dim;
    vector<int> d_node;
};

#endif // GRIDGRAPH_H_INCLUDED
