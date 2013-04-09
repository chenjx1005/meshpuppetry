#include <QtGui>
#include <QtOpenGL>
#include "GLU.h"
#include <stdlib.h>
#include <GL/glut.h>
#include <algorithm>

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
    //fixv_num=0;
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
	vector <int>::iterator result,iter;
	for(int i=0;i<VERTEX;i++)
    {
        glLoadName(i);
        glEnable(GL_POINT_SMOOTH);
        glPointSize(5.0);
        glBegin (GL_POINTS);
        /*if(select == 0)//如果当前选点模式为移动点
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
			//for(iter=fixv.begin();iter!=fixv.end();iter++)
			//{
				//if(i == *iter){
					//glColor3f(0,1,0);
					//break;
				//}
			//}
			result=find(fixv.begin(),fixv.end(),i);
			if(result == fixv.end())
				glColor3f(1,1,1);
			else 
				glColor3f(0,1,0);
		}*/
        glNormal3fv(Normal[i]);
        glVertex3fv(Vertex[i]);
        glEnd ();
     }
	 if(cptr!=-1){
		glPushMatrix();
		glTranslatef(Vertex[cptr][0],Vertex[cptr][1],Vertex[cptr][2]);
		glColor4f(1,0,0,0.2f);
		glutSolidSphere( 2.0f, 15, 15 );
		glColor3f(1,1,1);
		glPopMatrix();
		}
	 if(!fixv.empty()){
		for(iter=fixv.begin();iter!=fixv.end();iter++)
		{
			glPushMatrix();
		    glTranslatef(Vertex[*iter][0],Vertex[*iter][1],Vertex[*iter][2]);
		    glColor4f(1,1,0,0.2f);
		    glutSolidSphere( 2.0f, 15, 15 );
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
	fixv.push_back(p);
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
	fixv.clear();
}

void armadillo::cselect(bool value)
{
	select = value;

}
