#ifndef _SWEEP_OP_
#define _SWEEP_OP_

#include "halfedge.h"
#include "eular_op.h"

class SWEEPOP
{
public:
    void translate(double dir[3],FACE *face);
    void translate(FACE *face,double dir[3]);
private:
};

#endif // _SWEEP_OP_
