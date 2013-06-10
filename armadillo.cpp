#include <QtGui>
#include <QtOpenGL>
#include "GLU.h"
#include <string>
#include <stdlib.h>
using namespace std;

#include "..\Matrix\Matrix\Matrix.h"
#include "..\Matrix\Matrix\timer.h"
#include "armadillo.h"
#include "File.h"

using namespace std;
#define pi 3.14159265359
#define BUFSIZE 512

extern QString filename;

armadillo::armadillo(QObject *parent) :
    QObject(parent)
{
	vertex = NULL;
	normal = NULL;
	face = NULL;
	LtLx = NULL;
	cLtLx = NULL;
	A = NULL;
	len = NULL;
	xhat = NULL;

	ori = NULL;
	Lx = NULL;

	//method 3
	topo = NULL;
	coff = NULL;
	face_normal = NULL;

	method = 1;
	radius = 5.0f;

    InitialModel(filename);
	fixv_num=0;
	alpha = 50.0f;
	select = 1;
    cptr = -1;
}

armadillo::~armadillo()
{
	if(face != NULL) delete [] face;
	if(vertex != NULL) delete vertex;
	if(normal != NULL) delete normal;
	if(LtLx != NULL) delete LtLx;
	if(cLtLx != NULL) delete cLtLx;
	if(A != NULL) delete A;
	if(len != NULL) delete len;
	if(xhat != NULL) delete xhat;

	if(ori != NULL) delete ori;
	if(Lx != NULL) delete Lx;

	//method three
	if(topo != NULL) delete [] topo;
	if(coff != NULL) delete [] coff;
	if(face_normal != NULL) delete face_normal;
}

void armadillo::draw_scene()
{
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glNormalPointer(GL_FLOAT,0,normal->num);
    glVertexPointer(3,GL_FLOAT,0,vertex->num);
    glDrawElements(GL_TRIANGLES,
          nface*3,
          GL_UNSIGNED_INT,
          face);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
	if(cptr!=-1)
	{
		glPushMatrix();
		glTranslatef(vertex->num[cptr*3],vertex->num[cptr*3+1],vertex->num[cptr*3+2]);
		glColor4f(1,0,1,0.8f);
		glutSolidSphere( radius, 15, 15 );
		glColor3f(1,1,1);
		glPopMatrix();
	}
	if(fixv_num)
	{
		for(int j=0;j<fixv_num;j++)
		{
			glPushMatrix();
			glTranslatef(vertex->num[fixv[j]*3],vertex->num[fixv[j]*3+1],vertex->num[fixv[j]*3+2]);
			glColor4f(0,1,1,0.8f);
			glutSolidSphere( radius, 15, 15 );
			glColor3f(1,1,1);
			glPopMatrix();
		}
	}
	//glPushMatrix();
	//glTranslatef(vertex->num[1635*3],vertex->num[1635*3+1],vertex->num[1635*3+2]);
	//glColor4f(1,0,1,0.8f);
	//glutSolidSphere( radius*5, 15, 15 );
	//glColor3f(1,1,1);
	//glPopMatrix();
}

