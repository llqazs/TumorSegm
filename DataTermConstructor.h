#ifndef DATATERMCONSTRUCTOR_H_INCLUDED
#define DATATERMCONSTRUCTOR_H_INCLUDED

class DataTermConstructor
{
public:
    template<typename T>
    static void computeDataCost(const Image2D<short>& img, const Data2D<uint8>& mask);
};

#endif // DATATERMCONSTRUCTOR_H_INCLUDED
