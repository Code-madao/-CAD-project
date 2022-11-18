#pragma once
#define CALLBACK __stdcall
#include "halfedge.h"
#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <stdlib.h>
using namespace std;

//键盘一次wasd改变视角angle度
GLfloat angle = 5.0f * 3.14159 / 180;
GLfloat sinA = sin(angle);
GLfloat cosA = cos(angle);
GLfloat xDirection = 0.0f;
GLfloat yDirection = 0.0f;
GLfloat zDirection = 10.0f;
GLfloat upDirection = 1.0f;
GLfloat mini = 0.000001f;

SOLID* solid =NULL;

void InitEnvironment()
{
	glEnable(GL_DEPTH);
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4f(0, 0.5, 0, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1, 0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(xDirection, yDirection, zDirection, 0, 0, 0, 0, 1, 0);
}

void KeyBoards(unsigned char key, int x, int y)
{
	GLfloat oldX = xDirection;
	GLfloat oldY = yDirection;
	GLfloat oldZ = zDirection;
	//视点到原点距离的平方
	GLfloat dist2 = oldX * oldX + oldY * oldY + oldZ * oldZ;
	GLfloat maxY = sqrtf(0.99 * dist2);

	switch (key)
	{
	case 'w':
	{
		//newDirection记录根号下的x2+z2，避免重复计算；这里将根号下的x2+z2作为一个新方向，与y方向构成二维平面；键盘w沿着该新方向的经线向北极前进，s则向南极前进。
		GLfloat newDirection = sqrtf(oldX * oldX + oldZ * oldZ);
		yDirection = newDirection * sinA + oldY * cosA;
		//中间变量tmp避免重复计算
		GLfloat tmp;
		//南北极点是该参数曲面的奇异点，穿越极点的视角绘制比较麻烦，故这里设置了maxY = sqrtf(0.99*dist2) < dist2，即视野不能逾越极点
		if (oldY + mini >= maxY)
		{
			yDirection = maxY + mini;
			tmp = 0.01 * dist2;
		}
		else
			tmp = newDirection * newDirection * cosA * cosA + oldY * oldY * sinA * sinA - 2 * newDirection * cosA * oldY * sinA;

		if (abs(oldZ) < mini)
			xDirection = sqrtf(tmp) * abs(oldX) / oldX;
		else if (abs(oldX) < mini)
			zDirection = sqrtf(tmp) * abs(oldZ) / oldZ;
		else
		{
			xDirection = sqrtf(tmp / (1 + oldZ * oldZ / oldX / oldX)) * abs(oldX) / oldX;
			zDirection = sqrtf(tmp / (1 + oldX * oldX / oldZ / oldZ)) * abs(oldZ) / oldZ;
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(xDirection, yDirection, zDirection, 0, 0, 0, 0, 1, 0);
		glutPostRedisplay();
		break;
	}
	case 'a':
		xDirection = -oldZ * sinA + oldX * cosA;
		zDirection = oldZ * cosA + oldX * sinA;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(xDirection, yDirection, zDirection, 0, 0, 0, 0, 1, 0);
		glutPostRedisplay();
		break;
	case 's':
	{
		GLfloat newDirection = sqrtf(oldX * oldX + oldZ * oldZ);
		yDirection = -newDirection * sinA + oldY * cosA;
		GLfloat tmp;
		if (oldY - mini <= -maxY)
		{
			yDirection = -maxY - mini;
			tmp = 0.01 * dist2;
		}
		else
			tmp = newDirection * newDirection * cosA * cosA + oldY * oldY * sinA * sinA + 2 * newDirection * cosA * oldY * sinA;

		if (abs(oldZ) < mini)
			xDirection = sqrtf(tmp) * abs(oldX) / oldX;
		else if (abs(oldX) < mini)
			zDirection = sqrtf(tmp) * abs(oldZ) / oldZ;
		else
		{
			xDirection = sqrtf(tmp / (1 + oldZ * oldZ / oldX / oldX)) * abs(oldX) / oldX;
			zDirection = sqrtf(tmp / (1 + oldX * oldX / oldZ / oldZ)) * abs(oldZ) / oldZ;
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(xDirection, yDirection, zDirection, 0, 0, 0, 0, 1, 0);
		glutPostRedisplay();
		break;
	}
	case 'd':
		xDirection = oldZ * sinA + oldX * cosA;
		zDirection = oldZ * cosA - oldX * sinA;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(xDirection, yDirection, zDirection, 0, 0, 0, 0, 1, 0);
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	}
}

void CALLBACK tessBeginCB(GLenum which){glBegin(which);}
void CALLBACK tessEndCB(){glEnd();}
void CALLBACK tessVertexCB(const GLvoid* data)
{
	const GLdouble* ptr = (const GLdouble*)data;
	glVertex3dv(ptr);
}
void CALLBACK tessErrorCB(GLenum errorCode)
{
	const GLubyte* errorStr;
	errorStr = gluErrorString(errorCode);
}

void DrawLoop(GLUtesselator* tess, LOOP* loop)
{
	GLdouble* pos;
	gluTessBeginContour(tess);
	HALFEDGE* he = loop->halfedge;
	VERTEX* firstVertex = he->beginVertex;
	pos = new GLdouble[3]{he->beginVertex->coord[0],he->beginVertex->coord[1],he->beginVertex->coord[2]};
	gluTessVertex(tess, pos, pos);
	printf("Drawing pos = %.2lf,%.2lf,%.2lf\n", pos[0], pos[1], pos[2]);
	while(he->endVertex!=firstVertex){
		he = he->next;
		pos = new GLdouble[3]{he->beginVertex->coord[0],he->beginVertex->coord[1],he->beginVertex->coord[2]};
		gluTessVertex(tess, pos, pos);
		printf("Drawing pos = %.2lf,%.2lf,%.2lf\n", pos[0], pos[1], pos[2]);
	}
	gluTessEndContour(tess);
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	GLUtesselator* tess = gluNewTess();
	gluTessCallback(tess, GLU_TESS_BEGIN, (void (CALLBACK*)())tessBeginCB);
	gluTessCallback(tess, GLU_TESS_END, (void (CALLBACK*)())tessEndCB);
	gluTessCallback(tess, GLU_TESS_VERTEX, (void (CALLBACK*)())tessVertexCB);
	gluTessCallback(tess, GLU_TESS_ERROR, (void (CALLBACK*)())tessErrorCB);
	FACE* face = solid->faces;
    while (face)
    {
        gluTessBeginPolygon(tess, 0);
        LOOP* loop = face->outLoop;
        DrawLoop(tess, loop);
        loop = face->innerLoop;
        while(loop){
            DrawLoop(tess, loop);
            loop = loop->next;
        }
        gluTessEndPolygon(tess);
        face = face->next;
    }
	gluDeleteTess(tess);
	glFlush();
	glutSwapBuffers();
}

void Draw(int argc, char** argv, SOLID* targetSolid)
{
	solid=targetSolid;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("3D CAD project illustration");

	InitEnvironment();

	glutKeyboardFunc(KeyBoards);
	glutDisplayFunc(Display);
	glutMainLoop();
}
