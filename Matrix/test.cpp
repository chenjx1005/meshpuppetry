#include <time.h>
#include "Matrix.h"
#include "Spatrix.h"
#include "timer.h"
#include <iostream>
using namespace std;
#define line cout<<endl;

void cmp(const Matrix& m, const Matrix& n);

float a[16] = {
	2, 0, 0, 0,
	0, 3, 0, 6,
	0, 0, 4, 0,
	1, 0, 0, 5
};
float b[16] = {
	0, 3, 0, 0,
	2, 2, 0, 0,
	0, 0, 0, 1,
	1, 0, 3, 0
};

int main()
{
	Matrix a44(4, 4, a), a43(4, 3, a), a34(3, 4, a);
	Matrix b44(4, 4, b), b43(4, 3, b), b34(3, 4, b);
	Spatrix sa44(a44), sa43(a43), sa34(a34);
	Spatrix sb44(b44), sb43(b43), sb34(b34);
	Spatrix sr;
	Matrix r, r44(a44), r43(a43), r34(a34), r33(3, 3);

	sdmult(2.0f, 't', sa44, b44, 0.0f, r44);
	r44.show();
	r = 2.0f*a44.tran()*b44;
	r.show();

	sdmult(2.0f, 't', sa34, b34, 0.0f, r44);
	r44.show();
	r = 2.0f*a34.tran()*b34;
	r.show();

	sdmult(2.0f, 't', sa43, b43, 0.0f, r33);
	r33.show();
	r = 2.0f*a43.tran()*b43;
	r.show();
	return 0;
}
/*
const int x = 7;
const int y = 8;
const int z = 9;
const int l = y*z;
float a[l];
float b[l];
int main()
{
	int i, j, k;
	srand(time(NULL));
	for(i=0; i<l; ++i)
	{
		j = rand()%l;
		k = rand()%l;
		if(j%2 != 0)
			a[i] = 0;
		else
			a[i] = k;
		b[i] = j;
	}
	Matrix a0(y, y, a), a1(y, x, a), a2(y, x, b);
	a0.show();
	Matrix a3(y, y, a), a4(y, y, b);
	Matrix a5(y, z, a), a6(y, z, b);
	cout << 'a' << endl;
	Matrix b1(y, x, a), b2(y, x, b);
	Matrix b3(y, y, a), b4(y, y, b);
	Matrix b5(y, z, a), b6(y, z, b);
	cout << 'b' << endl;
	//Matrix c1(y, x, a), c2(y, x, b);
	//Matrix c3(y, y, a), c4(y, y, b);
	//Matrix c5(y, z, a), c6(y, z, b);
	//cout << 'c' << endl;
	Spatrix s(a0);
	//s.sparse(a3);
	//cmp(a0, a3);
	cout << 's' << endl;

	a0.tolu();

	//timer t;
	bool f = Spatrix::sdsolve(s, a1, b1);
	printf("%s\n", f?"true":"flase");
	//t.clock();
	//t.clock();
	solve(a0, a1, 'n');
	//t.clock();
	cmp(a1, b1);
	cout << '1' << endl;

	//t.clock();
	Spatrix::sdsolve(s, b2, b2);
	//t.clock();
	//t.clock();
	solve(a0, a2, 'n');
	//t.clock();
	cmp(a2, b2);
	cout << '2' << endl;

	Spatrix::sdsolve(s, b3, b3);
	solve(a0, a3, 'n');
	cmp(a3, b3);
	cout << '3' << endl;

	Spatrix::sdsolve(s, b4, b4);
	solve(a0, a4, 'n');
	cmp(a4, b4);
	cout << '4' << endl;

	Spatrix::sdsolve(s, b5, b5);
	solve(a0, a5, 'n');
	cmp(a5, b5);
	cout << '5' << endl;

	Spatrix::sdsolve(s, b6, b6);
	solve(a0, a6, 'n');
	cmp(a6, b6);
	cout << '6' << endl;
	getchar();
}
*/

void cmp(const Matrix& m, const Matrix& n)
{
	float ab = 0;
	float re = 0;

	if(m.col != n.col || m.row != n.row)
	{
		cout << "error" << endl;
		return;
	}

		for(int i=0; i<m.row*m.col; i++)
		{
			if(fabs(m.num[i])<0.001 && fabs(m.num[i])<0.001)
				continue;
			if(fabs(*(m.num+i)-*(n.num+i))/fabs(n.num[i]) > re)
			{
				re = fabs(*(m.num+i)-*(n.num+i))/fabs(n.num[i]);
			}
			if(fabs(m.num[i]-n.num[i]) > ab)
				ab = fabs(m.num[i]-n.num[i]);
			//cout << m.num[i] << ' ';
			//cout << n.num[i] << endl;
		}
		cout << ab << endl;
		cout << re << endl;
}
