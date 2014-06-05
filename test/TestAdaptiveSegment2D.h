#ifndef TESTADAPTIVESEGMENT2D_H_INCLUDED
#define TESTADAPTIVESEGMENT2D_H_INCLUDED

#include "../AdaptiveSegment2D.h"
#include "../Viewer.h"

bool TestAdaptiveSegment2D()
{
    Image2D<short> image;
    image.load("img");
    Data2D<LABEL> mask(image.getSize());
    mask.set(MyBasic::Range(14,45),MyBasic::Range(10,43),FOREGROUND);
    AdaptiveSegment2D seg(image,mask,false);

    seg.segment(2);
    Data2D<LABEL> label = seg.getLabeling();
    label.save("result");

    Viewer::show("result",label.getSize());
    return true;
}

#endif // TESTADAPTIVESEGMENT2D_H_INCLUDED
