#pragma once

#include "Data3D.h"

template<typename T>
class Image3D : public Data3D<T>
{
	void loadImage(const string& filename);
	void save(const string& filename);

private:
	int width;
	int height;
	int slice;
};