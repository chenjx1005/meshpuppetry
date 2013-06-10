#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <mkl.h>
#include <float.h>
#include "vector.h"
#include <memory.h>
#include <mkl_lapacke.h>

#define R CblasRowMajor
#define C CblasColMajor
#define T CblasTrans
#define N CblasNoTrans

class Spatrix;

class Matrix
{
public://private:
	float * num;
	int row, col;
	int * index;
public:
//-------------------------------------------------------------------
//------------------constructors and destructor----------------------
//-------------------------------------------------------------------
	Matrix() throw(): num(NULL), row(0), col(0), index(NULL){}
	Matrix(const int& r, const int& c) throw(): num(new float[r*c]), row(r), col(c), index(NULL){}
	Matrix(const int& r, const int& c, const float * p) throw(): row(r), col(c), index(NULL)
	{
		num = new float[r*c];
		cblas_scopy(r*c, p, INC, num, INC);
	}
	Matrix(const Matrix& m) throw(): row(m.row), col(m.col), index(NULL)
	{
		num = new float[row*col];
		cblas_scopy(row*col, m.num, INC, num, INC);
	}
	Matrix(int r, const float p[][3]) throw(): row(r), col(3), index(NULL)
	{
		num = new float[row*col];
		memcpy(num, p[0], sizeof(float)*row*col);
	}
	~Matrix()
	{
		delete [] num;
		delete [] index;
	}
//-------------------------------------------------------------------
//--------------------assignment of a matrix-------------------------
//-------------------------------------------------------------------
	inline void zerolize() throw()
	{
		memset(num, 0, sizeof(float)*row*col);
	}
//-------------------------------------------------------------------
//----------------------characters of a Matrix-----------------------
//-------------------------------------------------------------------
	inline int rows()const throw()
	{
		return row;
	}
	inline int cols()const throw()
	{
		return col;
	}  
	inline float* nums()const throw()
	{
		return num;
	}
	inline float trace()const throw()
	{
		if(row != col)
			return FLT_MAX;
		float trace = 0.0f;
		for(int i=0; i<row; i++)
			trace += *(num+i*col+i);
		return trace;
	}
	inline float norm2() const throw()
	{
		float norm = 0;
		for(int i=0; i<row*col; i++)
			norm += *(num+i) * *(num+i);
		return norm;
	}
//-------------------------------------------------------------------
//------------------------unary operations---------------------------
//-------------------------------------------------------------------
	void trans() throw();

	inline bool show() const throw()
	{
		if(num == NULL)
			return false;
		for(int i=0; i<row; i++)
		{
			for(int j=0; j<col; j++)
			{
				printf("%f\t", *(num+i*col+j));
			}
			printf("\n");
		}
		return true;
	}
	inline Matrix operator-() const throw()
	{
		Matrix temp(*this);
		cblas_saxpy(row*col, -2.0f, num, INC, temp.num, INC);
		return temp;
	}
	inline float* operator[](const int& r) const throw()
	{
		return num + r*col;
	}
	inline float operator()(const int& r, const int& c) const throw()
	{
		return *(num+r*col+c);
	}
//-------------------------------------------------------------------
//------------------operations that contain = -----------------------
//-------------------------------------------------------------------
	inline Matrix& operator=(const Matrix& m) throw()
	{
		if(row*col != m.row*m.col)
		{
			delete [] num;
			num = new float[m.row*m.col];
		}
		row = m.row;
		col = m.col;
		delete [] index;
		cblas_scopy(row*col, m.num, INC, num, INC);
		return *this;
	}
	inline Matrix& operator*=(const float& a) throw()
	{
		cblas_sscal(row*col, a, num, INC);
		return *this;
	}
	Matrix& operator*=(const Matrix& m) throw();
	inline Matrix& operator+=(const Matrix& m) throw()
	{
		if(row!=m.row || col!=m.col)
		{
			printf("To perform + needs two matrix with the same dimension\n");
			return *this;
		}
		cblas_saxpy(row*col, 1.0f, m.num, INC, num, INC);
		return *this;
	}
	inline Matrix& operator-=(const Matrix& m) throw()
	{
		if(row!=m.row || col!=m.col)
		{
			printf("To perform - needs two matrix with the same dimension\n");
			return *this;
		}
		cblas_saxpy(row*col, -1.0f, m.num, INC, num, INC);
		return *this;
	}
//-------------------------------------------------------------------
//----------------------multiple operations--------------------------
//-------------------------------------------------------------------
	inline friend Matrix operator+(const Matrix& a, const Matrix& b) throw();
	inline friend Matrix operator-(const Matrix& a, const Matrix& b) throw();
	inline friend Matrix operator*(const Matrix& a, const Matrix& b) throw();
	inline friend Matrix operator*(const float& a, const Matrix& m) throw();
	inline friend Matrix operator*(const Matrix& m, const float& a) throw();
//-------------------------------------------------------------------
//-------------------------linear system-----------------------------
//-------------------------------------------------------------------
	inline bool tolu() throw()
	{
		assert(row == col);
		index = new int[row];
		int flag = LAPACKE_sgetrf(R, row, col, num, col, index);
		return flag?false:true;
	}
	friend bool solve(const Matrix& m, Vector& v, char t) throw();
	friend bool solve(const Matrix& m, const Matrix& v, char t) throw();
	friend void trans(const Matrix& s, Matrix& d) throw();
	friend void mcopy(const Matrix& s, Matrix& d) throw();

	friend void sdsolve(float alpha, char t, Spatrix& a, Matrix&b, Matrix& c) throw();
	friend void sdproduct(float alpha, char t, Spatrix& a, Matrix& b, float beta, Matrix& c) throw();
};


inline Matrix operator+(const Matrix& a, const Matrix& b) throw()
{
	Matrix temp(a);
	return (temp += b);
}
inline Matrix operator-(const Matrix& a, const Matrix& b) throw()
{
	Matrix temp(a);
	return (temp -= b);
}
inline Matrix operator*(const Matrix& a, const Matrix& b) throw()
{
	Matrix temp(a);
	return (temp *= b);
}
inline Matrix operator*(const float& a, const Matrix& m) throw()
{
	Matrix temp(m);
	return (temp *= a);
}
inline Matrix operator*(const Matrix& m, const float& a) throw()
{
	Matrix temp(m);
	return (temp *= a);
}

#endif			