#pragma once

#include <string.h>

typedef unsigned int uint32;
typedef int node_id;
typedef unsigned short uint16;
typedef unsigned char uchar;


enum DIRECTION
{
	ROW=0,COLUMN,SLICE
};

enum LABEL
{
    BACKGROUND=0,FOREGROUND,UNKNOWN
};

    struct Config
    {
        Config& operator =(const Config& cfg) { memcpy(this,&cfg,sizeof(Config)); return *this; }
        bool starshape;
        bool dataterm;
        bool hardConstraint;
        bool smoothterm;
        bool white_to_dark;
        int bin_num;
        int min_area;
        int max_area;
        int seed_radius;
    };


