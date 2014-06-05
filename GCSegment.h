#ifndef GCSEGMENT_H_INCLUDED
#define GCSEGMENT_H_INCLUDED

class GCSegment
{
public:
    GCSegment();
    virtual ~GCSegment();
    virtual void run();
    virtual void load();

};

#endif // GCSEGMENT_H_INCLUDED
