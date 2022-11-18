//#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <GL/glut.h>
#include "halfedge.h"
#include "eular_op.h"
#include "sweep_op.h"
#include "solid_build.h"
#include "draw.h"

int main(int argc,char *argv[]){
    SOLID* example=cube_with_twoTriangularPrism();

//    example->print();

    Draw(argc,argv,example);

    return 0;
}