void armadillo::draw_vertex()
{
    for(int i=0;i<nvertex;i++)
    {
        glLoadName(i);
        glEnable(GL_POINT_SMOOTH);
        glPointSize(5.0);
        glBegin (GL_POINTS);
        if(select == 0)
		{
			if(i == cptr)
				glColor3f(0,1,0);
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
        glNormal3fv(normal->num+i*3);
        glVertex3fv(vertex->num+i*3);
        glEnd ();
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
	(*vertex)[cptr][0] = x;
    (*vertex)[cptr][1] = y;
    (*vertex)[cptr][2] = z;
}

void armadillo::up()
{
	(*vertex)[cptr][1] += 0.5f;
	this->transform();
}
void armadillo::down()
{
	(*vertex)[cptr][1] -= 0.5f;
	this->transform();
}
void armadillo::left()
{
	(*vertex)[cptr][0] += 0.5f;
	this->transform();
}
void armadillo::right()
{
	(*vertex)[cptr][0] -= 0.5f;
	this->transform();
}

void armadillo::setfixv(int p)
{
	fixv[fixv_num++]=p;
}

void armadillo::setalpha(int value)
 {
	 alpha = (float)value;
 }

void armadillo::setmethod(int m)
{
	method = m;
}

void armadillo::setradius(int r)
{
	radius = r/10.0f;
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

void format(Matrix & a, const Matrix & b) throw()
{
	assert(a.col==3 && b.row==a.row && 1==b.col);
	int i, j;
	float t;
	for(i=0; i<a.row; ++i)
	{
		t = 0;
		for(j=0; j<3; ++j)
		{
			t += a.num[i*3+j]*a.num[i*3+j];
		}
		t = sqrt(t);
		if(fabs(t) > 0.0001f)
		{
			for(j=0; j<3; ++j)
			{
				a.num[i*3+j] *= (b.num[i]/t);
			}
		}
	}
}

void cal_normal(float a[], float b[], float c[], float n[])
{
	float v1[3], v2[3], t=0;
	v1[0] = b[0]-a[0];
	v1[1] = b[1]-a[1];
	v1[2] = b[2]-a[2];
	v2[0] = c[0]-a[0];
	v2[1] = c[1]-a[1];
	v2[2] = c[2]-a[2];
	n[0] = v1[1]*v2[2]-v1[2]*v2[1];
	n[1] = v1[2]*v2[0]-v1[0]*v2[2];
	n[2] = v1[0]*v2[1]-v1[1]*v2[0];
	t = n[0]*n[0]+n[1]*n[1]+n[2]*n[2];
	t = sqrt(t);
	if(fabs(t) > 0.0001f)
	{
		n[0] /= t;
		n[1] /= t;
		n[2] /= t;
	}
}

bool iszero(float a[], bool first)
{
	if(first != true)
		return false;
	for(int i=0; i<3; ++i)
	{
		if(fabs(a[i])>0.0000001f)
			return false;
	}
	return true;
}

inline bool isvalid(float x)
{
    return (x <= FLT_MAX && x >= -FLT_MAX);
}

void cal_coff(Matrix& L, Matrix& norm, vector<int>* topo, vector<float>* coff)
{
	int n;
	bool flag;
	float la;
	for(int i=0; i<L.row; ++i)
	{
		n = topo[i].size();
		coff[i].resize(n);
		la = 0.00000001f;
		bool first = true;
		while(iszero(&coff[i][0], first) || !isvalid(coff[i][0]))
		{
			Matrix ata(n, 3), a(3, n);
			for(int j=0; j<n; ++j)
				memcpy(ata.num+j*3, norm.num+topo[i][j]*3, 3*sizeof(float));
			Matrix atb(n, 1);
			atb.zerolize();
			trans(ata, a);
			for(int j=0; j<n; ++j)
			{
				for(int k=0; k<3; ++k)
					atb.num[j] += ata.num[j*3+k]*L.num[i*3+k];
			}
			ata *= a;

			//ata.write("ata.txt");

			if(first == true)
				first = false;
			else
			{
				for(int k=0; k<ata.row; ++k)
					ata[k][k] += la;
				la *= 10.0f;
			}
			ata.tolu();
			flag = solve(ata, atb, 'n');
			//assert(flag == true);
			memcpy(&coff[i][0], atb.num, n*sizeof(float));
		}
	}
}

void cal_lv(vector<int> * topo, vector<float> * coff, Matrix& norm, Matrix& res)
{
	res.zerolize();
	int size;
	float * a, *b, *c;
	for(int i=0; i<res.row; ++i)
	{
		size = topo[i].size();
		assert(size >= 3);
		a = res.num+i*3;
		b = a + 1;
		c = b + 1;
		for(int j=0; j<size; ++j)
		{
			*a += coff[i][j]*norm[topo[i][j]][0];
			*b += coff[i][j]*norm[topo[i][j]][1];
			*c += coff[i][j]*norm[topo[i][j]][2];
		}
	}
}

void check(int p, int face[], int num, Matrix& ee, Matrix& er)
{
	bool in;
	for(int i=0; i<num; ++i)
	{
		in = false;
		for(int j=0; j<3; ++j)
		{
			if(face[i*3+j] == p)
			{
				in = true;
				break;
			}
		}
		if(in == true)
		{
			for(int k=0; k<3; ++k)
				memcpy(ee.num+3*face[i*3+k],
					er.num+3*face[i*3+k],
					3*sizeof(float));
		}
	}
}
			
void armadillo::laplacian(int t)
{
	static int move = -2, num = -2;
	static float cof = -1;
	//float a = (1-alpha)/alpha;
	Matrix temp(nvertex, 3);

		if(move!=cptr || num!=fixv_num || cof!=alpha)
		{
			Timer t;
			sA.sparse(*A);
			sAtA = sA;
			ssadd(sLtL, alpha, 'n', sAtA, sAtA);
			move = cptr;
			num = fixv_num;
			cof = alpha;
		}
		
		if(method == 2)
		{
			Timer t;
			sdmult(1.0f, 'N', sL, *vertex, 0.0f, temp);
			format(temp, *len);
			//check(cptr, face, nface, temp, *Lx);
			//temp.write("temp.txt");
			sdmult(1.0f, 'N', sLt, temp, 0.0f, *cLtLx);
			//cLtLx->write("temp2.txt");
		}
		else if(method == 3)
		{
			Timer t;
			for(int i=0; i<nface; ++i)
			{
				cal_normal(vertex->num+3*face[i*3],
					vertex->num+3*face[i*3+1],
					vertex->num+3*face[i*3+2],
					face_normal->num+i*3);
			}
			cal_lv(topo, coff, *face_normal, temp);
			//check(cptr, face, nface, temp, *Lx);
			//temp.write("temp.txt");
			sdmult(1.0f, 'N', sLt, temp, 0.0f, *cLtLx);
			//cLtLx->write("temp2.txt");
		}
		else
		{
			Timer t;
			//sdmult(1.0f, 'T', sL, *Lx, 0.0f, *cLtLx);
			//cLtLx->write("cLtLx.temp");
			mcopy(*LtLx, *cLtLx);
		}
		sdmult(alpha, 'n', sA, *xhat, 1.0f, *cLtLx);
		bool flag = sdsolve(sAtA, *cLtLx);
		assert(flag == true);
		cLtLx->torow(*vertex);
}

void armadillo::transform()
{
	(*A)[cptr][cptr] = 1;
	(*xhat)[cptr][0] = (*vertex)[cptr][0];
	(*xhat)[cptr][1] = (*vertex)[cptr][1];
	(*xhat)[cptr][2] = (*vertex)[cptr][2];
	for(int i=0; i<fixv_num; i++)
	{
		(*A)[fixv[i]][fixv[i]] = 1;
		(*xhat)[fixv[i]][0] = (*vertex)[fixv[i]][0];
		(*xhat)[fixv[i]][1] = (*vertex)[fixv[i]][1];
		(*xhat)[fixv[i]][2] = (*vertex)[fixv[i]][2];
	}
	laplacian(cptr);
	(*A)[cptr][cptr] = 0;
	(*xhat)[cptr][0] = 0;
	(*xhat)[cptr][1] = 0;
	(*xhat)[cptr][2] = 0;
	for(int i=0; i<fixv_num; i++)
	{
		(*A)[fixv[i]][fixv[i]] = 0;
		(*xhat)[fixv[i]][0] = 0;
		(*xhat)[fixv[i]][1] = 0;
		(*xhat)[fixv[i]][2] = 0;
	}
}

float dist(float a[], float b[])
{
	float d = 0;
	d += (a[0]-b[0])*(a[0]-b[0]);
	d += (a[1]-b[1])*(a[1]-b[1]);
	d += (a[2]-b[2])*(a[2]-b[2]);
	return sqrt(d);
}

float dot(float a[], float b[])
{
	return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

void cal_cot(Matrix& p, int a, int b, int c, float& u, float& v, float& w)
{
	float A = dist(p.num+3*b, p.num+3*c);
	float B = dist(p.num+3*a, p.num+3*c);
	float C = dist(p.num+3*a, p.num+3*b);
	//u = A;
	//v = B;
	//w = C;
	u = (B*B+C*C-A*A)/2/B/C;
	assert(u>=-1 && u<=1);
	v = (A*A+C*C-B*B)/2/A/C;
	assert(v>=-1 && v<=1);
	w = (A*A+B*B-C*C)/2/A/B;
	assert(w>=-1 && w<=1);
	u = u/sqrt(1-u*u);
	v = v/sqrt(1-v*v);
	w = w/sqrt(1-w*w);
}

void armadillo::initialTrans()
{
	//DEBUG
	if(ori != NULL) delete ori;
	ori = new Matrix(nvertex, 3);
	//mcopy(*vertex, *ori);

	//can be temp when solve with spatrix
	Matrix LtL(nvertex, nvertex), L(nvertex, nvertex);
	unsigned int a, b, c;
	float u, v, w;
	L.zerolize();	A->zerolize();	xhat->zerolize();

	//method three
	if(topo != NULL) delete topo;
	if(coff != NULL) delete coff;
	if(face_normal != NULL) delete face_normal;
	topo = new vector<int>[nvertex];
	coff = new vector<float>[nvertex];
	face_normal = new Matrix(nface, 3);

	for(int i=0; i<nface; i++)
	{
		a = face[i*3];
		b = face[i*3+1];
		c = face[i*3+2];
		cal_cot(*vertex, a, b, c, u, v, w);
		/*L[a][a] += (w + v);
		L[b][b] += (w + u);
		L[c][c] += (u + v);
		L[a][b] -= w;
		L[b][a] -= w;
		L[a][c] -= v;
		L[c][a] -= v;
		L[b][c] -= u;
		L[c][b] -= u;*/
		L[a][a] -= (w + v);
		L[b][b] -= (w + u);
		L[c][c] -= (u + v);
		L[a][b] += w;
		L[b][a] += w;
		L[a][c] += v;
		L[c][a] += v;
		L[b][c] += u;
		L[c][b] += u;
		/*L[a][a] -= 1;//+= (w + v);
		L[b][b] -= 1;//+= (w + u);
		L[c][c] -= 1;//+= (u + v);
		L[a][b] =1;//-= w;
		L[b][a] =1;//-= w;
		L[a][c] =1;//-= v;
		L[c][a] =1;//-= v;
		L[b][c] =1;//-= u;
		L[c][b] =1;//-= u;
		*///method 3
		topo[a].push_back(i);
		topo[b].push_back(i);
		topo[c].push_back(i);
		cal_normal(vertex->num+a*3, vertex->num+b*3,
			vertex->num+c*3, face_normal->num+i*3);
	}
	
	mcopy(L, LtL);
	LtL.trans();
	sLt.sparse(LtL);
	LtL *= L;
	sLtL.sparse(LtL);
	mcopy(LtL, *LtLx);
	(*LtLx) *= (*vertex);
	//LtLx->write("LtLx.txt");

	sL.sparse(L);
	L *= (*vertex);
	//L.write("Lx.txt");
	if(Lx != NULL) delete Lx;
	Lx = new Matrix(L);

	//method 3
	cal_coff(L, *face_normal, topo, coff);

	//method 2
	if(len != NULL) delete len; 
	len = new Matrix(nvertex, 1);
	float t;
	for(a=0; a<nvertex; ++a)
	{
		t = 0;
		for(b=0; b<3; ++b)
		{
			t += L.num[a*3+b]*L.num[a*3+b];
		}
		t = sqrt(t);
		len->num[a] = t;
	}
}

void armadillo::InitialModel(QString filename)
{
	if(filename=="")	
		filename = "E:\\Course\\MeshPuppetry\\Mesh\\bigcube.obj";
	string s = filename.toStdString();
	file f(s.c_str(), "r");
	f.ignore(':');
	f.geti(nvertex);
	f.ignore(':');
	f.geti(nface);
	f.ignore();

	if(vertex != NULL) delete vertex;
	if(normal != NULL) delete normal;
	if(face != NULL) delete [] face;
	vertex = new Matrix(nvertex, 3);
	normal = new Matrix(nvertex, 3);
	face = new int[nface*3];

	char ch;
	int v=0, n=0, fp=0;
	while(!f.eof())
	{
		ch = f.getch();
		if(ch == '\n')
			continue;
		else if(ch == EOF)
			break;
		else if(ch == 'v')
		{
			ch = f.getch();
			if(ch == 'n')
			{
				f.getf(normal->num[n*3]);
				f.getf(normal->num[n*3+1]);
				f.getf(normal->num[n*3+2]);
				++ n;
			}
			else
			{
				f.getf(vertex->num[v*3]);
				f.getf(vertex->num[v*3+1]);
				f.getf(vertex->num[v*3+2]);
				++ v;
			}
		}
		else if(ch == 'f')
		{
			f.geti(face[fp*3]);
			-- face[fp*3];
			f.ignore(' ');
			f.geti(face[fp*3+1]);
			-- face[fp*3+1];
			f.ignore(' ');
			f.geti(face[fp*3+2]);
			-- face[fp*3+2];
			++ fp;
		}
		if(!f.ignore())
			break;
	}
	if(LtLx != NULL) delete LtLx;
	if(cLtLx != NULL) delete cLtLx;
	if(A != NULL) delete A;
	if(xhat != NULL) delete xhat;
	//(*vertex) *= 10.0f;
	LtLx = new Matrix(nvertex, nvertex);
	cLtLx = new Matrix(nvertex, 3);
	A = new Matrix(nvertex, nvertex);
	xhat = new Matrix(nvertex, 3);
	initialTrans();
}

void armadillo::Export(QString Exportname)
{
	FILE * fp;
	string s = Exportname.toStdString();
	fopen_s(&fp, s.c_str(), "w");
	fprintf_s(fp, "haha");
	fclose(fp);
}

/*
void armadillo::laplacian2()
{
	static int move = -2, num = -2;
	float a = 10*(1-alpha)/alpha;
	
		printf("\n\n");
		if(move!=cptr || num!=fixv_num)
		{
			Timer t;
			sA.sparse(*A);
			sAtA = sA;
			//ssmult('n', sAtA, sAtA, sAtA);
			ssadd(sLtL, a, 'n', sAtA, sAtA);
			move = cptr;
			num = fixv_num;
		}
		{
			Timer t;
			Matrix temp(nvertex, 3);
			sdmult(1.0f, 'N', sL, *vertex, 0.0f, temp);
			format(temp, *len);
			sdmult(1.0f, 'N', sLt, temp, 0.0f, *cLtLx);
			//mcopy(temp, *cLtLx);
		}
		{
			Timer t;
			sdmult(a, 'n', sA, *xhat, 1.0f, *cLtLx);
		}
		{
			Timer t;
			bool flag = sdsolve(sAtA, *cLtLx);
			assert(flag == true);
		}
		{
			Timer t;
			//cLtLx->torow(*vertex);
			for(int i=0; i<nvertex; ++i)
			{
				for(int j=0; j<3; ++j)
					(*vertex)[i][j] = cLtLx->num[j*nvertex+i];
			}
		}
}

void armadillo::laplacian3()
{
	static int move = -2, num = -2;
	float a = 10*(1-alpha)/alpha;
	
		printf("\n\n");
		if(move!=cptr || num!=fixv_num)
		{
			Timer t;
			sA.sparse(*A);
			sAtA = sA;
			//ssmult('n', sAtA, sAtA, sAtA);
			ssadd(sLtL, a, 'n', sAtA, sAtA);
			move = cptr;
			num = fixv_num;
		}
		{
			Timer t;
			Matrix temp(nvertex, 3);
			for(int i=0; i<nface; ++i)
			{
				cal_normal(vertex->num+3*face[i*3],
					vertex->num+3*face[i*3+1],
					vertex->num+3*face[i*3+2],
					face_normal->num+i*3);
			}
			cal_lv(topo, coff, *face_normal, temp);
			sdmult(1.0f, 'N', sLt, temp, 0.0f, *cLtLx);
		}
		{
			Timer t;
			sdmult(a, 'n', sA, *xhat, 1.0f, *cLtLx);
		}
		{
			Timer t;
			bool flag = sdsolve(sAtA, *cLtLx);
			assert(flag == true);
		}
		{
			Timer t;
			//cLtLx->torow(*vertex);
			for(int i=0; i<nvertex; ++i)
			{
				for(int j=0; j<3; ++j)
					(*vertex)[i][j] = cLtLx->num[j*nvertex+i];
			}
		}
}
*/