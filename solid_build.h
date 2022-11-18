#pragma once

#include "eular_op.h"
#include "sweep_op.h"

SOLID* cube_with_twoTriangularPrism(){
    //bottom square
    double p1[3]={-1.2,-1.2,0};
    double p2[3]={-1.2,1.2,0};
    double p3[3]={1.2,1.2,0};
    double p4[3]={1.2,-1.2,0};
    //triangle1
    double p5[3]={0,-1,0};
    double p6[3]={1,-1,0};
    double p7[3]={1,0,0};
    //triangle2
    double p8[3]={-1,0,0};
    double p9[3]={0,1,0};
    double p10[3]={-1,1,0};
    //build bottom square
    EULAROP *eular=new EULAROP;
    SOLID* solid=eular->mvsf(p1);
    LOOP* loop=solid->faces->outLoop;
    eular->mev(p2,loop,solid->vertexList[0]);
    eular->mev(p3,loop,solid->vertexList[1]);
    eular->mev(p4,loop,solid->vertexList[2]);
    FACE* squareFace=eular->mef(solid->vertexList[3],solid->vertexList[0],loop);

    loop=squareFace->outLoop;

    //build triangle1
    HALFEDGE* killedHe1=eular->mev(p5,loop,solid->vertexList[0]);
    eular->mev(p6,loop,solid->vertexList[4]);
    eular->mev(p7,loop,solid->vertexList[5]);
    FACE* triangle1=eular->mef(solid->vertexList[6],solid->vertexList[4],loop);
    eular->kemr(killedHe1,squareFace);
    //build triangle2
    HALFEDGE* killedHe2=eular->mev(p8,loop,solid->vertexList[1]);
    eular->mev(p9,loop,solid->vertexList[7]);
    eular->mev(p10,loop,solid->vertexList[8]);
    FACE* triangle2=eular->mef(solid->vertexList[9],solid->vertexList[7],loop);
    eular->kemr(killedHe2,squareFace);

    SWEEPOP* sweep=new SWEEPOP;
    double dir[3]={0,0,1.2};
    sweep->translate(dir,squareFace);
    sweep->translate(dir,triangle1);
    sweep->translate(dir,triangle2);
    eular->kfmrh(triangle1,squareFace);
    eular->kfmrh(triangle2,squareFace);
    return solid;
}
