#include <QtGui>
#include <QtOpenGL>
#include "GLU.h"
#include <stdlib.h>
#include <GL/glut.h>

#include "armadillo.h"

#define VERTEX 172974
#define FACE 345944
#define pi 3.14159265359
#define BUFSIZE 512

extern GLfloat vertex[VERTEX][3];
extern GLfloat normal[VERTEX][3];
extern unsigned int face[FACE][3];

armadillo::armadillo(QObject *parent) :
    QObject(parent)
{
    fixv_num=0;
    newv_num=0;
	alpha = 0;
	select = 0;
    cptr = -1;
    for(int i=0;i<VERTEX;i++)
        for(int j=0;j<3;j++){
            Vertex[i][j] = vertex[i][j];
            Normal[i][j] = normal[i][j];
        }
    for(int i=0;i<FACE;i++)
        for(int j=0;j<3;j++){
            Face[i][j] = face[i][j];
        }
}

void armadillo::draw_scene()
{
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glNormalPointer(GL_FLOAT,0,Normal);
    glVertexPointer(3,GL_FLOAT,0,Vertex);
    glDrawElements(GL_TRIANGLES,
          FACE*3,
          GL_UNSIGNED_INT,
          Face);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void armadillo::draw_vertex()
{
	for(int i=0;i<VERTEX;i++)
    {
        glLoadName(i);
        glEnable(GL_POINT_SMOOTH);
        glPointSize(5.0);
        glBegin (GL_POINTS);
        if(select == 0)
		{
			if(i == cptr)
			{
				glColor3f(0,1,0);
			}
			else
				glColor3f(1,1,1);
		}
		else
		{
			int j;
			for(j=0;j<fixv_num;j++)
			{
				if(i == fixv[j]){
					glColor3f(0,1,0);
					break;
				}
			}
			if(j==fixv_num)
				glColor3f(1,1,1);
		}
        glNormal3fv(Normal[i]);
        glVertex3fv(Vertex[i]);
        glEnd ();
     }
	 if(cptr!=-1){
		glPushMatrix();
		glTranslatef(Vertex[cptr][0],Vertex[cptr][1],Vertex[cptr][2]);
		glColor4f(1,0,0,0.2f);
//		glutSolidSphere( 2.0f, 15, 15 );
		glColor3f(1,1,1);
		glPopMatrix();
		}
	 if(fixv_num){
		for(int j=0;j<fixv_num;j++)
		{
			glPushMatrix();
		    glTranslatef(Vertex[fixv[j]][0],Vertex[fixv[j]][1],Vertex[fixv[j]][2]);
		    glColor4f(1,1,0,0.2f);
//		    glutSolidSphere( 2.0f, 15, 15 );
		    glColor3f(1,1,1);
		    glPopMatrix();
		}
	 }
}

void armadillo::setptr(int p)//讲选中点设置为P
{
    cptr = p;
}

int armadillo::getptr()//返回选中点
{
    return cptr;
}

void armadillo::movevertex(float x, float y, float z)
{
    Vertex[cptr][0] = x;
    Vertex[cptr][1] = y;
    Vertex[cptr][2] = z;
}

void armadillo::setfixv(int p)
{
	fixv[fixv_num++]=p;
}

void armadillo::setalpha(int value)
 {
	 alpha = (float)value/100.0;
 }

int armadillo::reselect()
{
	return select;
}

void armadillo::refreshfix()
{
	fixv_num=0;
}

void armadillo::cselect(bool value)
{
	select = value;

}
