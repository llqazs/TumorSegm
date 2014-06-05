#ifndef REGIONPROPS_H_INCLUDED
#define REGIONPROPS_H_INCLUDED

namespace lliu
{
    template<typename T>
    double area(const Data2D<T>& region)
    {
        int nRow = region.getNumRow();
        int nCol = region.getNumCol();

        double result=0;
        for(int r=0;r<nRow; r++)
        {
            for(int c=0;c<nCol;c++)
            {
                if(region.at(r,c)!=0) result +=1;
            }
        }

        return result;
    }


}

#endif // REGIONPROPS_H_INCLUDED
