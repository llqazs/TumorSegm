#ifndef VECTOPERATION_H_INCLUDED
#define VECTOPERATION_H_INCLUDED

#include <vector>
#include <numeric>

using namespace std;

//operations for vector
template<typename T>
vector<T> operator+(const vector<T>& a, const T& scalar)
{
    vector<T> result(a.size());
    for(unsigned int i=0; i<a.size();i++)
    {
        result[i] = a[i] + scalar;
    }
    return result;
}

template<typename T>
vector<T> operator-(const vector<T>& a, const T& scalar)
{
    vector<T> result(a.size());
    for(unsigned int i=0; i<a.size();i++)
    {
        result[i] = a[i] - scalar;
    }
    return result;
}

template<typename T>
vector<T> operator*(const vector<T>& a, const T& scalar)
{
    vector<T> result(a.size());
    for(unsigned int i=0; i<a.size();i++)
    {
        result[i] = a[i]*scalar;
    }
    return result;
}

    template<typename T>
    vector<T> operator/(const vector<T>& a, T dividor)
    {
        assert(dividor!=0);
        vector<T> result(a);
        for(int i=0;i<result.size();i++)
        {
            result[i] = result[i]/dividor;
        }

        return result;
    }

template<typename T>
T sum(const vector<T>& a)
{
    return std::accumulate(a.begin(),a.end(),0);
}

template<typename T>
void divide(vector<T>& data, T dividor)
{
    assert(dividor!=0);
    for(int i=0;i<data.size();i++)
    {
        data[i] = data[i]/dividor;
    }
}

#endif // VECTOPERATION_H_INCLUDED
