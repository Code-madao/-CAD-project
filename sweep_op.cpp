#include "sweep_op.h"

void SWEEPOP::translate(double dir[3],FACE *face){
    LOOP *loop=face->outLoop;
    HALFEDGE *he=loop->halfedge;
    VERTEX *firstVertex,*nextVertex,*firstBuiltV,*preBuiltVertex1,*preBuiltVertex2;
    firstVertex=he->beginVertex;
    nextVertex=he->endVertex;

    EULAROP* eular=new EULAROP;
    double coord[3];
    for(int i=0;i<3;i++)
        coord[i]=firstVertex->coord[i]+dir[i];
    HALFEDGE* temphe=eular->mev(coord,loop,firstVertex);
    preBuiltVertex1=temphe->endVertex;
    firstBuiltV=preBuiltVertex1;
    while(nextVertex!=firstVertex){
        for(int i=0;i<3;i++)
            coord[i]=nextVertex->coord[i]+dir[i];
        temphe=eular->mev(coord,loop,nextVertex);
        preBuiltVertex2=temphe->endVertex;
        eular->mef(preBuiltVertex1,preBuiltVertex2,loop);
        preBuiltVertex1=preBuiltVertex2;
        he=he->next;
        nextVertex=he->endVertex;
    }
    eular->mef(preBuiltVertex1,firstBuiltV,loop);
}

void SWEEPOP::translate(FACE *face,double dir[3]){
    translate(dir,face);
}
