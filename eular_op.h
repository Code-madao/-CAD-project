#ifndef _EULAR_OP_
#define _EULAR_OP_

#include "halfedge.h"
#include<vector>
using namespace std;

class EULAROP
{
public:
    SOLID* mvsf(double point[3]);
    HALFEDGE* mev(double point[3],LOOP* loop,VERTEX* beginVertex);
    HALFEDGE* mev(VERTEX* beginVertex,VERTEX* endVertex,LOOP* loop);
    FACE* mef(VERTEX* beginVertex,VERTEX* endVertex,LOOP* loop);
    LOOP* kemr(HALFEDGE* he,FACE* outerFace);
    void kfmrh(FACE* innerFace,FACE* outerFace);

    EULAROP();
private:
    vector<LOOP*> loopList;
    vector<FACE*> faceList;
};


#endif // _EULAR_OP_
