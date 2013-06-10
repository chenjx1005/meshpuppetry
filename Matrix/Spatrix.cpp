#include "Spatrix.h"

int Spatrix::pt[64], Spatrix::maxfct, Spatrix::mnum, Spatrix::mtype, Spatrix::phase;
int Spatrix::iparm[64], Spatrix::msglvl, Spatrix::error, Spatrix::idum;
double Spatrix::ddum;


bool sdsolve(Spatrix& a, Matrix& b, Matrix& x) throw()
{
	assert(a.row == a.col);
	assert(a.row == b.row);
	assert(x.col = b.col);

	memset(Spatrix::pt, 0, sizeof(Spatrix::pt));
	memset(Spatrix::iparm, 0, sizeof(Spatrix::iparm));
	Spatrix::iparm[ 0] = 1;	//No solver default
	Spatrix::iparm[27] = 1;	//Single Precision
	Spatrix::iparm[34] = 1-a.index[0];	//Base
	Spatrix::maxfct = Spatrix::mnum = 1;
	Spatrix::mtype = 11;
	Spatrix::phase = 13;
	Spatrix::msglvl = 0;

	PARDISO (Spatrix::pt, &Spatrix::maxfct, &Spatrix::mnum, &Spatrix::mtype, &Spatrix::phase,
		&a.row, a.value, a.index, a.column, &Spatrix::idum,
		&b.col,	Spatrix::iparm, &Spatrix::msglvl, b.num, x.num, &Spatrix::error);
	if(Spatrix::error != 0) return false;
	Spatrix::phase = -1;
	PARDISO (Spatrix::pt, &Spatrix::maxfct, &Spatrix::mnum, &Spatrix::mtype, &Spatrix::phase,
		&a.row, &Spatrix::ddum, a.index, a.column, &Spatrix::idum, 
		&b.col, Spatrix::iparm, &Spatrix::msglvl, &Spatrix::ddum, &Spatrix::ddum, &Spatrix::error);
	if(Spatrix::error != 0) return false;
	return true;
}

bool sdsolve(Spatrix& a, Matrix& x) throw()
{
	Matrix tmp(x.row, x.col);
	x.tocol(tmp);
	return sdsolve(a, tmp, x);
}

void sdmult(float alpha, char t, Spatrix& a, Matrix& b, float beta, Matrix& c) throw()
{
	if(a.index[0] == 1) a.zerobase();
	char descra[4] = {'G', 0, 0, 'C'}; //
	mkl_scsrmm(&t, &a.row, &c.col, &a.col, &alpha, descra, 
	a.value, a.column, a.index, a.index+1, 
	b.num, &b.col, &beta, c.num, &c.col);
}

bool ssadd(Spatrix& a, float beta, char t, Spatrix& b, Spatrix& c) throw()
{
	if(t=='T' || t=='t') 
		assert(a.row==b.col && a.col==b.row);
	else assert(a.row==b.row && a.col==b.col);
	if(a.index[0] == 0) a.onebase();
	if(b.index[0] == 0) b.onebase();

	Spatrix tmp(a.row, a.col, a.size+b.size+1);	//fuck 1
	int flag, request=0, sort=0;

	mkl_scsradd(&t, &request, &sort, &a.row, &a.col, a.value, a.column, a.index, 
		&beta, b.value, b.column, b.index, tmp.value, tmp.column, tmp.index, &tmp.size, &flag);
	tmp.size = tmp.index[tmp.row] - 1;
	c.resize(a.row, a.col, tmp.size);
	memcpy(c.value, tmp.value, c.size*sizeof(float));
	memcpy(c.column, tmp.column, c.size*sizeof(int));
	memcpy(c.index, tmp.index, (c.row+1)*sizeof(int));

	return flag?false:true;
}

bool ssmult(char t, Spatrix& a, Spatrix& b, Spatrix& c) throw()
{
	if(t=='T' || t=='t') 
		assert(a.row == b.row);
	else assert(a.col == b.row);
	if(a.index[0] == 0) a.onebase();
	if(b.index[0] == 0) b.onebase();

	Spatrix tmp;
	tmp.row = (t=='T'||t=='t'?a.col:a.row);
	tmp.col = b.col;
	tmp.size = tmp.row*tmp.col;
	tmp.value = new float[tmp.size];
	tmp.column = new int[tmp.size];
	tmp.index = new int[tmp.row+1];
	int flag, request=0, sort=0;
	mkl_scsrmultcsr(&t, &request, &sort, &a.row, &a.col, &b.col, a.value, a.column, 
		a.index, b.value, b.column, b.index, tmp.value, tmp.column, tmp.index, &tmp.size, &flag);

	tmp.size = tmp.index[tmp.row] - 1;
	c.resize(tmp.row, tmp.col, tmp.size);
	memcpy(c.value, tmp.value, c.size*sizeof(float));
	memcpy(c.column, tmp.column, c.size*sizeof(int));
	memcpy(c.index, tmp.index, (c.row+1)*sizeof(int));
	return flag?false:true;
}

/*
bool sdsolve(Spatrix& a, Matrix& b, Matrix& x) throw()
{
	_MKL_DSS_HANDLE_t handle;
	MKL_INT opt = MKL_DSS_DEFAULTS;
	_INTEGER_t error;
	error = dss_create(handle, opt);
	if(error != MKL_DSS_SUCCESS)
		goto fail;
	
	MKL_INT sym = MKL_DSS_SYMMETRIC;
	error = dss_define_structure(
		handle, sym, a.index, a.row, 
		a.col, a.column, a.index[a.row]);
	if(error != MKL_DSS_SUCCESS)
		goto fail;
	
	error = dss_reorder(handle, opt, 0);
	if(error != MKL_DSS_SUCCESS)
		goto fail;

	MKL_INT type = MKL_DSS_POSITIVE_DEFINITE;
	error = dss_factor_real(handle, type, a.value);
	if(error != MKL_DSS_SUCCESS)
		goto fail;

	error = dss_solve_real(handle, opt, b.num, b.col, x.num);
	if(error != MKL_DSS_SUCCESS)
		goto fail;

	error = dss_delete(handle, opt);
	if(error != MKL_DSS_SUCCESS)
		goto fail;
	return true;
fail:
	printf("\nError:%d in sdsolve()!\n", error);
	return false;
}

/*
void ssproduct(char t, Spatrix& a, Spatrix& b, Matrix& c) throw()
{
	assert(b.col == c.cols());
	if(t == 'N')
		assert(c.rows()==a.row && a.col==b.row)
	else if(t == 'T')
		assert(c.rows()==a.col && a.row==b.row);
	mkl_scsrmultd(&t, &a.row, &a.col, &b.col, a.value, a.column, a.index,
		b.value, b.column, b.index, c.nums(), &b.col);
}
*/