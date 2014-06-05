#ifndef NEIGHBOR4_H_INCLUDED
#define NEIGHBOR4_H_INCLUDED

class Neighbour4
{
	int offset[4][2];
	int indexmap[4];

    Neighbor4(void)
    {
        offset[0][0]=0; offest[0][1] = -1;
        offset[1][0]=-1; offest[1][1] = 0;
        offset[2][0]=0; offest[2][1] = 1;
        offset[3][0]=1; offest[3][1] = 0;

        indexmap[0]=2;
        indexmap[1]=3;
        indexmap[2]=0;
        indexmap[3]=1;
    }

	virtual ~Neighbour4(void)
	{
	}

	static Neighbour4& getInstance() {
		static Neighbour4 instance;
		return instance;
	}
public:

	static void at( int index, int& r, int& c )
	{
		if( index<0 || index>=4 ) {
			cout << "Index Error for 4 neigbours" << endl;
			system("pause");
			r=c= 0;
			return;
		}
		r = getInstance().offset[index][0];
		c = getInstance().offset[index][1];
	}

	static void getNeigbour( int index,
		const int& old_r,
		const int& old_c,
		int& r, int& c)
	{
		if( index<0 || index>=4 ) {
			cout << "Index Error for 4 neigbours" << endl;
			system("pause");
			r = old_r;
			c = old_c;
			return;
		}
		r = getInstance().offset[index][0] + old_r;
		c = getInstance().offset[index][1] + old_c;
	}

	static int indexToNeighbour(int index)
	{
        return getInstance().indexmap[index];
	}

};

#endif // NEIGHBOR4_H_INCLUDED
