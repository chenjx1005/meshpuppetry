#include "matrix.h"
#include "spatrix.h"

Matrix::Matrix(Spatrix& s) throw(): row(s.row), col(s.col), index(NULL)
{
	num = new float[row*col];
	s.dense(*this);
}

void Matrix::trans() throw()
{
	assert(row == col);
	float temp;
	for(int i=0; i<col; i++)
	{
		for(int j=0; j<i; j++)
		{
			temp = *(num+i*col+j);
			*(num+i*col+j) = *(num+j*col+i);
			*(num+j*col+i) = temp;
		}
	}
}

Matrix Matrix::tran() throw()
{
	Matrix t(col, row);
	for(int i=0; i<t.row; i++)
	{
		for(int j=0; j<t.col; j++)
			*(t.num+i*t.col+j) = *(num+j*col+i);
	}
	return t;

}

void trans(const Matrix& s, Matrix& d) throw()
{
	assert(s.col==d.row && s.row==d.col);
	for(int i=0; i<s.row; i++)
		for(int j=0; j<s.col; j++)
			*(d.num+j*d.col+i) = *(s.num+i*s.col+j);
}

Matrix& Matrix::operator*=(const Matrix& m) throw()
{
	assert(col == m.row);
	Matrix temp(*this);
	if(col != m.col)
	{
		delete [] num;
		col = m.col;
		num = new float[row*col];
	}
	cblas_sgemm(R, N, N, row, col, m.row, 1.0f, 
	temp.num, m.row, m.num, m.col, 0.0f, num, col);
	return *this;
}

bool solve(const Matrix& m, Vector& v, char t) throw()
{
	assert(m.index!=NULL && m.row==v.size);
	int flag = LAPACKE_sgetrs(R, t, m.col, 1, m.num, m.col, m.index, v.data, INC);
	return flag?false:true;
}
bool solve(const Matrix& m, Matrix& v, char t) throw()
{
	assert(m.index!=NULL && m.col==v.row);
	int flag = LAPACKE_sgetrs(R, t, m.col, v.col, m.num, m.col, m.index, v.num, v.col);
	return flag?false:true;
}

void mcopy(const Matrix& s, Matrix& d) throw()
{
	assert(s.row==d.row && s.col==d.col);
	cblas_scopy(s.row*s.col, s.num, INC, d.num, INC);
}