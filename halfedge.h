#ifndef _HALFEDGE_
#define _HALFEDGE_

#include<iostream>
#include<vector>
using namespace std;

struct SOLID;
struct FACE;
struct LOOP;
struct EDGE;
struct HALFEDGE;
struct VERTEX;

struct Point{
    int x,y,z;
    Point(double x,double y,double z){
        this->x=x;
        this->y=y;
        this->z=z;
    }
};

struct VERTEX{
    int id;
    double coord[3];
    VERTEX *pre,*next;

    void printCoord(){
        printf("[%.1lf,%.1lf,%.1lf]",coord[0],coord[1],coord[2]);
    }
    VERTEX(double _coord[3]):id(-1),pre(NULL),next(NULL){
        coord[0]=_coord[0];
        coord[1]=_coord[1];
        coord[2]=_coord[2];
    }
    VERTEX(Point* p){
        coord[0]=p->x;
        coord[1]=p->y;
        coord[2]=p->z;
    }
};

struct HALFEDGE{
    HALFEDGE *pre,*next,*pair_he;
    LOOP *loop;
    VERTEX *beginVertex,*endVertex;
    EDGE *edge;

    void print(){
        printf("beginV:");
        beginVertex->printCoord();
        printf(" endV:");
        endVertex->printCoord();
        printf("\n");
    }
    HALFEDGE():pre(NULL),next(NULL),pair_he(NULL),loop(NULL),beginVertex(NULL),endVertex(NULL),edge(NULL){}
};

struct LOOP{
    int id;

    LOOP *pre,*next;
    FACE *face;
    HALFEDGE *halfedge;

    void print(){
        printf("loop id:%d\n",id);
        HALFEDGE* temphe=halfedge;
        while(temphe){
            temphe->print();
            temphe=temphe->next;
        }
        printf("loop end\n");
    }
    LOOP():id(0),pre(NULL),next(NULL),face(NULL),halfedge(NULL){}
};

struct FACE{
    int id;
    int innerLoopCount;

    SOLID *solid;
    LOOP *outLoop,*innerLoop;
    FACE *pre,*next;

    void print(){
        printf("face id:%d\n",id);
        printf("outerloop:");
        outLoop->print();
        LOOP* tempLoop=innerLoop;
        printf("innerloop:");
        while(tempLoop){
            tempLoop->print();
            tempLoop=tempLoop->next;
        }
    }
    FACE():id(0),innerLoopCount(0),solid(NULL),outLoop(NULL),innerLoop(NULL),pre(NULL),next(NULL){}
};

struct SOLID{
    int id;
    int vertexCount;
    int loopCount;
    int faceCount;

    SOLID *pre,*next;
    FACE *faces;//first face
    EDGE *edges;//first edge
    vector<VERTEX*> vertexList;

    void print(){
        FACE* face=faces;
        while(face){
            face->print();
            face=face->next;
        }
    }
    SOLID():id(0),vertexCount(0),loopCount(0),faceCount(0),pre(NULL),next(NULL),faces(NULL),edges(NULL){}
};

struct EDGE{
    HALFEDGE *left_he,*right_he;//left_he:the halfedge has same direction with edge
    EDGE *pre,*next;

    EDGE():left_he(NULL),right_he(NULL),pre(NULL),next(NULL){}
};



#endif // _HALFEDGE_
