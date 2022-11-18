#include "eular_op.h"

EULAROP::EULAROP(){

}

SOLID* EULAROP::mvsf(double point[3]){
    SOLID* solid=new SOLID();
    FACE* face=new FACE();
    LOOP* loop=new LOOP();
    VERTEX* vertex=new VERTEX(point);

    solid->faces=face;
    face->solid=solid;
    face->outLoop=loop;
    loop->face=face;

    face->id=solid->faceCount++;
    loop->id=solid->loopCount++;
    vertex->id=solid->vertexCount++;
    solid->vertexList.push_back(vertex);
//    loopList.push_back(loop);
//    faceList.push_back(face);

    return solid;
}

HALFEDGE* EULAROP::mev(double point[3],LOOP* loop,VERTEX* beginVertex){
    SOLID* solid=loop->face->solid;
    EDGE* edge=new EDGE();
    HALFEDGE* left_he=new HALFEDGE();
    HALFEDGE* right_he=new HALFEDGE();
    VERTEX* endVertex=new VERTEX(point);

    endVertex->id=solid->vertexCount++;
    solid->vertexList.push_back(endVertex);
    left_he->beginVertex=beginVertex;
    left_he->endVertex=endVertex;
    left_he->edge=edge;
    left_he->pair_he=right_he;
    left_he->loop=loop;
    right_he->beginVertex=endVertex;
    right_he->endVertex=beginVertex;
    right_he->edge=edge;
    right_he->pair_he=left_he;
    right_he->loop=loop;
    edge->left_he=left_he;
    edge->right_he=right_he;
    //add new halfedges into loop
    if(loop->halfedge==NULL){
        left_he->next=right_he;
//        left_he->pre=right_he;
        right_he->next=left_he;
//        right_he->pre=left_he;

        loop->halfedge=left_he;
    }
    else{
        HALFEDGE* temphe=loop->halfedge;
        while(temphe->endVertex!=beginVertex)temphe=temphe->next;
        left_he->next=right_he;
//        left_he->pre=temphe;
        right_he->next=temphe->next;
//        right_he->pre=left_he;
//        temphe->next->pre=right_he;
        temphe->next=left_he;
    }

    //add edge into solid
//    EDGE* tempe=solid->edges;
//    if(!tempe)solid->edges=edge;
//    else{
//        while(tempe->next)tempe=tempe->next;
//        tempe->next=edge;
////        edge->pre=tempe;
//    }

    return left_he;
}
HALFEDGE* EULAROP::mev(VERTEX* beginVertex,VERTEX* endVertex,LOOP* loop){
    return mev(endVertex->coord,loop,beginVertex);
}

FACE* EULAROP::mef(VERTEX* beginVertex,VERTEX* endVertex,LOOP* loop){
    SOLID* solid=loop->face->solid;
    FACE* face=new FACE();
    EDGE* edge=new EDGE();
    HALFEDGE* left_he=new HALFEDGE();
    HALFEDGE* right_he=new HALFEDGE();
    LOOP* newLoop=new LOOP();

    left_he->beginVertex=beginVertex;
    left_he->endVertex=endVertex;
    left_he->pair_he=right_he;
    left_he->edge=edge;
    right_he->beginVertex=endVertex;
    right_he->endVertex=beginVertex;
    right_he->pair_he=left_he;
    right_he->edge=edge;
    edge->left_he=left_he;
    edge->right_he=right_he;

    HALFEDGE* temphe=loop->halfedge;
    HALFEDGE *temphe1,*temphe2;
    while(temphe->endVertex!=beginVertex)temphe=temphe->next;
    temphe1=temphe;
    while(temphe->endVertex!=endVertex)temphe=temphe->next;
    temphe2=temphe;

    //divide loop into two loops
    right_he->next=temphe1->next;
//    temphe1->next->pre=right_he;
    temphe1->next=left_he;
//    left_he->pre=temphe1;
    left_he->next=temphe2->next;
//    temphe2->next->pre=left_he;
    temphe2->next=right_he;
//    right_he->pre=temphe2;

    //update information
    newLoop->halfedge=right_he;
    loop->halfedge=left_he;
    left_he->loop=loop;
    right_he->loop=newLoop;

    HALFEDGE* temp=right_he->next;
    while(temp!=right_he){
        temp->loop=newLoop;
        temp=temp->next;
    }

    newLoop->id=solid->loopCount++;
    loopList.push_back(newLoop);

    //add face into solid
    face->next=solid->faces;
    solid->faces->pre=face;
    solid->faces=face;
    face->solid=solid;
    face->id=solid->faceCount++;
    faceList.push_back(face);

    //add loop into face
    newLoop->face=face;
    face->outLoop=newLoop;

    //add edge into solid
    EDGE* tempe=solid->edges;
    if(!tempe)solid->edges=edge;
    else{
        while(tempe->next)tempe=tempe->next;
        tempe->next=edge;
        edge->pre=tempe;
    }

    return face;
}

LOOP* EULAROP::kemr(HALFEDGE* left_he,FACE* outerFace){
    SOLID *solid=outerFace->solid;
    LOOP* innerLoop=new LOOP();
    HALFEDGE *temphe1,*temphe2;
    temphe1=left_he;
    temphe2=left_he->pair_he;

    HALFEDGE* first=left_he->next;
    HALFEDGE* last=left_he->next;
    while(last->next!=temphe2)last=last->next;
    last->next=first;
    first->loop->halfedge=first;
    innerLoop->halfedge=first;

    first=temphe2->next;
    last=temphe2->next;
    while(last->next!=temphe1)last=last->next;
    last->next=first;
    left_he->loop->halfedge=first;

    //update he's loop info
    HALFEDGE* temp=innerLoop->halfedge;
    while(temp!=innerLoop->halfedge){
        temp->loop=innerLoop;
        temp=temp->next;
    }

    //add loop into face
    innerLoop->id=solid->loopCount++;
    loopList.push_back(innerLoop);
    innerLoop->face=outerFace;
    outerFace->innerLoopCount++;
    innerLoop->next=outerFace->innerLoop;
    outerFace->innerLoop=innerLoop;


    delete temphe1->edge;
    delete temphe1;
    delete temphe2;

    return innerLoop;
}

void EULAROP::kfmrh(FACE* innerFace,FACE* outerFace){
    SOLID* solid=innerFace->solid;
    LOOP* innerLoop=innerFace->outLoop;

    //make loop be outerface's innerloop
    innerLoop->face=outerFace;
    outerFace->innerLoopCount++;
    innerLoop->next=outerFace->innerLoop;
    outerFace->innerLoop=innerLoop;

    if(innerFace->pre){
        innerFace->pre->next=innerFace->next;
        if(innerFace->next)
            innerFace->next->pre=innerFace->pre;
    }
    else{
        solid->faces=innerFace->next;
        innerFace->next->pre=NULL;
    }

    delete innerFace;
    solid->faceCount--;
    solid->loopCount--;
}
