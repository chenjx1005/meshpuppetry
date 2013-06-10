
#include "timer.h"
#include "Matrix.h"
#include "Spatrix.h"
#include "common.h"

extern float vertex[VERTEX][3];
extern int face[FACE][3];

Spatrix sLtL, sAtA, sA;
Matrix LtLx(VERTEX, VERTEX), cLtLx(VERTEX, 3);
Matrix A(VERTEX, VERTEX), xhat(VERTEX, 3);


void initial()
{
	freopen("metric.txt", "w", stdout);
	//can be temp when solve with spatrix
	Matrix LtL(VERTEX, VERTEX), x(VERTEX, vertex), L(VERTEX, VERTEX);
	unsigned int a, b, c;
	L.zerolize();	A.zerolize();	xhat.zerolize();
	for(int i=0; i<FACE; i++)
	{
		a = face[i][0];
		b = face[i][1];
		c = face[i][2];
		L[a][a] = L[a][a] - 1;
		L[b][b] = L[b][b] - 1;
		L[c][c] = L[c][c] - 1;
		L[a][b] = L[b][a] = 1;
		L[a][c] = L[c][a] = 1;
		L[b][c] = L[c][b] = 1;
	}
	mcopy(L, LtL);
	LtL.trans();
	LtL *= L;
	sLtL.sparse(LtL);
	mcopy(LtL, LtLx);
	LtLx *= x;
}

//target = reverse(alpha*AtA+LtL)*(alpha*At*xhat+LtL*x)
void laplacian(float alpha, float vertex[][3], int move, int num)
{
	static int lmove = -1, lnum = -1;

	alpha = 10*(1-alpha)/alpha;
	
		printf("\nSpatrix\n");
		if(move!=lmove || num!=lnum)
		{
			Timer t;
			sA.sparse(A);
			sAtA = sA;
			ssmult('n', sAtA, sAtA, sAtA);
			ssadd(sLtL, alpha, 'n', sAtA, sAtA);
			lmove = move;
			lnum = num;
		}
		{
			Timer t;
			mcopy(LtLx, cLtLx);
		}
		{
			Timer t;
			sdmult(alpha, 'n', sA, xhat, 1.0f, cLtLx);
		}
		{
			Timer t;
			bool flag = sdsolve(sAtA, cLtLx);
			assert(flag == true);
		}
		{
			Timer t;
			for(int i=0; i<VERTEX; ++i)
			{
				for(int j=0; j<3; ++j)
					vertex[i][j] = cLtLx.num[j*VERTEX+i];
			}
		}
	//else
	//{
	//	printf("\nMatrix\n");
	//	Timer t;
	//
	//	Matrix AtA(A);
	//	Matrix Atxhat(AtA);
	//	AtA *= A;
	//	AtA *= alpha;
	//	AtA += LtL;
	//	Atxhat *= xhat;
	//	Atxhat *= alpha;
	//	Atxhat += LtLx;
	//	AtA.tolu();
	//	solve(AtA, Atxhat, 'N');
	//	memcpy(vertex[0], Atxhat.nums(), sizeof(float)*VERTEX*3);
	//}

	//dense = !dense;
}
