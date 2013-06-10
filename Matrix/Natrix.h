#ifndef _NATRIX_H_
#define _NATRIX_H_
#include <stdio.h>
#include <memory.h>
#include <float.h>
#include "vector.h"

class matrix
{
private:
	float * num;
	int row, col;

public:
	matrix() throw(): num(NULL), row(0), col(0){}
	matrix(int r, int c) throw(): num(new float[r*c]), row(r), col(c){}
	matrix(const matrix& m) throw(): row(m.row), col(m.col)
	{
		num = new float[row*col];
		memcpy(num, m.num, sizeof(float)*row*col);
	}
	matrix(int r, int c, float *p) throw(): row(r), col(c)
	{
		num = new float[row*col];
		memcpy(num, p, sizeof(float)*row*col);
	}
	~matrix() throw()
	{
		if(num != NULL)
			delete[] num;
	}

	inline int rows() const throw()
	{
		return row;
	}
	inline int cols() const throw()
	{
		return col;
	}
	inline float* nums() const throw()
	{
		return num;
	}

	inline float* operator[](int r) const throw()
	{
		return num+r*col;
	}
	inline float operator()(int r, int c) const throw()
	{
		return *(num+r*col+c);
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

	inline bool trans() throw()
	{
		if(row != col)
		{
			printf("Transposition can only be performed on Square Matrices\n");
			return false;
		}
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
		return true;
	}

	inline matrix operator-() const throw()
	{
		matrix temp(*this);
		unsigned int size = row*col;
		for(unsigned int i=0; i<size; i++)
			*(temp.num+i) = -*(temp.num+i);
		return temp;
	}
	inline matrix& operator*=(const float& a) throw()
	{
		unsigned int size = row*col;
		for(unsigned int i=0; i<size; i++)
			*(num+i) *= a;
		return *this;
	}
	inline matrix& operator+=(const matrix& m) throw()
	{
		if(row!=m.row || col!=m.col)
		{
			printf("To perform + needs two matrix with the same dimension\n");
			return *this;
		}
		int length = row*col;
		for(int i=0; i<length; i++)
			*(num+i) += *(m.num+i);
		return *this;
	}
	inline matrix& operator-=(const matrix& m) throw()
	{
		if(row!=m.row || col!=m.col)
		{
			printf("To perform - needs two matrix with the same dimension\n");
			return *this;
		}
		int length = row*col;
		for(int i=0; i<length; i++)
			*(num+i) -= *(m.num+i);
		return *this;
	}
	matrix& operator*=(const matrix& m) throw()
	{
		if(col != m.row)
		{
			printf("To perform * needs equal cols and row\n");
			return *this;
		}
		matrix temp(*this);
		int index;
		if(col != m.col)
		{
			delete [] num;
			num = new float[row*m.col];
			col = m.col;
		}
		for(int i=0; i<row; i++)
			for(int j=0; j<col; j++)
			{
				index = i*col+j;
				*(num+index) = 0;
				for(int k=0; k<m.row; k++)
					*(num+index) += *(temp.num+i*temp.col+k)* *(m.num+k*m.col+j);
			}
		return *this;
	}
	inline matrix& operator=(const matrix& m) throw()
	{
		if(row*col != m.row*m.col)
		{
			delete [] num;
			num = new float[m.row*m.col];
		}
		row = m.row;
		col = m.col;
		memcpy(num, m.num, sizeof(float)*row*col);
		return *this;
	}

	bool tolu() throw();
	friend bool solve(const matrix& m, const Vector& v, Vector& r) throw();
	inline friend matrix operator+(const matrix& a, const matrix& b) throw();
	inline friend matrix operator-(const matrix& a, const matrix& b) throw();
	inline friend matrix operator*(const matrix& a, const matrix& b) throw();
};

inline matrix operator+(const matrix& a,const matrix& b) throw()
{
	matrix temp(a);
	return (temp += b);
}
inline matrix operator-(const matrix& a,const matrix& b) throw()
{
	matrix temp(a);
	return (temp -= b);
}
inline matrix operator*(const matrix& a,const matrix& b) throw()
{
	matrix temp(a);
	return (temp *= b);
}
#endif